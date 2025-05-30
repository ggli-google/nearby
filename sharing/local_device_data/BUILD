# Copyright 2024 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

licenses(["notice"])

cc_library(
    name = "local_device_data",
    srcs = [
        "nearby_share_local_device_data_manager.cc",
        "nearby_share_local_device_data_manager_impl.cc",
    ],
    hdrs = [
        "nearby_share_local_device_data_manager.h",
        "nearby_share_local_device_data_manager_impl.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//internal/base",
        "//internal/platform:types",
        "//internal/platform/implementation:account_manager",
        "//internal/platform/implementation:types",
        "//proto/identity/v1:resources_cc_proto",
        "//proto/identity/v1:rpcs_cc_proto",
        "//sharing/common",
        "//sharing/common:enum",
        "//sharing/internal/api:platform",
        "//sharing/internal/base:utf_utils",
        "//sharing/internal/impl/common:nearby_identity_grpc_client",
        "//sharing/internal/public:logging",
        "//sharing/internal/public:types",
        "//sharing/proto:share_cc_proto",
        "//util/hash:highway_fingerprint",
        "@com_google_absl//absl/algorithm",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/random",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "test_support",
    testonly = True,
    srcs = [
        "fake_nearby_share_local_device_data_manager.cc",
    ],
    hdrs = [
        "fake_nearby_share_local_device_data_manager.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":local_device_data",
        "//sharing/common:enum",
        "//sharing/internal/api:platform",
        "//sharing/internal/public:types",
        "//sharing/proto:share_cc_proto",
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
    name = "local_device_data_test",
    srcs = [
        "nearby_share_local_device_data_manager_impl_test.cc",
    ],
    deps = [
        ":local_device_data",
        "//internal/platform/implementation:account_manager",
        "//internal/platform/implementation/g3",  # fixdeps: keep
        "//internal/test",
        "//proto/identity/v1:resources_cc_proto",
        "//proto/identity/v1:rpcs_cc_proto",
        "//sharing/common",
        "//sharing/common:enum",
        "//sharing/internal/api:mock_sharing_platform",
        "//sharing/internal/public:logging",
        "//sharing/internal/test:nearby_test",
        "//sharing/proto:share_cc_proto",
        "//sharing/scheduling",
        "//sharing/scheduling:test_support",
        "@com_github_protobuf_matchers//protobuf-matchers",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
        "@com_google_absl//absl/types:optional",
        "@com_google_googletest//:gtest_main",
    ],
)
