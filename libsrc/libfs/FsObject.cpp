
#include <sys-fs/FsObject.h>
#include <string>
#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <cpputils-base/logging.h>
#include <fcntl.h>
#include <cpputils-base/sysutil.h>
#include <pwd.h>
#include <grp.h>

namespace Sys
{
    namespace Fs
    {
        FsObject::FsObject()
            : Path::Path(), m_realPath(""), m_type(FsObject::Type::UKNOWN)
        {
        }

        FsObject::FsObject(const FsObject &fsPath)
            : Path::Path(fsPath), m_realPath(""), m_type(FsObject::Type::UKNOWN)
        {
        }

        FsObject::FsObject(FsObject &&fsPath)
            : Path::Path(fsPath), m_realPath(""), m_type(FsObject::Type::UKNOWN)
        {
        }

        FsObject::FsObject(const std::string &fsPath)
            : Path::Path(fsPath), m_realPath(""), m_type(FsObject::Type::UKNOWN)
        {
        }

        FsObject::FsObject(std::string &&fsPath)
            : Path::Path(fsPath), m_realPath(""), m_type(FsObject::Type::UKNOWN)
        {
        }

        FsObject::FsObject(const char *fsPath)
            : Path::Path(fsPath), m_realPath(""), m_type(FsObject::Type::UKNOWN)
        {
        }

        FsObject::~FsObject()
        {
        }

        FsObject &FsObject::operator=(const FsObject &Path)
        {
            Path::operator=(Path);
            return *this;
        }

        FsObject &FsObject::operator=(FsObject &&Path)
        {
            Path::operator=(Path);
            return *this;
        }

        FsObject::Type FsObject::getType() const
        {
            struct stat sb;
            if (!getStat(sb))
            {
                return FsObject::Type::UKNOWN;
            }

            return Type(sb.st_mode & S_IFMT);
        }

        std::string FsObject::getTypeToStr() const
        {
            switch (getType())
            {
            case Type::UKNOWN:
                return std::string("UKNOWN");
            case Type::FIFO:
                return std::string("FIFO");
            case Type::CHAR_DEVICE:
                return std::string("CHAR_DEVICE");
            case Type::DIRECTORY:
                return std::string("DIRECTORY");
            case Type::BLOCK_DEVICE:
                return std::string("BLOCK_DEVICE");
            case Type::REGULAR_FILE:
                return std::string("REGULAR_FILE");
            case Type::SYMBOLIC_LINK:
                return std::string("SYMBOLIC_LINK");
            case Type::SOCKET:
                return std::string("SOCKET");
            default:
                return std::string("UKNOWN");
            }
        }

        long FsObject::getInode() const
        {
            struct stat sb;
            if (getStat(sb))
            {
                return (long)sb.st_ino;
            }
            return -1;
        }

        mode_t FsObject::getMode() const
        {
            struct stat sb;
            if (getStat(sb))
            {

                return (unsigned long)(sb.st_mode & (~S_IFMT));
            }
            return 0;
        }

        long FsObject::getLinkCount() const
        {
            struct stat sb;
            if (getStat(sb))
            {
                return (long)sb.st_nlink;
            }
            return -1;
        }

        long FsObject::getUID() const
        {
            struct stat sb;
            if (getStat(sb))
            {
                return (long)sb.st_uid;
            }
            return -1;
        }

        long FsObject::getGID() const
        {
            struct stat sb;
            if (getStat(sb))
            {
                return (long)sb.st_gid;
            }
            return -1;
        }

        std::string FsObject::getOwner() const
        {
            struct passwd *pw = getpwuid(getUID());
            if (!pw)
            {
                LOG(ERROR) << fullPath() << " : " << strerror(errno);
                return "";
            }
            std::string owner(pw->pw_name);
            return owner;
        }

        std::string FsObject::getGroup() const
        {
            struct group *gr = getgrgid(getGID());
            if (!gr)
            {
                LOG(ERROR) << fullPath() << " : " << strerror(errno);
                return "";
            }
            std::string group(gr->gr_name);
            return group;
        }

