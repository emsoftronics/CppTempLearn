#pragma once

#include <sys-fs/Path.h>
#include <string>
#include <ctime>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>

namespace Sys
{
    namespace Fs
    {
        /**
        *  @brief
        *    To handle all file system objects.
        */
        class FsObject : public Path
        {
        public:
            /**
            *  @brief
            *    Enumeration for types of objects under Linux filesystem.
            */
            enum Type
            {
                UKNOWN = 0000000,        ///< Indicates undentified file
                FIFO = 0010000,          ///< Indicates FIFO file
                CHAR_DEVICE = 0020000,   ///< Indicates character device file
                DIRECTORY = 0040000,     ///< Indicates directory
                BLOCK_DEVICE = 0060000,  ///< Indicates block device file
                REGULAR_FILE = 0100000,  ///< Indicates regular file
                SYMBOLIC_LINK = 0120000, ///< Indicates symoblic link
                SOCKET = 0140000,        ///< Indicates Socket
            };

            enum Mode
            {
                S_UID = 04000,    ///< set-user-ID (set process effective user ID on execve(2))
                S_GID = 02000,    ///< set-group-ID (set process effective group ID on execve(2); mandatory locking, as described in fcntl(2); take a new
                                  ///< file's group from parent directory, as described in chown(2) and mkdir(2))
                B_STICKY = 01000, ///< sticky bit (restricted deletion flag, as described in unlink(2))
                R_USR = 00400,    ///< read by owner
                W_USR = 00200,    ///< write by owner
                X_USR = 00100,    ///< execute/search  by  owner  ("search"  applies  for directories, and means that entries within the directory can be
                                  ///< accessed)
                R_GRP = 00040,    ///< read by group
                W_GRP = 00020,    ///< write by group
                X_GRP = 00010,    ///< execute/search by group
                R_OTH = 00004,    ///< read by others
                W_OTH = 00002,    ///< write by others
                X_OTH = 00001,    ///< execute/search by others
            };

        public:
            /**
            *  @brief
            *    Constructor
            */
            FsObject();

            /**
            *  @brief
            *    Copy constructor
            *
            *  @param[in] path
            *    File system object path to copy
            */
            FsObject(const FsObject &path);

            /**
            *  @brief
            *    Move constructor
            *
            *  @param[in] path
            *    File system object path to move
            */
            FsObject(FsObject &&path);

            /**
            *  @brief
            *    Constructor
            *
            *  @param[in] path
            *    File system object path
            */
            FsObject(const std::string &path);

            /**
            *  @brief
            *    Constructor
            *
            *  @param[in] path
            *    File system object path
            */
            FsObject(std::string &&path);

            /**
            *  @brief
            *    Constructor
            *
            *  @param[in] path
            *    File system object path
            */
            FsObject(const char *path);

            /**
            *  @brief
            *    Destructor
            */
            virtual ~FsObject();

            /**
            *  @brief
            *    Copy assignment operator
            *
            *  @param[in] path
            *    Right-hand value to copy
            *
            *  @return
            *    Reference to this value
            */
            FsObject &operator=(const FsObject &path);

            /**
            *  @brief
            *    Move assignment operator
            *
            *  @param[in] path
            *    Right-hand value to move
            *
            *  @return
            *    Reference to this value
            */
            FsObject &operator=(FsObject &&path);

            /**
            *  @brief
            *    Get type of file system object.
            *
            *  @return
            *    Type of this fsobject.
            */
            Type getType() const;

            /**
            *  @brief
            *    Get string of type of file system object.
            *
            *  @return
            *    String Type of this fsobject.
            */
            std::string getTypeToStr() const;

            /**
            *  @brief
            *    Get inode of file system object.
            *
            *  @return
            *    inode of this fsobject.
            */
            long getInode() const;

            /**
            *  @brief
            *    Get permision mode of file system object.
            *
            *  @return
            *    Permision mode of this fsobject.
            */
            mode_t getMode() const;

            /**
            *  @brief
            *    Get number of soft/hard links to this file system object.
            *
            *  @return
            *    Number of soft/hard links to this fsobject.
            */
            long getLinkCount() const;

            /**
            *  @brief
            *    Get UID of file system object.
            *
            *  @return
            *    UID of this fsobject.
            */
            long getUID() const;

            /**
            *  @brief
            *    Get GID of file system object.
            *
            *  @return
            *    GID of this fsobject.
            */
            long getGID() const;

