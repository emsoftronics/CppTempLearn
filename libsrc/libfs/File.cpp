
#include <sys-fs/File.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <climits>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#include <cpputils-base/logging.h>

namespace Sys
{
    namespace Fs
    {
        File::File()
            : FsObject::FsObject()
        {
            m_type = FsObject::Type::REGULAR_FILE;
        }

        File::File(const File &fsFile)
            : FsObject::FsObject(fsFile)
        {
            m_type = FsObject::Type::REGULAR_FILE;
        }

        File::File(File &&fsFile)
            : FsObject::FsObject(fsFile)
        {
            m_type = FsObject::Type::REGULAR_FILE;
        }

        File::File(const std::string &fsPath)
            : FsObject::FsObject(fsPath)
        {
            m_type = FsObject::Type::REGULAR_FILE;
        }

        File::File(std::string &&fsPath)
            : FsObject::FsObject(fsPath)
        {
            m_type = FsObject::Type::REGULAR_FILE;
        }

        File::File(const char *fsPath)
            : FsObject::FsObject(fsPath)
        {
            m_type = FsObject::Type::REGULAR_FILE;
        }

        File::~File()
        {
            checkAndClose();
        }

        void File::setPath(const std::string &path)
        {
            checkAndClose();
            Path::setPath(path);
        }

        void File::setPath(std::string &&path)
        {
            checkAndClose();
            Path::setPath(path);
        }

        File &File::operator=(const File &Path)
        {
            Path::operator=(Path);
            return *this;
        }

        File &File::operator=(File &&Path)
        {
            Path::operator=(Path);
            return *this;
        }

        void File::openRead(bool asTextFile)
        {
            ios_base::openmode mode = ios_base::in;

            if (!asTextFile)
                mode |= ios_base::binary;

            openInit(mode);
        }

        void File::openWrite(bool asTextFile)
        {
            ios_base::openmode mode = ios_base::out;

            if (!asTextFile)
                mode |= ios_base::binary;

            openInit(mode);
        }

        void File::openAppend(bool asTextFile)
        {
            ios_base::openmode mode = ios_base::out | ios_base::app;

            if (!asTextFile)
                mode |= ios_base::binary;

            openInit(mode);
        }

        void File::openReadWrite(bool asTextFile)
        {
            ios_base::openmode mode = ios_base::in | ios_base::out;

            if (!asTextFile)
                mode |= ios_base::binary;

            openInit(mode);
        }

        void File::open(const char *filename,
                        ios_base::openmode mode)
        {
            setPath(std::string(filename));
            openInit(mode);
        }

        void File::open(const std::string &filename,
                        ios_base::openmode mode)
        {
            setPath(filename);
            openInit(mode);
        }

        bool File::move(const std::string &newPath)
        {
            if (!isExist())
                return false;

            checkAndClose();
            return FsObject::move(newPath);
        }

        bool File::remove()
        {
            if (!isExist())
                return false;

            checkAndClose();
            return FsObject::remove();
        }

        void File::openInit(ios_base::openmode mode)
        {
            try
            {
                if (isEmpty())
                {
                    LOG(ERROR) << "Path is invalid or empty : " << fullPath();
                    return;
                }

                if (is_open())
                    close();

                std::fstream::open(getAbsolutePath().c_str(), mode);
            }
            catch (const std::exception &e)
            {
                LOG(ERROR) << e.what();
            }
        }

        void File::checkAndClose()
        {
            try
            {
                if (is_open())
                    close();
            }
            catch (const std::exception &e)
            {
                LOG(ERROR) << e.what();
            }
        }

    } // namespace Fs

} // namespace Sys
