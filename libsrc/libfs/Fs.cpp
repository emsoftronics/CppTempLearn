#include <sys-fs/Fs.h>
#include <unistd.h>
#include <cpputils-base/logging.h>
#include <cstring>

using namespace cpputils::base;

namespace Sys
{
    namespace Fs
    {

        bool Os::copy(const std::string &src, const std::string &dst)
        {
            FsObject f(src);
            return f.copy(dst);
        }
        bool Os::move(const std::string &src, const std::string &dst)
        {
            FsObject f(src);
            return f.move(dst);
        }
        bool Os::createSymLink(const std::string &target, const std::string &linkPath)
        {
            FsObject f(target);
            return f.createSymLink(linkPath);
        }

        bool Os::remove(const std::string &path)
        {
            FsObject f(path);
            return f.remove();
        }

        bool Os::removeSymLink(const std::string &path)
        {
            FsObject f(path);
            return f.removeSymLink();
        }

        bool Os::setCurrentWorkingDir(const std::string &path)
        {
            if (chdir(path.c_str()) < 0)
            {
                LOG(ERROR) << path << " : " << strerror(errno);
                return false;
            }
            return true;
        }

        std::string Os::getCurrentWorkingDir()
        {
            std::string ret("");
            char *path = getcwd(NULL, 0);
            if (!path)
            {
                LOG(ERROR) << " : " << strerror(errno);
            }
            else
            {
                ret = std::string(path);
                free(path);
            }
            return ret;
        }

        std::string Os::getUser()
        {
            std::string s(getenv("USER"));
            return s;
        }
        std::string Os::getHomeDir()
        {
            std::string s(getenv("HOME"));
            return s;
        }
    } // namespace Fs
} // namespace Sys