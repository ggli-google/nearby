load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Rule repository, note that it's recommended to use a pinned commit to a released version of the rules
# http_archive(
#    name = "rules_foreign_cc",
#    strip_prefix = "rules_foreign_cc-0.6.0",
#    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/0.6.0.tar.gz",
#)

# load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

# This sets up some common toolchains for building targets. For more details, please see
# https://github.com/bazelbuild/rules_foreign_cc/tree/main/docs#rules_foreign_cc_dependencies
# rules_foreign_cc_dependencies()

_ALL_CONTENT = """\
filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
    visibility = ["//visibility:public"],
)
"""

http_archive(
  name = "bazel_skylib",  # 2023-05-31T19:24:07Z
  sha256 = "08c0386f45821ce246bbbf77503c973246ed6ee5c3463e41efc197fa9bc3a7f4",
  strip_prefix = "bazel-skylib-288731ef9f7f688932bd50e704a91a45ec185f9b",
  urls = ["https://github.com/bazelbuild/bazel-skylib/archive/288731ef9f7f688932bd50e704a91a45ec185f9b.zip"],
)

http_archive(
    name = "platforms",  # 2023-07-28T19:44:27Z
    sha256 = "40eb313613ff00a5c03eed20aba58890046f4d38dec7344f00bb9a8867853526",
    strip_prefix = "platforms-4ad40ef271da8176d4fc0194d2089b8a76e19d7b",
    urls = ["https://github.com/bazelbuild/platforms/archive/4ad40ef271da8176d4fc0194d2089b8a76e19d7b.zip"],
)

http_archive(
    name = "rules_cc",
    strip_prefix = "rules_cc-0.0.9",
    urls = ["https://github.com/bazelbuild/rules_cc/archive/refs/tags/0.0.9.tar.gz"],
)

http_archive(
    name = "com_google_absl",
    strip_prefix = "abseil-cpp-4038192a57cb75f7ee671f81a3378ff4c74c4f8e",
    urls = ["https://github.com/abseil/abseil-cpp/archive/4038192a57cb75f7ee671f81a3378ff4c74c4f8e.zip"],
)

http_archive(
    name = "com_google_protobuf",
    strip_prefix = "protobuf-3.21.12",
    urls = ["https://github.com/protocolbuffers/protobuf/archive/v3.21.12.tar.gz"],
)

# Load common dependencies.
load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")
protobuf_deps()
load("@rules_cc//cc:repositories.bzl", "rules_cc_dependencies", "rules_cc_toolchains")
rules_cc_dependencies()
rules_cc_toolchains()

http_archive(
    name = "com_google_ukey2",
    strip_prefix = "ukey2-master",
    urls = ["https://github.com/google/ukey2/archive/master.zip"],
)

http_archive(
    name = "aappleby_smhasher",
    strip_prefix = "smhasher-master",
    build_file_content = """
package(default_visibility = ["//visibility:public"])
cc_library(
    name = "libmurmur3",
    srcs = ["src/MurmurHash3.cpp"],
    hdrs = ["src/MurmurHash3.h"],
    copts = ["-Wno-implicit-fallthrough"],
    licenses = ["unencumbered"],  # MurmurHash is explicity public-domain
)""",
    urls = ["https://github.com/aappleby/smhasher/archive/master.zip"],
)

http_archive(
    name = "nlohmann_json",
    strip_prefix = "json-3.10.5",
    build_file_content = """
cc_library(
  name = "json",
  hdrs = glob([
    "include/nlohmann/**/*.hpp",
  ]),
  includes = ["include"],
  visibility = ["//visibility:public"],
  alwayslink = True,
)""",
    urls = [
        "https://github.com/nlohmann/json/archive/refs/tags/v3.10.5.tar.gz",
    ],
)

http_archive(
    name = "com_google_googletest",
    strip_prefix = "googletest-main",
    urls = ["https://github.com/google/googletest/archive/main.zip"],
)

# ----------------------------------------------
# Nisaba: Script processing library from Google:
# ----------------------------------------------
# We depend on some of core C++ libraries from Nisaba and use the fresh code
# from the HEAD. See
#   https://github.com/google-research/nisaba

nisaba_version = "main"

http_archive(
    name = "com_google_nisaba",
    url = "https://github.com/google-research/nisaba/archive/refs/heads/%s.zip" % nisaba_version,
    strip_prefix = "nisaba-%s" % nisaba_version,
)

load("@com_google_nisaba//bazel:workspace.bzl", "nisaba_public_repositories")

nisaba_public_repositories()
http_archive(
    name = "boringssl",
    sha256 = "5d299325d1db8b2f2db3d927c7bc1f9fcbd05a3f9b5c8239fa527c09bf97f995",  # Last updated 2022-10-19
    strip_prefix = "boringssl-0acfcff4be10514aacb98eb8ab27bb60136d131b",
    urls = ["https://github.com/google/boringssl/archive/0acfcff4be10514aacb98eb8ab27bb60136d131b.tar.gz"],
)
# -------------------------------------------------------------------------
# Protocol buffer matches (should be part of gmock and gtest, but not yet):
#   https://github.com/inazarenko/protobuf-matchers

http_archive(
    name = "com_github_protobuf_matchers",
    urls = ["https://github.com/inazarenko/protobuf-matchers/archive/refs/heads/master.zip"],
    strip_prefix = "protobuf-matchers-master",
)
