

#include <sys-fs/FsObject.h>
#include <gtest/gtest.h>
#include <cpputils-base/logging.h>
#include <cpputils-base/sysutil.h>

using namespace Sys::Fs;
using namespace cpputils::base::Unix;

bool getFileStat(const char *path, struct stat &sb)
{
  std::string s = "echo \"Hello Jagdish! It is just test file.\" > ";
  s += std::string(path);
  LOG(INFO) << path << " : Creating file...";
  system(s.c_str());

  if (stat(path, &sb) == -1)
  {
    LOG(ERROR) << path << " : " << strerror(errno);
    return false;
  }

  std::string ftype;

  switch (sb.st_mode & S_IFMT)
  {
  case S_IFBLK:
    ftype = "block device\n";
    break;
  case S_IFCHR:
    ftype = "character device\n";
    break;
  case S_IFDIR:
    ftype = "directory\n";
    break;
  case S_IFIFO:
    ftype = "FIFO/pipe\n";
    break;
  case S_IFLNK:
    ftype = "symlink\n";
    break;
  case S_IFREG:
    ftype = "regular file\n";
    break;
  case S_IFSOCK:
    ftype = "socket\n";
    break;
  default:
    ftype = "unknown?\n";
    break;
  }

  LOG(INFO) << path << " : File type:                " << ftype;
  LOG(INFO) << path << " : I-node number:            " << (long)sb.st_ino;

  LOG(INFO) << path << " : Mode:                     "
            << std::oct << (unsigned long)sb.st_mode;

  LOG(INFO) << path << " : Link count:               " << (long)sb.st_nlink;
  LOG(INFO) << path << " : Ownership:                UID=" << (long)sb.st_uid
            << "   GID=" << (long)sb.st_gid;

  LOG(INFO) << path << " : Preferred I/O block size: "
            << (long)sb.st_blksize << " bytes";
  LOG(INFO) << path << " : File size:                "
            << (long long)sb.st_size << " bytes";
  LOG(INFO) << path << " : Blocks allocated:         "
            << (long long)sb.st_blocks;

  LOG(INFO) << path << " : Last status change:       " << ctime(&sb.st_ctime);
  LOG(INFO) << path << " : Last file access:         " << ctime(&sb.st_atime);
  LOG(INFO) << path << " : Last file modification:   " << ctime(&sb.st_mtime);

  return true;
}

TEST(SysFsFsObject, getAbsolutePath)
{
  FsObject pObj = FsObject("/tmp/testdir/jp/../pp/");
  ASSERT_EQ(pObj.getAbsolutePath(), "/tmp/testdir/pp");
}

TEST(SysFsFsObject, getType_UID_GID_Inode_Mode_CATime_LATime_MATime_LCount_Size_BSize)
{
  struct stat sb;
  ASSERT_TRUE(getFileStat("/tmp/FsObjectTestStatFunctions.txt", sb));
  FsObject pObj = FsObject("/tmp/FsObjectTestStatFunctions.txt");
  ASSERT_TRUE(pObj.getType() == FsObject::Type::REGULAR_FILE);
  ASSERT_TRUE(pObj.getInode() == sb.st_ino);
  ASSERT_TRUE(pObj.getMode() == (sb.st_mode & (~S_IFMT)));
  ASSERT_TRUE(pObj.getLinkCount() == sb.st_nlink);
  ASSERT_TRUE(pObj.getUID() == sb.st_uid);
  ASSERT_TRUE(pObj.getGID() == sb.st_gid);
  ASSERT_TRUE(pObj.getBlockSize() == sb.st_blksize);
  ASSERT_TRUE(pObj.getSize() == sb.st_size);
  ASSERT_TRUE(pObj.getBlockCount() == sb.st_blocks);
  ASSERT_TRUE(pObj.getCreationTime() == sb.st_ctim.tv_sec);
  ASSERT_TRUE(pObj.getLastAccessTime() == sb.st_atim.tv_sec);
  ASSERT_TRUE(pObj.getLastUpdateTime() == sb.st_mtim.tv_sec);
}

TEST(SysFsFsObject, file_copy_move_remove)
{
  FsObject p("/tmp/FsObjectTest/test.txt");
  ASSERT_TRUE(!p.create());
  Shell s("echo \"Hello Jagdish\" > /tmp/FsObjectTest/test.txt");
  ASSERT_TRUE(s.execute());
  ASSERT_TRUE(p.isExist());
  ASSERT_TRUE(p.move("/tmp/FsObjectTest/dir2/test.txt"));
  ASSERT_TRUE(!p.isExist());
  p.setPath("/tmp/FsObjectTest/dir2/test.txt");
  ASSERT_TRUE(p.isExist());
  ASSERT_TRUE(p.copy("/tmp/FsObjectTest/anotherpath/jp.txt"));
  ASSERT_TRUE(p.copy("/tmp/FsObjectTest/jp2/"));
  ASSERT_TRUE(p.copy("/tmp/FsObjectTest/jp2"));
  p.setPath("/tmp/FsObjectTest/dir2/test.txt");
  ASSERT_TRUE(p.remove());
  ASSERT_TRUE(!p.isExist());
  p.setPath("/tmp/FsObjectTest");
  ASSERT_TRUE(p.remove());
  ASSERT_TRUE(!p.isExist());
}

TEST(SysFsFsObject, changeMode)
{
  FsObject p("/tmp/FsObjectTest.txt");
  ASSERT_TRUE(!p.create());
  Shell s("echo \"Hello Jagdish\" > /tmp/FsObjectTest.txt");
  ASSERT_TRUE(s.execute());
  ASSERT_TRUE(p.isExist());
  mode_t mode = p.getMode();
  LOG(ERROR) << std::oct << mode;
  mode_t newMode = FsObject::Mode::R_USR | FsObject::Mode::W_USR | FsObject::Mode::R_GRP | FsObject::Mode::R_OTH | FsObject::Mode::W_OTH; //0646;
  ASSERT_TRUE(mode != newMode);
  ASSERT_TRUE(p.changeMode(newMode));
  LOG(ERROR) << std::oct << p.getMode();
  ASSERT_TRUE(p.getMode() == newMode);
  ASSERT_TRUE(p.changeMode(mode));
  ASSERT_TRUE(p.getMode() == mode);
  //ASSERT_TRUE(p.setGroup("sambashare"));
  //ASSERT_TRUE(p.getGroup() == "sambashare");
  ASSERT_TRUE(p.remove());
}

TEST(SysFsFsObject, createSymLink_removeSymLink)
{
  FsObject p("/tmp/FsObjectTest.txt");
  ASSERT_TRUE(!p.create());
  Shell s("echo \"Hello Jagdish\" > /tmp/FsObjectTest.txt");
  ASSERT_TRUE(s.execute());
  ASSERT_TRUE(p.isExist());
  FsObject lp("/tmp/testlink.txt");
  ASSERT_TRUE(!lp.isExist());
  ASSERT_TRUE(p.createSymLink(lp.fullPath()));
  ASSERT_TRUE(lp.isSymbolicLink());
  ASSERT_TRUE(lp.removeSymLink());
  ASSERT_TRUE(p.remove());
}