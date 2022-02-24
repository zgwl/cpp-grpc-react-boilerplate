load("@rules_cc//cc:defs.bzl", "cc_binary")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "greeter_server",
    srcs = ["greeter_server.cc"],
    defines = ["BAZEL_BUILD"],
    deps = [
        "//protos:helloworld_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
    ],
)
