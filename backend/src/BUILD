load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "service_call_data",
    srcs = [
        "greeter_say_hello_call_data.cc",
    ],
    hdrs = [
        "base_call_data.h",
        "greeter_base_call_data.h",
        "greeter_say_hello_call_data.h",
    ],
    deps = [
        "//protos:helloworld_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
    ],
)

cc_binary(
    name = "server",
    srcs = ["server.cc"],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":service_call_data",
        "//protos:helloworld_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc++_reflection",
    ],
)
