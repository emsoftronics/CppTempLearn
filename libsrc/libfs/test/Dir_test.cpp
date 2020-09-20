

#include <sys-fs/Dir.h>
#include <sys-fs/File.h>
#include <gtest/gtest.h>
#include <cpputils-base/logging.h>

using namespace Sys::Fs;

static bool print(Dir &dir)
{
  LOG(INFO) << "DirectoryEntriesCount [" << dir.fullPath() << "] :" << dir.getEntries().size();
  for (FsObject f : dir.getEntries())
  {
    if (f.isEmpty() || !f.isExist())
      return false;

    if (f.isSymbolicLink())
    {
      LOG(INFO) << "[L]" << f.fullPath() << "->" << f.getAbsolutePath();
    }
    else
    {
      if (f.getType() == FsObject::Type::DIRECTORY)
      {
        LOG(INFO) << "[D]" << f.fullPath();
        Dir d(f.fullPath());
        if (!print(d))
          return false;
      }
      else
      {
        LOG(INFO) << "[F]" << f.fullPath();
      }
    }
  }
  return true;
}

TEST(SysFsDir, Dir_getEntries)
{
  Dir dp("/tmp/sysfsDirTest");
  ASSERT_TRUE(dp.create());
  File f(dp.fullPath() + "/sysfsFile.txt");
  f.openWrite();
  f << "Hello Jagdish";
  f.close();
  Dir dc(dp.fullPath() + "/childDir");
  ASSERT_TRUE(dc.create());
  ASSERT_TRUE(f.createSymLink(dc.fullPath() + "/myLinkfile.txt"));
  ASSERT_TRUE(dp.createSymLink(dc.fullPath() + "/maindir"));
  ASSERT_TRUE(f.createSymLink(dp.fullPath() + "/linktosysfsfile.txt"));
  ASSERT_TRUE(print(dp));
  ASSERT_TRUE(dp.remove());
}
