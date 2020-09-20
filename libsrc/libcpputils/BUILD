load("@rules_cc//cc:defs.bzl", "cc_library")
load("//tools:sharedarg.bzl", "MACRO_FLAG", "COMPILE_FLAG", "CXXSTD_FLAG", "COMMON_DEP")

cc_library(
    name = "cpputils",
    srcs = glob(["*.cpp", "*.h"]),
    hdrs = glob(["*/*.h"]),
    deps = COMMON_DEP,
    copts = ["-Ilibsrc/libcpputils",
	] + CXXSTD_FLAG + COMPILE_FLAG + MACRO_FLAG,
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
)


