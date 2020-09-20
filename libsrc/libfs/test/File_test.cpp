

#include <sys-fs/File.h>
#include <gtest/gtest.h>

using namespace Sys::Fs;

TEST(SysFsFile, isExist_create_remove)
{
  File pFile = File("/tmp/pp/jp/test.txt");
  pFile.remove();
  ASSERT_TRUE(!pFile.isExist());
  ASSERT_TRUE(pFile.create());
  ASSERT_TRUE(pFile.isExist());
}

TEST(SysFsFile, openWrite_openRead)
{
  File pFile = File("/tmp/pp/mytest.txt"); //jp/test.txt
  pFile.openWrite();
  pFile << "Hello World!\n";
  pFile.close();
  pFile.openRead();
  char buffer[20];
  pFile.getline(buffer, sizeof(buffer));
  pFile.close();
  ASSERT_EQ(std::string(buffer), "Hello World!");
  ASSERT_EQ(pFile.fileName(), "mytest.txt");
  ASSERT_EQ(pFile.extension(), ".txt");
}
