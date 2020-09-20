load("@rules_cc//cc:defs.bzl", "cc_test")
load("//tools:sharedarg.bzl", "MACRO_FLAG", "COMPILE_FLAG", "CXXSTD_FLAG", 
"COMMON_DEP", "GTEST_LIBS", "GTEST_DEP")

cc_test(
    name = "cutils-test",
    srcs = glob(["*.cpp", "*.h"]),
    copts = ["-Ilibsrc/libcutils",
	] + CXXSTD_FLAG + COMPILE_FLAG + MACRO_FLAG,
    deps = [
        "//libsrc/libcutils:cutils",
    ] + GTEST_DEP + COMMON_DEP,
    linkopts = GTEST_LIBS,
)