        long FsObject::getBlockSize() const
        {
            struct stat sb;
            if (getStat(sb))
            {
                return (long)sb.st_blksize;
            }
            return -1;
        }

        long long FsObject::getSize() const
        {
            struct stat sb;
            if (getStat(sb))
            {
                return (long)sb.st_size;
            }
            return -1;
        }

        long long FsObject::getBlockCount() const
        {
            struct stat sb;
            if (getStat(sb))
            {
                return (long)sb.st_blocks;
            }
            return -1;
        }

        time_t FsObject::getCreationTime() const
        {
            struct stat sb;
            if (getStat(sb))
            {
                return sb.st_ctim.tv_sec;
            }
            return -1;
        }

        time_t FsObject::getLastAccessTime() const
        {
            struct stat sb;
            if (getStat(sb))
            {
                return sb.st_atim.tv_sec;
            }
            return -1;
        }

        time_t FsObject::getLastUpdateTime() const
        {
            struct stat sb;
            if (getStat(sb))
            {
                return sb.st_mtim.tv_sec;
            }
            return -1;
        }

        bool FsObject::isSymbolicLink() const
        {
            struct stat sb;
            if (getStat(sb, true) && S_ISLNK(sb.st_mode))
            {
                return true;
            }
            return false;
        }

        bool FsObject::isExist() const
        {
            checkIfExists();
            return m_exists;
        }

        bool FsObject::create(mode_t mode)
        {
            makePath();
            if ((m_type != FsObject::Type::REGULAR_FILE) && (m_type != FsObject::Type::DIRECTORY))
            {
                LOG(WARNING) << fullPath() << ": No any sepecific object type is mentioned to create.";
                return false;
            }
            struct stat sb;
            if (!getStat(sb))
            {
                if (m_type == FsObject::Type::DIRECTORY)
                {
                    if (mkdir(fullPath().c_str(), mode) < 0)
                    {
                        LOG(ERROR) << m_fullPath << " : " << strerror(errno);
                    }
                    else
                    {
                        return true;
                    }
                }
                else if (m_type == FsObject::Type::REGULAR_FILE)
                {
                    int fd = open(fullPath().c_str(), O_CREAT | O_WRONLY, mode);
                    if (fd < 0)
                    {
                        LOG(ERROR) << m_fullPath << " : " << strerror(errno);
                    }
                    else
                    {
                        close(fd);
                        return true;
                    }
                }
            }
            else
            {
                if ((S_ISDIR(sb.st_mode) && (m_type == FsObject::Type::DIRECTORY)) || (S_ISREG(sb.st_mode) && (m_type == FsObject::Type::REGULAR_FILE)))
                {
                    return true;
                }
                else
                {
                    std::string newPath = directoryPath() + baseName() + "#" + extension();
                    setPath(newPath);
                    return create(mode);
                }
            }

            return false;
        }

        const std::string &FsObject::getAbsolutePath() const
        {
            try
            {
                makePath();

                char *tbuf = realpath(m_fullPath.c_str(), NULL);
                if (!tbuf)
                {
                    tbuf = realpath(m_directoryPath.c_str(), NULL);
                    if (!tbuf)
                    {
                        LOG(ERROR) << m_fullPath << " : " << strerror(errno);
                        m_absolute = false;
                    }
                    else
                    {
                        m_realPath = std::string(tbuf);
                        m_realPath += std::string("/") + m_filename;
                        free(tbuf);
                    }
                }
                else
                {
                    m_realPath = std::string(tbuf);
                    free(tbuf);
                }
            }
            catch (const std::exception &e)
            {
                LOG(ERROR) << e.what();
            }

            return m_realPath;
        }