            /**
            *  @brief
            *    Get Owner name of file system object.
            *
            *  @return
            *    owner name of this fsobject.
            */
            std::string getOwner() const;

            /**
            *  @brief
            *    Get group name of file system object.
            *
            *  @return
            *    group name of this fsobject.
            */
            std::string getGroup() const;

            /**
            *  @brief
            *    Get memory block size of file system object.
            *
            *  @return
            *    block size of this fsobject in bytes.
            */
            long getBlockSize() const;

            /**
            *  @brief
            *    Get size of file system object.
            *
            *  @return
            *    Size of this fsobject in bytes.
            */
            long long getSize() const;

            /**
            *  @brief
            *    Get number of blocks allocated for file system object.
            *
            *  @return
            *    Number of blocks for this fsobject.
            */
            long long getBlockCount() const;

            /**
            *  @brief
            *    Get creation time of file system object.
            *
            *  @return
            *    creation time of this fsobject.
            */
            time_t getCreationTime() const;

            /**
            *  @brief
            *    Get last access time of file system object.
            *
            *  @return
            *    last access time of this fsobject.
            */
            time_t getLastAccessTime() const;

            /**
            *  @brief
            *    Get last modification time of file system object.
            *
            *  @return
            *    last modification time of this fsobject.
            */
            time_t getLastUpdateTime() const;

            /**
            *  @brief
            *    Check, if native path is symbolic link.
            *
            *  @return
            *    'true' if file system object is symbolic link, else 'false'
            */
            bool isSymbolicLink() const;

            /**
            *  @brief
            *    Check for existance of file system object.
            *
            *  @return
            *    'true' if file system object exists, else 'false'
            */
            bool isExist() const;

            /**
            *  @brief
            *    Create file system object.
            *
            *  @return
            *    'true' if file system object exists, else 'false'
            */
            bool create(mode_t mode = 0777);

            /**
            *  @brief
            *    Get absolute path
            *
            *  @return
            *    Absolute path in unified format, but without a trailing separator
            *
            *  @remarks
            *    Calling this function triggers a full analysis of the path (costly operation).
            */
            const std::string &getAbsolutePath() const;

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
            virtual bool move(const std::string &dstPath);

            /**
            *  @brief
            *    Copy the filesystem object.
            *
            *  @param[in] dstPath
            *    destination path for copying
            *
            *  @return
            *    'true' if operation is successful, else 'false'
            */
            bool copy(const std::string &dstPath);

            /**
            *  @brief
            *    Create symbolic link the filesystem object.
            *
            *  @param[in] dstPath
            *    destination path for symbolic link
            *
            *  @return
            *    'true' if operation is successful, else 'false'
            */
            bool createSymLink(const std::string &dstPath) const;

            /**
            *  @brief
            *    Delete the filesystem object.
            * 
            *  @return
            *    'true' if operation is successful, else 'false'
            */
            virtual bool remove();

            /**
            *  @brief
            *    Delete symbolic link of filesystem object.
            * 
            *  @return
            *    'true' if operation is successful, else 'false'
            */
            bool removeSymLink();

            /**
            *  @brief
            *    Set owner of the filesystem object.
            *
            *  @param[in] owner
            *    Owner/user name
            * 
            *  @return
            *    'true' if operation is successful, else 'false'
            */
            bool setOwner(const std::string &owner) const;

            /**
            *  @brief
            *    Set group of the filesystem object.
            *
            *  @param[in] group
            *    Group name
            * 
            *  @return
            *    'true' if operation is successful, else 'false'
            */
            bool setGroup(const std::string &group) const;

            /**
            *  @brief
            *    Change permission mode of the filesystem object.
            *
            *  @param[in] mode
            *    permission mode
            * 
            *  @return
            *    'true' if operation is successful, else 'false'
            */
            bool changeMode(mode_t mode) const;

        protected:
            /**
            *  @brief
            *    Make full path if path is not existing
            *
            *  @param[in] mode
            *    permission mode
            * 
            *  @return
            *    'true' if path is created or exist, else 'false'
            */
            bool makePath(const mode_t mode = 0777) const;

            bool getStat(struct stat &sb, bool lState = false) const;

            void checkIfExists() const;

            bool execCommand(const std::string &cmd, std::vector<std::string> *outlines = nullptr) const;

        protected:
            mutable std::string m_realPath; ///< Absolute path (without trailing separators)
            mutable Type m_type;            ///< Type of filesystem object.
        };

    } // namespace Fs

} // namespace Sys
