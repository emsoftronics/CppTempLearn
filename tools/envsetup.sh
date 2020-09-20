function hmm() {
cat <<EOF
Invoke ". build/envsetup.sh" from your shell to add the following functions to your environment:
- croot:   		Changes directory to the top of the tree.
- m|(mp for rpi):       Makes from the top of the tree.
- mm|(mmp for rpi):     Builds all of the modules in the current directory.
- mmm|(mmmp for rpi):   Builds all of the modules in the supplied directories.
           		To limit the modules being built use the syntax: mmm dir/:target1,target2.
- cgrep:   		Greps on all local C/C++ files.
- ggrep:   		Greps on all local Gradle files.
- jgrep:   		Greps on all local Java files.
- resgrep: 		Greps on all local res/*.xml files.
- sgrep:   		Greps on all local source files.
- godir:   		Go to the directory containing a file.

Look at the source to view more functions. The complete list is:
EOF
    T=$(gettop)
    local A
    A=""
    SCRIPT=`find $T/ -name envsetup.sh | head -1`
    if [ ! \( -n $SCRIPT \) ]; then
    	for i in `cat $SCRIPT | sed -n "/^[ \t]*function /s/function \([a-z_]*\).*/\1/p" | sort | uniq`; do
        A="$A $i"
        done
        echo $A
    fi
}


function gettop
{
    local TOPFILE="WORKSPACE"

    local HERE=`PWD= /bin/pwd -P`
    T=$HERE
    while [ \( ! \( -f $TOPFILE \) \) -a \( $PWD != "/" \) ]; do
    	\cd ..
        T=`PWD= /bin/pwd -P`
    done
    \cd $HERE
    if [ -f "$T/$TOPFILE" ]; then
    	echo $T
    fi
}


