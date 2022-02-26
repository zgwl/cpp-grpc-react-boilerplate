Boilerplate for building applications with a React frontend and a C++ GRPC backend.

# Run App Locally

## Backend
1. Install [Bazel](https://bazel.build/install)

Bazel is for building the backend service, using Bazel is much easier than using Makefile or CMake. It’s recommended to use the Bazelisk to manage the appropriate version of Bazel.

2. Launch backend service locally

Go to the **backend** directory, and run the following command from the terminal:
```
bazel run :greeter_server
```
Make sure the backend service is running successfully.

## Frontend

1. Install [NVM](https://github.com/nvm-sh/nvm#installing-and-updating)

NVM manages the version for node.js. It's recommended to use the `--lts` version of the node.js.

2. Launch frontend app locally

Go to the **frontend** directory, then run the following command from the terminal to install all dependent packages and launch the frontend app:
```
npm i
npm run start
```
Make sure the frontend dev app is running successfully.

## Proxy

React frontend can't communicate with the GRPC backend server directly. Browsers have to connect to gRPC services via a special proxy called [Envoy](https://github.com/envoyproxy/envoy).