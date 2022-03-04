#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "greeter_call_data.h"
#include "protos/helloworld.grpc.pb.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

class GreeterCallDataSayHello
    : public GreeterCallDataT<HelloRequest, HelloReply> {
 public:
  GreeterCallDataSayHello(Greeter::AsyncService* service,
                          ServerCompletionQueue* cq,
                          std::unordered_map<std::string, int>* dict)
      : GreeterCallDataT(service, cq), dict_(dict) {
    Proceed();
  };

  virtual void WaitForRequest() override {
    service_->RequestSayHello(&ctx_, &request_, &responder_, cq_, cq_, this);
  };

  virtual void AddNextToCompletionQueue() override {
    // Spawn a new GreeterCallDataSayHello instance to serve new client.
    new GreeterCallDataSayHello(service_, cq_, dict_);
  };

  virtual void HandleRequest() override {
    std::string prefix("Hello ");
    const auto& name = request_.name();
    if (dict_->find(name) == dict_->end()) {
      reply_.set_message(prefix + name);
      dict_->insert({name, 1});
    } else {
      const auto times = dict_->at(name);
      reply_.set_message(prefix + name + ", " + std::to_string(times) +
                         " times");
      (*dict_)[name]++;
    }
  };

 private:
  std::unordered_map<std::string, int>* dict_;
};

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
