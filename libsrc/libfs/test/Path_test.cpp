

#include <sys-fs/Path.h>
#include <gtest/gtest.h>

using namespace Sys::Fs;

TEST(SysFsPath, toNative)
{
  Path tpath = Path("../jp/pp/");
  ASSERT_EQ(tpath.toNative(), "../jp/pp/");
}

TEST(SysFsPath, isEmpty)
{
  Path tpath = Path("../jp/pp/");
  ASSERT_TRUE(!tpath.isEmpty());
  tpath.setPath("");
  ASSERT_TRUE(tpath.isEmpty());
}

TEST(SysFsPath, pointsToContent)
{
  Path tpath = Path("../jp/pp/");
  ASSERT_TRUE(tpath.pointsToContent());
  tpath.setPath("./pj/tp");
  ASSERT_TRUE(!tpath.pointsToContent());
}

TEST(SysFsPath, fullPath)
{
  Path tpath = Path("../jp/pp/");
  ASSERT_EQ(tpath.fullPath(), "../jp/pp");
  tpath.setPath("./kp../../pj/tp");
  ASSERT_EQ(tpath.fullPath(), "./kp../../pj/tp");
}

TEST(SysFsPath, fileName)
{
  Path tpath = Path("../jp/pp/");
  ASSERT_EQ(tpath.fileName(), "pp");
  tpath.setPath("./pj/tp");
  ASSERT_EQ(tpath.fileName(), "tp");
  tpath.setPath("./pj/file.txt");
  ASSERT_EQ(tpath.fileName(), "file.txt");
}

TEST(SysFsPath, baseName)
{
  Path tpath = Path("../jp/pp/");
  ASSERT_EQ(tpath.baseName(), "pp");
  tpath.setPath("./pj/tp");
  ASSERT_EQ(tpath.baseName(), "tp");
  tpath.setPath("./pj/file.txt");
  ASSERT_EQ(tpath.baseName(), "file");
}

TEST(SysFsPath, extension)
{
  Path tpath = Path("../jp/pp/");
  ASSERT_EQ(tpath.extension(), "");
  tpath.setPath("./pj/tp");
  ASSERT_EQ(tpath.extension(), "");
  tpath.setPath("./pj/file.txt");
  ASSERT_EQ(tpath.extension(), ".txt");
}

TEST(SysFsPath, directoryPath)
{
  Path tpath = Path("../jp/pp/");
  ASSERT_EQ(tpath.directoryPath(), "../jp/");
  tpath.setPath("./pj/tp");
  ASSERT_EQ(tpath.directoryPath(), "./pj/");
  tpath.setPath("./pj/file.txt");
  ASSERT_EQ(tpath.directoryPath(), "./pj/");
}

TEST(SysFsPath, driveLetter)
{
  Path tpath = Path("../jp/pp/");
  ASSERT_EQ(tpath.driveLetter(), "");
}

TEST(SysFsPath, isAbsolute)
{
  Path tpath = Path("../jp/pp/");
  ASSERT_TRUE(!tpath.isAbsolute());
  tpath.setPath("./pj/tp");
  ASSERT_TRUE(!tpath.isAbsolute());
  tpath.setPath("/kk/pj/tp");
  ASSERT_TRUE(tpath.isAbsolute());
}

TEST(SysFsPath, isRelative)
{
  Path tpath = Path("../jp/pp/");
  ASSERT_TRUE(tpath.isRelative());
  tpath.setPath("./pj/tp");
  ASSERT_TRUE(tpath.isRelative());
  tpath.setPath("/kk/pj/tp");
  ASSERT_TRUE(!tpath.isRelative());
}

TEST(SysFsPath, resolve)
{
  Path tpath = Path("../jp/pp/");
  Path npath = Path("rp");
  ASSERT_EQ(tpath.resolve(npath).fullPath(), "../jp/pp/rp");
  npath.setPath("./rp/kp/");
  ASSERT_EQ(tpath.resolve(npath).fullPath(), "../jp/pp/./rp/kp");
  npath.setPath("/jp/pp/file.txt");
  ASSERT_EQ(tpath.resolve(npath).fullPath(), "/jp/pp/file.txt");
  npath.setPath("../rp/kp/file.txt");
  ASSERT_EQ(tpath.resolve(npath).fullPath(), "../jp/pp/../rp/kp/file.txt");
  tpath.setPath("../rp/kp/file.txt");
  ASSERT_EQ(tpath.resolve(npath).fullPath(), "../rp/kp/file.txt/../rp/kp/file.txt");
}

TEST(SysFsPath, resolved)
{
  Path tpath = Path("../jp/pp/");
  ASSERT_EQ(tpath.resolved(), "../jp/pp");
  tpath.setPath("./jp/../../pp/./kp");
  ASSERT_EQ(tpath.resolved(), "../pp/kp");
  tpath.setPath("./pj/tp");
  ASSERT_EQ(tpath.resolved(), "pj/tp");
}