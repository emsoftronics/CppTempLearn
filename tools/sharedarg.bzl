
COMPILE_FLAG = [  "-Wall", "-Wextra", "-g" ]

CXXSTD_FLAG = [ "-std=c++14" ]

MACRO_FLAG = [ "-DNDEBUG", "-D_FILE_OFFSET_BITS=64", 
                '-DLOG_FILE=\\"/tmp/debuglogs.log\\"',
]

COMMON_DEP = [ "@org_llvm_libcxx//:libcxx" ]





# Either use GTEST_DEP or use GTEST_LIBS, but not both at a time.
GTEST_LIBS = [
   #      "-lgtest_main", "-lgtest"
]
GTEST_DEP = [
   "@com_google_googletest//:gtest_main", "@com_google_googletest//:gtest",
]

