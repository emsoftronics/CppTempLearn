#pragma once
#include <sys-fs/FsObject.h>
#include <fstream>

namespace Sys
{
    namespace Fs
    {
        /**
        *  @brief
        *    File operations
        *
        *    This class handles file operation by taking the file path as input.
        *
        *    Path uses a unified format for storing paths that can be used
        *    consistently on every platform, using only '/' as a separator.
        *    When a Path is constructed from a string, the path is translated into
        *    the unified format and can then be used in a cross-platform way throughout
        *    an application. All operations on Path also return paths in the same
        *    unified format. To obtain a path in native platform format, use toNative().
        *
        *    All operations are completely string-based and don't use any system information.
        *    Therefore, paths are treated purely syntactically and do not imply that
        *    for example a file does really exist.
        */
        class File : public FsObject, public std::fstream
        {
        public:
            /**
            *  @brief
            *    Constructor
            */
            File();

            /**
            *  @brief
            *    Copy constructor
            *
            *  @param[in] fsFile
            *    File path to copy
            */
            File(const File &fsFile);

            /**
            *  @brief
            *    Move constructor
            *
            *  @param[in] fsFile
            *    File path to move
            */
            File(File &&fsFile);

            /**
            *  @brief
            *    Constructor
            *
            *  @param[in] fsPath
            *    File path
            */
            File(const std::string &fsPath);

            /**
            *  @brief
            *    Constructor
            *
            *  @param[in] fsPath
            *    File path
            */
            File(std::string &&fsPath);

            /**
            *  @brief
            *    Constructor
            *
            *  @param[in] fsPath
            *    File path
            */
            File(const char *fsPath);

            /**
            *  @brief
            *    Destructor
            */
            virtual ~File();

            /**
            *  @brief
            *    Set new path
            *
            *  @param[in] newPath
            *    File path
            */
            void setPath(const std::string &newPath);

            /**
            *  @brief
            *    Set new path
            *
            *  @param[in] newPath
            *    File path
            */
            void setPath(std::string &&newPath);

            /**
            *  @brief
            *    Copy assignment operator
            *
            *  @param[in] fsPath
            *    Right-hand value to copy
            *
            *  @return
            *    Reference to this value
            */
            File &operator=(const File &fsPath);

            /**
            *  @brief
            *    Move assignment operator
            *
            *  @param[in] fsPath
            *    Right-hand value to move
            *
            *  @return
            *    Reference to this value
            */
            File &operator=(File &&fsPath);

            /**
            *  @brief
            *    Open file with read only mode.
            * 
            *  @param[in] asTextFile
            *    file as text file, if true, else as binary file
            *  
            *  @return
            *    None
            */
            void openRead(bool asTextFile = true);

            /**
            *  @brief
            *    Open file with write only mode.
            * 
            *  @param[in] asTextFile
            *    file as text file, if true, else as binary file
            *  
            *  @return
            *    None 
            */
            void openWrite(bool asTextFile = true);

            /**
            *  @brief
            *    Open file with append mode.
            * 
            *  @param[in] asTextFile
            *    file as text file, if true, else as binary file
            *  
            *  @return
            *    None
            */
            void openAppend(bool asTextFile = true);

            /**
            *  @brief
            *    Open file with read and write mode.
            * 
            *  @param[in] asTextFile
            *    file as text file, if true, else as binary file
            *  
            *  @return
            *    None
            */
            void openReadWrite(bool asTextFile = true);

            /**
            *  @brief
            *    Opens the file identified by argument filename, associating it with the stream object, so that input/output operations are performed on its content. Argument mode specifies the opening mode.
            *    If the stream is already associated with a file (i.e., it is already open), calling this function fails.
            *
            *  @param[in] filename
            *    String with the name of the file to open.
            *    Specifics about its format and validity depend on the library implementation and running environment.
            * 
            *  @param[in] mode
            *    Flags describing the requested input/output mode for the file.<br>
            *    This is an object of the bitmask member type <samp><a href="/ios_base::openmode">openmode</a></samp> that consists of a combination of the following member constants:<br>
            *    <table class="boxed">
            *    <tr><th>member constant</th><th>stands for</th><th>access</th></tr>
            *    <tr><td><samp>in</samp></td><td><b>in</b>put</td><td>File open for reading: the <i><a href="/fstream::rdbuf">internal stream buffer</a></i> supports input operations.</td></tr>
            *    <tr><td><samp>out</samp></td><td><b>out</b>put</td><td>File open for writing: the <i><a href="/fstream::rdbuf">internal stream buffer</a></i> supports output operations.</td></tr>
            *    <tr><td><samp>binary</samp></td><td><b>binary</b></td><td>Operations are performed in binary mode rather than text.</td></tr>
            *    <tr><td><samp>ate</samp></td><td><b>at e</b>nd</td><td>The <i>output position</i> starts at the end of the file.</td></tr>
            *    <tr><td><samp>app</samp></td><td><b>app</b>end</td><td>All output operations happen at the end of the file, appending to its existing contents.</td></tr>
            *    <tr><td><samp>trunc</samp></td><td><b>trunc</b>ate</td><td>Any contents that existed in the file before it is open are discarded.<br>
            *    </td></tr>
            *    </table>
            *    These flags can be combined with the bitwise OR operator (<code>|</code>).<br> 
            *
            *  @return
            *    None
            */
            void open(const char *filename,
                      ios_base::openmode mode = ios_base::in | ios_base::out);
            void open(const std::string &filename,
                      ios_base::openmode mode = ios_base::in | ios_base::out);

            /**
            *  @brief
            *    Move or rename the filesystem object.
            *
            *  @param[in] dstPath
            *    destination path for moving
            *
            *  @return
            *    None
            */
            bool move(const std::string &dstPath);

            /**
            *  @brief
            *    Delete the filesystem object.
            * 
            *  @return
            *    'true' if operation is successful, else 'false'
            */
            bool remove();

        protected:
            void openInit(ios_base::openmode mode);
            void checkAndClose();
        };
    } // namespace Fs
} // namespace Sys