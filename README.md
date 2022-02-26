Boilerplate for building applications with a React frontend and a C++ gRPC backend.

## How To Run

### Backend
1. Install [Bazel](https://bazel.build/install)

Bazel is for building the backend service, using Bazel is much easier than using Makefile or CMake. It’s recommended to use the Bazelisk to manage the appropriate version of Bazel.

2. Launch backend service locally

Run the following command from the **backend** directory:
```
bazel run :greeter_server
```
Make sure the backend service is running successfully.

### Frontend

1. Install [NVM](https://github.com/nvm-sh/nvm#installing-and-updating)

NVM manages the version for node.js. It's recommended to use the `--lts` version of the node.js.

2. Launch frontend app locally

Run the following commands from the **frontend** directory to install all dependent packages and launch the frontend app:
```
npm i
npm run start
```
Make sure the frontend dev app is running successfully.

### Proxy

React frontend can't communicate with the gRPC backend server directly. Browsers have to connect to gRPC services via a special proxy called [Envoy](https://github.com/envoyproxy/envoy).

In order to enable the Envoy Proxy, a docker image is required to be running locally.

1. Install [Docker](https://docs.docker.com/get-docker/)

2. Build the docker image

Run the following command from the **frontend** directory.
```
docker build -t cpp-grpc-react .
```
This builds a Docker image with the name cpp-grpc-react. Now, we run the Docker:
```
docker run -d --name cpp-grpc-react -p 8080:8080 -p 9901:9901 cpp-grpc-react
```
Make sure the Docker image is running successfully.

## How To Develop

### Prerequisite

1. Install grpc-web code generator plugin

Following the [instruction](https://github.com/grpc/grpc-web#code-generator-plugin) to download and install `protoc-gen-grpc-web` and `protoc`.

