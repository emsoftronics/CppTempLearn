#pragma once

#include <sys-fs/FsObject.h>
#include <sys-fs/File.h>
#include <vector>
#include <string>

namespace Sys
{
    namespace Fs
    {
        class Dir : public FsObject
        {
        public:
            /**
            *  @brief
            *    Constructor
            */
            Dir();

            /**
            *  @brief
            *    Copy constructor
            *
            *  @param[in] fsDir
            *    Directory path to copy
            */
            Dir(const Dir &fsDir);

            /**
            *  @brief
            *    Move constructor
            *
            *  @param[in] fsDir
            *    Directory path to move
            */
            Dir(Dir &&fsDir);

            /**
            *  @brief
            *    Constructor
            *
            *  @param[in] fsPath
            *    Directory path
            */
            Dir(const std::string &fsPath);

            /**
            *  @brief
            *    Constructor
            *
            *  @param[in] fsPath
            *    Directory path
            */
            Dir(std::string &&fsPath);

            /**
            *  @brief
            *    Constructor
            *
            *  @param[in] fsPath
            *    Directory path
            */
            Dir(const char *fsPath);

            /**
            *  @brief
            *    Destructor
            */
            virtual ~Dir();

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
            Dir &operator=(const Dir &fsPath);

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
            Dir &operator=(Dir &&fsPath);

            /**
            *  @brief
            *    Get this directory enties
            *
            *  @param[in] fsPath
            *    Right-hand value to move
            *
            *  @return
            *    vector array of file system objects
            */
            std::vector<FsObject> &getEntries();

        protected:
            std::vector<FsObject> m_entries; ///< Directory entries
            time_t m_lastUpdate;             ///< Last modification time
        };

    } // namespace Fs
} // namespace Sys