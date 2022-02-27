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

Make sure the `protoc` can compile the proto to the right Javascript code by running the following command from the **backend** directory:
```
protoc -I=. protos/helloworld.proto --js_out=import_style=commonjs,binary:../frontend/src --grpc-web_out=import_style=commonjs,mode=grpcwebtext:../frontend/src
```
Even the **protos** are in the **backend** directory, it's actually being used in both backend and frontend. The above command generates the corresponding Javascript file from the **protos** and save then to the **frontend/src/protos** directory.

2. Setup VS Code (Optional)

### Backend

Making changes in the backend is stright-foward. Editing the proto could update the service interface, and editing the cpp file could update the service business logic.

Once all changes are finished, using the `bazel run` command to launch the service locally.

To test the service manually, it's recommended to use the [grpcurl](https://github.com/fullstorydev/grpcurl). Follow the instruction to download the grpcurl, then interact with the local grpc server to verify it's working as expected.

E.g. running the following command will call the `helloworld.Greeter` endpoint and the server shall return the correct response:
```
grpcurl -d '{"name":"alex"}' -plaintext localhost:9090 helloworld.Greeter/SayHello
```

### Frontend

When the proto files get updated, it's required to re-generate the client Javascript files by running the `protoc` command above.