function bzl-deploy()
{
    local T=
    if [ "$TOP" ]; then
	T=$TOP
    else
	T=$(gettop)
    fi
    local outpath=$T/deploy-bin
    if [ \( $# -gt 0 \) -a \( -d "$1" \) ]; then
	outpath=`readlink -f $1`
	if [ \( "$outpath" = "" \) -o \( ! \( -d "$outpath" \) \) ]; then
		echo "$1 : No such directory exists."
		return
	fi
    fi

    if [ "$outpath" = "$T" ]; then
	outpath=$T/deploy-bin
    fi
    local copiedpaths=
    local HERE=`PWD= /bin/pwd -P`
    mkdir -p $outpath/lib $outpath/local/bin
    \cd $T/bazel-bin
    find ./ -type f -name '*.so' -exec cp -f --parents \{\} $outpath/lib/ \;
    #copiedpaths="$copiedpaths "
    find ./ -name tools -prune -o  -type f -executable -not -name "*.*" -not -name "MANIFEST" -exec cp -f \{\} $outpath/local/bin/ \;
    \cd $outpath/lib/
    libs=`find ./ -type f -name '*.so'`
   
    local dirpaths= 
    for lib in $libs; do
	liblink=${lib/.\//lib}
	liblink=${liblink//_/_U}
	liblink=${liblink//\//_S}
	lib=${lib/.\//}
	local checkstr=" ${lib%%/*} "
	if [[ ! ( "$dirpaths" == *$checkstr* ) ]]; then
		dirpaths="$dirpaths $checkstr " 
   	fi
	copiedpaths="$copiedpaths $liblink"
	ln -sf $lib $liblink
    done
    copiedpaths="$copiedpaths $dirpaths"
    \cd $HERE
cat << END > $outpath/local/bin/uninstall-bzl.sh
#!/bin/sh
machine=\`uname -m\`
if [[ "\$machine" == "armv7l" ]]; then 
  here=\`pwd\`
  cd /usr/lib/
  rm -rf $copiedpaths
  find ../local/bin/ -type f -not -name '*.sh' -exec rm -f \{\} \;
  cd \$here
fi
END
	
}


function configeth0
{
    local ip="192.168.5.1"
    if [ $# -gt 0 ]; then
	ip=$1 
    fi

    local x=`ifconfig enp0s31f6 | grep "inet addr:$ip" > /dev/null && echo "yes" || echo "no"`

    if [ "$x" == "yes" ]; then
	echo "Ethernet is already up at $ip ********" 
    else
	echo "Need $USER's login password..."
  	sudo ifconfig enp0s31f6 $ip up
	x=`ifconfig enp0s31f6 | grep "inet addr:$ip" > /dev/null && echo "yes" || echo "no"`
	if [ "$x" == "yes" ]; then
		echo "Ethernet IP is set to $ip!"
	else
		echo "Error: Unable to set IP:$ip to Ethernet."
		return 1
	fi
    fi
    return 0
}


function installtorpi()
{
    configeth0
    if [ $? ]; then
	local pip="192.168.5.5"
	if [ "$TARGET_RPI_IP" ]; then
		pip=$TARGET_RPI_IP
	fi
	local x=`ping -c 1 $pip > /dev/null && echo "yes"  || echo "no"`
	if [ "$x" == "yes" ]; then
		echo "Target IP:$pip is accessable!"
	else
		echo "Error: No any target is accessible @$pip!"
		return 1
	fi
	echo "Deploying the binaries..."
	bzl-deploy
	echo "Installing... : Need target root login password..."
	local T=
    	if [ "$TOP" ]; then
		T=$TOP
    	else
		T=$(gettop)
    	fi
	scp -r $T/deploy-bin/* root@$pip:/usr/
	if [ $? ]; then
		echo "Installation is compeleted successfully!"
	else
		echo "Error: installation is failed!"
	fi 
    fi
    return 0
}


function bzl()
{
    local HERE=`PWD= /bin/pwd -P`
    local T=
    if [ "$TOP" ]; then
	T=$TOP
    else
	T=$(gettop)
    fi

    if [ "$T" ]; then
	\cd $T
        bazel $@
	\cd $HERE
    else
        echo "Couldn't locate the top of the tree.  Try setting TOP."
    fi  
}
alias m='bzl build ...'
alias mp='bzl build --config=pi ...'

function findbuildfile()
{
    TOPFILE="WORKSPACE"
    local HERE=$PWD
    T=
    while [ \( ! \( -f $TOPFILE \) \) -a \( $PWD != "/" \) ]; do
        T=`PWD= /bin/pwd`
        if [ -f "$T/BUILD" ]; then
            echo $T/BUILD
            \cd $HERE
            return
        fi
        \cd ..
    done
    \cd $HERE
}

function tobzlpath()
{
    local T=
    if [ "$TOP" ]; then
	T=$TOP
    else
	T=$(gettop)
    fi
    if [ \( $# -gt 0 \) -a \( -d $1 \) ]; then
	local absdir=`readlink -f $1`
	if [ "$absdir" ]; then
	    if [ "$absdir" = "$T" ]; then
            	echo "//" 
	    else
	    	echo ${absdir/$T/\/}
	    fi
	fi
    fi
}

function mm()
{
    local T=
    if [ "$TOP" ]; then
	T=$TOP
    else
	T=$(gettop)
    fi
    local curdir=`PWD= /bin/pwd`
    local targetpath=$(tobzlpath $curdir)/...
    if [ "$T" = "$curdir" ]; then
	targetpath="..."
    else
	targetpath=$(tobzlpath $curdir)/...
    fi

    if [ \( $# -gt 0 \) -a \( "$1" = "--config=pi" \) ]; then
	bzl build --config=pi $targetpath
    else
	bzl build $targetpath
    fi
}
alias mmp='mm --config=pi'

function mmm()
{
    local T=
    if [ "$TOP" ]; then
	T=$TOP
    else
	T=$(gettop)
    fi
    if [ "$T" ]; then
        local TARGETS=
        local ARGS=
        local DASH_ARGS=$(echo "$@" | awk -v RS=" " -v ORS=" " '/^-.*$/')
        local DIRS=$(echo "$@" | awk -v RS=" " -v ORS=" " '/^[^-].*$/')
        for DIR in $DIRS ; do
            MODULE=`echo $DIR | sed -n -e 's/.*:\(.*$\)/\1/p' | sed 's/,/ /'`
            if [ "$MODULES" = "" ]; then
                MODULE="/..."
	    else
                MODULE=":$MODULE"
            fi
            DIR=`echo $DIR | sed -e 's/:.*//' -e 's:/$::'`
            if [ -f $DIR/BUILD ]; then
		DIR=$(tobzlpath $DIR)
		TARGETS="$TARGETS $DIR$MODULE"
            else
                case $DIR in
                  showcommands | snod | dist | incrementaljavac) ARGS="$ARGS $DIR";;
                  GET-INSTALL-PATH) GET_INSTALL_PATH=$DIR;;
                  *) echo "No BUILD file in $DIR."; return 1;;
                esac
            fi
        done
        bzl build $DASH_ARGS $targetpath
    else
        echo "Couldn't locate the top of the tree.  Try setting TOP."
    fi
}
alias mmmp='mmm --config=pi'

function croot()
{
    T=$(gettop)
    if [ "$T" ]; then
        \cd $(gettop)
    else
        echo "Couldn't locate the top of the tree.  Try setting TOP."
    fi
}

function cproj()
{
    TOPFILE="WORKSPACE"
    local HERE=`PWD= /bin/pwd`
    T=
    while [ \( ! \( -f $TOPFILE \) \) -a \( $PWD != "/" \) ]; do
        T=$PWD
        if [ -f "$T/BUILD" ]; then
            \cd $T
            return
        fi
        \cd ..
    done
    \cd $HERE
    echo "can't find Android.mk"
}

# simplified version of ps; output in the form
# <pid> <procname>
function qpid() {
    local prepend=''
    local append=''
    if [ "$1" = "--exact" ]; then
        prepend=' '
        append='$'
        shift
    elif [ "$1" = "--help" -o "$1" = "-h" ]; then
		echo "usage: qpid [[--exact] <process name|pid>"
		return 255
    fi

    local EXE="$1"
    if [ "$EXE" ] ; then
		qpid | \grep "$prepend$EXE$append"
    else
			ps -e \
			| tr -d '\r' \
			| sed -e 1d -e 's/^[^ ]* *\([0-9]*\).* \([^ ]*\)$/\1 \2/'
    fi
}

function pid()
{
    local prepend=''
    local append=''
    if [ "$1" = "--exact" ]; then
        prepend=' '
        append='$'
        shift
    fi
    local EXE="$1"
    if [ "$EXE" ] ; then
        local PID=`ps -e \
            | tr -d '\r' \
            | \grep "$prepend$EXE$append" \
            | sed -e 's/^[^ ]* *\([0-9]*\).*$/\1/'`
        echo "$PID"
    else
        echo "usage: pid [--exact] <process name>"
		return 255
    fi
}


# Read the ELF header from /proc/$PID/exe to determine if the process is
# 64-bit.
function is64bit()
{
    local PID="$1"
    if [ "$PID" ] ; then
        if [[ "$(cat /proc/$PID/exe | xxd -l 1 -s 4 -ps)" -eq "02" ]] ; then
            echo "64"
        else
            echo ""
        fi
    else
        echo ""
    fi
}


function ggrep()
{
    find . -name .repo -prune -o -name .git -prune -o -name out -prune -o -type f -name "*\.gradle" -print0 | xargs -0 grep --color -n "$@"
}

function jgrep()
{
    find . -name .repo -prune -o -name .git -prune -o -name out -prune -o -type f -name "*\.java" -print0 | xargs -0 grep --color -n "$@"
}

function cgrep()
{
    find . -name .repo -prune -o -name .git -prune -o -name out -prune -o -type f \( -name '*.c' -o -name '*.cc' -o -name '*.cpp' -o -name '*.h'  -o -name '*.hpp' \) -print0 | xargs -0 grep --color -n "$@"
}

function resgrep()
{
    for dir in `find . -name .repo -prune -o -name .git -prune -o -name out -prune -o -name res -type d`; do find $dir -type f -name '*\.xml' -print0 | xargs -0 grep --color -n "$@"; done;
}

function mangrep()
{
    find . -name .repo -prune -o -name .git -prune -o -path ./out -prune -o -type f -name 'AndroidManifest.xml' -print0 | xargs -0 grep --color -n "$@"
}

function sepgrep()
{
    find . -name .repo -prune -o -name .git -prune -o -path ./out -prune -o -name sepolicy -type d -print0 | xargs -0 grep --color -n -r --exclude-dir=\.git "$@"
}

case `uname -s` in
    Darwin)
        function mgrep()
        {
            find -E . -name .repo -prune -o -name .git -prune -o -path ./out -prune -o -type f -iregex '.*/(Makefile|Makefile\..*|.*\.make|.*\.mak|.*\.mk)' -print0 | xargs -0 grep --color -n "$@"
        }

        function bgrep()
        {
            find -E . -name .repo -prune -o -name .git -prune -o -path ./out -prune -o -type f -iregex '.*/(BUILD|BUILD.bazel)' -print0 | xargs -0 grep --color -n "$@"
        }

        function treegrep()
        {
            find -E . -name .repo -prune -o -name .git -prune -o -type f -iregex '.*\.(c|h|hpp|cpp|S|java|xml)' -print0 | xargs -0 grep --color -n -i "$@"
        }

        ;;
    *)
        function mgrep()
        {
            find . -name .repo -prune -o -name .git -prune -o -path ./out -prune -o -regextype posix-egrep -iregex '(.*\/Makefile|.*\/Makefile\..*|.*\.make|.*\.mak|.*\.mk)' -type f -print0 | xargs -0 grep --color -n "$@"
        }

        function bgrep()
        {
            find . -name .repo -prune -o -name .git -prune -o -path ./out -prune -o -regextype posix-egrep -iregex '(.*\/BUILD|.*BUILD\.bazel)' -type f -print0 | xargs -0 grep --color -n "$@"
        }

        function treegrep()
        {
            find . -name .repo -prune -o -name .git -prune -o -regextype posix-egrep -iregex '.*\.(c|h|hpp|cpp|S|java|xml)' -type f -print0 | xargs -0 grep --color -n -i "$@"
        }

        ;;
esac

function godir () {
    if [[ -z "$1" ]]; then
        echo "Usage: godir <regex>"
        return
    fi
    T=$(gettop)
    if [[ ! -f $T/filelist ]]; then
        echo -n "Creating index..."
        (\cd $T; find . -wholename ./out -prune -o -wholename ./.repo -prune -o -type f > filelist)
        echo " Done"
        echo ""
    fi
    local lines
    lines=($(\grep "$1" $T/filelist | sed -e 's/\/[^/]*$//' | sort | uniq))
    if [[ ${#lines[@]} = 0 ]]; then
        echo "Not found"
        return
    fi
    local pathname
    local choice
    if [[ ${#lines[@]} > 1 ]]; then
        while [[ -z "$pathname" ]]; do
            local index=1
            local line
            for line in ${lines[@]}; do
                printf "%6s %s\n" "[$index]" $line
                index=$(($index + 1))
            done
            echo
            echo -n "Select one: "
            unset choice
            read choice
            if [[ $choice -gt ${#lines[@]} || $choice -lt 1 ]]; then
                echo "Invalid choice"
                continue
            fi
            pathname=${lines[$(($choice-1))]}
        done
    else
        pathname=${lines[0]}
    fi
    \cd $T/$pathname
}


# Print colored exit condition
function pez {
    "$@"
    local retval=$?
    if [ $retval -ne 0 ]
    then
        echo -e "\e[0;31mFAILURE\e[00m"
    else
        echo -e "\e[0;32mSUCCESS\e[00m"
    fi
    return $retval
}

function get_make_command()
{
  echo command make
}

function make()
{
    local start_time=$(date +"%s")
    $(get_make_command) "$@"
    local ret=$?
    local end_time=$(date +"%s")
    local tdiff=$(($end_time-$start_time))
    local hours=$(($tdiff / 3600 ))
    local mins=$((($tdiff % 3600) / 60))
    local secs=$(($tdiff % 60))
    echo
    if [ $ret -eq 0 ] ; then
        echo -n -e "#### make completed successfully "
    else
        echo -n -e "#### make failed to build some targets "
    fi
    if [ $hours -gt 0 ] ; then
        printf "(%02g:%02g:%02g (hh:mm:ss))" $hours $mins $secs
    elif [ $mins -gt 0 ] ; then
        printf "(%02g:%02g (mm:ss))" $mins $secs
    elif [ $secs -gt 0 ] ; then
        printf "(%s seconds)" $secs
    fi
    echo -e " ####"
    echo
    return $ret
}



if [ "x$SHELL" != "x/bin/bash" ]; then
    case `ps -o command -p $$` in
        *bash*)
            ;;
        *)
            echo "WARNING: Only bash is supported, use of other shell would lead to erroneous results"
            ;;
    esac
fi

export TOP=$(gettop)
#bzl-deploy

