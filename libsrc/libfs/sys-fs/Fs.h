#pragma once
#include <sys-fs/Path.h>
#include <sys-fs/FsObject.h>
#include <sys-fs/Dir.h>
#include <sys-fs/File.h>
#include <cpputils-base/macros.h>

namespace Sys
{
    namespace Fs
    {
        class Os
        {
        private:
            DISALLOW_IMPLICIT_CONSTRUCTORS(Os);

        public:
            static bool copy(const std::string &src, const std::string &dst);
            static bool move(const std::string &src, const std::string &dst);
            static bool createSymLink(const std::string &target, const std::string &linkPath);
            static bool remove(const std::string &path);
            static bool removeSymLink(const std::string &path);
            static bool setCurrentWorkingDir(const std::string &path);
            static std::string getCurrentWorkingDir();
            static std::string getUser();
            static std::string getHomeDir();
            //static std::string getLocalIp(); // ifconfig wlp2s0 | grep "inet addr:" | cut -d':' -f2 | cut -d' ' -f1
        };

    } // namespace Fs
} // namespace Sys