        bool FsObject::move(const std::string &dstPath)
        {
            if (!isExist())
                return false;

            FsObject newpath(dstPath);

            if (newpath.isExist())
            {
                if ((newpath.getType() != DIRECTORY) && (getType() == DIRECTORY))
                {
                    LOG(ERROR) << m_fullPath << "; " << newpath.fullPath() << " : Source is directory, but destination is file.";
                    return false;
                }
            }
            else
            {
                FsObject tobj(directoryPath());
                mode_t mode = tobj.getMode() & 0777;
                newpath.create(mode);
                if ((getType() == DIRECTORY) || (newpath.toNative().back() == '/'))
                {
                    if (mkdir(newpath.getAbsolutePath().c_str(), mode) < 0)
                    {
                        LOG(ERROR) << newpath.fullPath() << " : " << strerror(errno);
                        return false;
                    }
                }
            }

            std::string cmd("mv ");
            cmd = cmd + getAbsolutePath() + " " + newpath.toNative();
            if (!execCommand(cmd))
                return false;

            m_exists = false;
            return true;
        }

        bool FsObject::copy(const std::string &dstPath)
        {
            if (!isExist())
                return false;

            FsObject newpath(dstPath);

            if (newpath.isExist())
            {
                if ((newpath.getType() != DIRECTORY) && (getType() == DIRECTORY))
                {
                    LOG(ERROR) << m_fullPath << "; " << newpath.fullPath() << " : Source is directory, but destination is file.";
                    return false;
                }
            }
            else
            {
                FsObject tobj(directoryPath());
                mode_t mode = tobj.getMode() & 0777;
                newpath.create(mode);
                if ((getType() == DIRECTORY) || (newpath.toNative().back() == '/'))
                {
                    if (mkdir(newpath.getAbsolutePath().c_str(), mode) < 0)
                    {
                        LOG(ERROR) << newpath.fullPath() << " : " << strerror(errno);
                        return false;
                    }
                }
            }

            std::string cmd("cp -rf ");
            cmd = cmd + getAbsolutePath() + " " + newpath.toNative();
            return execCommand(cmd);
        }

        bool FsObject::createSymLink(const std::string &dstPath) const
        {
            if (!isExist())
                return false;

            FsObject newpath(dstPath);

            if (newpath.isExist())
            {
                if ((newpath.getType() != DIRECTORY))
                {
                    LOG(ERROR) << m_fullPath << "; " << newpath.fullPath() << " : Source is directory, but destination is file.";
                    return false;
                }
            }
            else
            {
                newpath.create();
            }

            if (symlink(toNative().c_str(), newpath.getAbsolutePath().c_str()) < 0)
            {
                LOG(ERROR) << m_realPath << "; " << newpath.m_realPath << " : " << strerror(errno);
                return false;
            }

            return true;
        }

        bool FsObject::remove()
        {
            if (!isExist())
                return false;

            if (getType() == Type::DIRECTORY)
            {
                std::string cmd("rm -rf ");
                cmd = cmd + getAbsolutePath();
                if (!execCommand(cmd))
                    return false;
            }
            else
            {
                if (unlink(getAbsolutePath().c_str()) < 0)
                {
                    LOG(ERROR) << m_realPath << " : " << strerror(errno);
                    return false;
                }
            }

            m_exists = false;
            return true;
        }

        bool FsObject::removeSymLink()
        {
            if (!isExist())
                return false;

            if (isSymbolicLink())
            {
                std::string rpath = getAbsolutePath();
                if (unlink(toNative().c_str()) < 0)
                {
                    LOG(ERROR) << m_fullPath << " : " << strerror(errno);
                    return false;
                }
                setPath(rpath);
            }
            return true;
        }

        bool FsObject::setOwner(const std::string &owner) const
        {
            if (!isExist())
                return false;

            if (!owner.empty())
            {
                std::string cmd("chown ");
                cmd += owner + " " + getAbsolutePath();
                return execCommand(cmd);
            }
            return false;
        }

        bool FsObject::setGroup(const std::string &group) const
        {
            if (!isExist())
                return false;

            if (!group.empty())
            {
                std::string cmd("chgrp ");
                cmd += group + " " + getAbsolutePath();
                return execCommand(cmd);
            }
            return false;
        }

