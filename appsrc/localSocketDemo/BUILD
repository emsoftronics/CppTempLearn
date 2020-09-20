load("@rules_cc//cc:defs.bzl", "cc_binary")
load("//tools:sharedarg.bzl", "COMPILE_FLAG", "CXXSTD_FLAG", "COMMON_DEP")

cc_binary(
    name = "local-server",
    srcs = ["local-server.cpp"],
    copts = [
        "-Ilibsrc/libcutils",
        "-Ilibsrc/libcpputils",
        '-DLOG_TAG=\\"localSocketDemo\\"',
	] + CXXSTD_FLAG + COMPILE_FLAG,
    deps = [
        "//libsrc/libcutils:cutils",
        "//libsrc/libcpputils:cpputils",
    ] + COMMON_DEP,
    linkstatic = 0,
)


cc_binary(
    name = "local-client",
    srcs = ["local-client.cpp"],
    copts = [
        "-Ilibsrc/libcutils",
        "-Ilibsrc/libcpputils",
        '-DLOG_TAG=\\"localSocketDemo\\"',
	] + CXXSTD_FLAG + COMPILE_FLAG,
    deps = [
        "//libsrc/libcutils:cutils",
        "//libsrc/libcpputils:cpputils",
    ] + COMMON_DEP,
    linkstatic = 0,
)
