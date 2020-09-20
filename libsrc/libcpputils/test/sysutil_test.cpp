#include <gtest/gtest.h>
#include <cpputils-base/sysutil.h>

using namespace cpputils::base::Unix;

TEST(sysutil_Shell, isShellAvailable)
{
    Shell s;
    ASSERT_TRUE(s.isShellAvailable());
}

TEST(sysutil_Shell, execute_and_all_Other_members)
{
    Shell s;
    s.updateCommand("mkdir -p /tmp/myTestshell");
    ASSERT_EQ(s.getCommand(), "mkdir -p /tmp/myTestshell");
    ASSERT_EQ(s.getOutPath(), "");
    ASSERT_TRUE(s.execute());
    s.updateCommand("echo \"hi jagdish prajapati\" > /tmp/myTestshell/testHello.txt");
    ASSERT_EQ(s.getCommand(), "echo \"hi jagdish prajapati\"");
    ASSERT_EQ(s.getOutPath(), "/tmp/myTestshell/testHello.txt");
    ASSERT_TRUE(s.execute());
    ASSERT_EQ(s.getOutputLines()[0], "hi jagdish prajapati");
    ASSERT_TRUE(s.getLastStatus());
    s.setOutPath("/tmp/myTestshell/testHello2.txt");
    ASSERT_TRUE(s.execute());
    s.updateCommand("ls /tmp/myTestshell/");
    s.setOutPath("");
    ASSERT_TRUE(s.execute());
    ASSERT_EQ(s.getOutputLines()[0], "testHello2.txt");
    ASSERT_EQ(s.getOutputLines()[1], "testHello.txt");
    s.updateCommand("rm -rf /tmp/myTestshell");
    ASSERT_TRUE(s.execute());
}