        bool FsObject::changeMode(mode_t mode) const
        {
            if (!isExist())
                return false;

            struct stat sb;
            if (!getStat(sb))
                return false;

            mode = (sb.st_mode & S_IFMT) | (mode & (~S_IFMT));
            if (chmod(getAbsolutePath().c_str(), mode) < 0)
            {
                LOG(ERROR) << m_realPath << " : " << strerror(errno);
                return false;
            }
            return true;
        }

        bool FsObject::makePath(const mode_t mode) const
        {
            if (isExist())
                return true;

            size_t len = directoryPath().size();

            if (len <= 0)
                return false;

            char *tmp = new char[len + 1];
            char *p = nullptr;
            struct stat sb;

            /* copy path */
            memcpy(tmp, m_directoryPath.c_str(), len);
            tmp[len] = '\0';

            /* remove trailing slash */
            if (tmp[len - 1] == '/')
            {
                tmp[len - 1] = '\0';
            }

            /* check if path exists and is a directory */
            if (stat(tmp, &sb) == 0)
            {
                if (S_ISDIR(sb.st_mode))
                {
                    delete[] tmp;
                    return true;
                }
            }

            /* recursive mkdir */
            for (p = tmp + 1; *p; p++)
            {
                if (*p == '/')
                {
                    *p = 0;
                    /* test path */
                    if (stat(tmp, &sb) != 0)
                    {
                        /* path does not exist - create directory */
                        if (mkdir(tmp, mode) < 0)
                        {
                            LOG(ERROR) << tmp << " : " << strerror(errno);
                            delete[] tmp;
                            return false;
                        }
                    }
                    else if (!S_ISDIR(sb.st_mode))
                    {
                        /* not a directory */
                        LOG(ERROR) << tmp << " : " << strerror(errno);
                        delete[] tmp;
                        return false;
                    }
                    *p = '/';
                }
            }
            /* test path */
            if (stat(tmp, &sb) != 0)
            {
                /* path does not exist - create directory */
                if (mkdir(tmp, mode) < 0)
                {
                    LOG(ERROR) << tmp << " : " << strerror(errno);
                    delete[] tmp;
                    return false;
                }
            }
            else if (!S_ISDIR(sb.st_mode))
            {
                /* not a directory */
                LOG(ERROR) << tmp << " : " << strerror(errno);
                delete[] tmp;
                return false;
            }
            delete[] tmp;
            return true;
        }

        bool FsObject::getStat(struct stat &sb, bool lState) const
        {
            if (isEmpty())
                return false;

            int ret = 0;
            if (lState)
            {
                ret = lstat(fullPath().c_str(), &sb);
            }
            else
            {
                ret = stat(fullPath().c_str(), &sb);
            }

            if (ret < 0)
            {
                if (errno != ENOENT)
                {
                    LOG(ERROR) << fullPath() << " : " << strerror(errno);
                }
                return false;
            }

            return true;
        }

        void FsObject::checkIfExists() const
        {
            if (m_exists || isEmpty())
                return;

            struct stat stats;
            if (getStat(stats))
            {
                if (((m_type == FsObject::Type::REGULAR_FILE) && !S_ISREG(stats.st_mode)) || ((m_type == FsObject::Type::DIRECTORY) && !S_ISDIR(stats.st_mode)))
                {
                    m_exists = false;
                }
                else
                {
                    m_exists = true;
                }
            }
        }

        bool FsObject::execCommand(const std::string &cmd, std::vector<std::string> *outlines) const
        {
            cpputils::base::Unix::Shell s(cmd);
            if (!s.execute())
            {
                LOG(ERROR) << cmd;
                LOG(ERROR) << s.getErrorMessage();
                return false;
            }
            else
            {
                if (outlines != nullptr)
                {
                    *outlines = s.getOutputLines();
                }
            }
            return true;
        }

    } // namespace Fs

} // namespace Sys
