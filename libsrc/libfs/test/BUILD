load("@rules_cc//cc:defs.bzl", "cc_test")
load("//tools:sharedarg.bzl", "MACRO_FLAG", "COMPILE_FLAG", "CXXSTD_FLAG", 
"COMMON_DEP", "GTEST_LIBS", "GTEST_DEP")

cc_test(
    name = "fs-test",
    srcs = glob(["*.cpp", "*.h"]),
    copts = [
        "-Ilibsrc/libfs",
        "-Ilibsrc/libcpputils",
	] + CXXSTD_FLAG + COMPILE_FLAG + MACRO_FLAG,
    deps = [
        "//libsrc/libfs:fs",
        "//libsrc/libcpputils:cpputils",
    ] + GTEST_DEP + COMMON_DEP,
    linkopts = GTEST_LIBS,
)

