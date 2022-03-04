#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "greeter_say_hello_call_data.h"
#include "protos/helloworld.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using helloworld::Greeter;

class ServerImpl final {
 public:
  ~ServerImpl() {
    server_->Shutdown();
    // Always shutdown the completion queue after the server.
    cq_->Shutdown();
  }

  void Run() {
    const std::string address = "0.0.0.0";
    const std::string port = "9090";
    const std::string server_address = address + ":" + port;

    ServerBuilder builder;
    // Note: Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);
    cq_ = builder.AddCompletionQueue();
    server_ = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;

    handleRpc();
  }

 private:
  void handleRpc() {
    new GreeterCallDataSayHello(&service_, cq_.get(), &dict_);
    void* tag;  // uniquely identifies a request.
    bool ok;

    while (true) {
      GPR_ASSERT(cq_->Next(&tag, &ok));
      GPR_ASSERT(ok);
      static_cast<BaseCallData*>(tag)->Proceed();
    }
  }

  std::unique_ptr<ServerCompletionQueue> cq_;
  Greeter::AsyncService service_;
  std::unique_ptr<Server> server_;

  std::unordered_map<std::string, int> dict_;
};

int main(int argc, char** argv) {
  ServerImpl server;
  server.Run();

  return 0;
}
