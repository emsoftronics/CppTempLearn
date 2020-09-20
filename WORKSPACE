
workspace(name = "rpi_bazel")

BAZEL_VERSION = "1.2.0"
BAZEL_VERSION_SHA = "34eb178663a9f9c6765db72dd58958cdac64c5f08dd51e5b67e28d466079bd1e"

load("//tools/workspace:default.bzl", "add_default_repositories")

add_default_repositories()

local_repository(
    name = "com_google_googletest",
    path = "../googletest",
)

