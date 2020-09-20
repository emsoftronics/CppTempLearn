

#include <sys-fs/Fs.h>
#include <cpputils-base/logging.h>
#include <gtest/gtest.h>

using namespace Sys::Fs;
using namespace cpputils::base;

TEST(SysFsFile, copy_remove)
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
  ASSERT_TRUE(Os::createSymLink(f.fullPath(), dp.fullPath() + "/linktosysfsfile.txt"));
  ASSERT_TRUE(Os::removeSymLink(dc.fullPath() + "/maindir"));
  ASSERT_TRUE(Os::copy(dp.fullPath(), dp.directoryPath() + "jagdish/"));
  ASSERT_TRUE(dp.remove());
  ASSERT_TRUE(!dp.isExist());
  ASSERT_TRUE(Os::move(dp.directoryPath() + std::string("jagdish/") + dp.fileName(), dp.directoryPath()));
  ASSERT_TRUE(dp.isExist());
  ASSERT_TRUE(dp.remove());
  ASSERT_TRUE(Os::remove(dp.directoryPath() + std::string("jagdish")));
}

TEST(SysFsFile, move_remove)
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
  ASSERT_TRUE(Os::move(dp.fullPath(), dp.directoryPath() + "jagdish/"));
  ASSERT_TRUE(!dp.remove());
  ASSERT_TRUE(Os::remove(dp.directoryPath() + "jagdish"));
}
