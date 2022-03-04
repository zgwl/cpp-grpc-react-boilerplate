#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

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

class GreeterCallData {
 public:
  GreeterCallData(Greeter::AsyncService* service, ServerCompletionQueue* cq,
                  std::unordered_map<std::string, int>* dict)
      : dict_(dict),
        service_(service),
        cq_(cq),
        responder_(&ctx_),
        status_(CREATE) {
    Proceed();
  }

  void Proceed() {
    if (status_ == CREATE) {
      status_ = PROCESS;
      service_->RequestSayHello(&ctx_, &request_, &responder_, cq_, cq_, this);
    } else if (status_ == PROCESS) {
      // Spawn a new GreeterCallData instance to serve new client.
      new GreeterCallData(service_, cq_, dict_);

      // Actual logic
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

      status_ = FINISH;
      responder_.Finish(reply_, Status::OK, this);
    } else {
      GPR_ASSERT(status_ == FINISH);
      // Once in the FINISH state, deallocate ourselves (GreeterCallData).
      delete this;
    }
  }

 private:
  std::unordered_map<std::string, int>* dict_;

  Greeter::AsyncService* service_;
  ServerCompletionQueue* cq_;
  ServerContext ctx_;

  HelloRequest request_;
  HelloReply reply_;
  ServerAsyncResponseWriter<HelloReply> responder_;

  enum CallStatus { CREATE, PROCESS, FINISH };
  CallStatus status_;
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
    new GreeterCallData(&service_, cq_.get(), &dict_);
    void* tag;  // uniquely identifies a request.
    bool ok;

    while (true) {
      GPR_ASSERT(cq_->Next(&tag, &ok));
      GPR_ASSERT(ok);
      static_cast<GreeterCallData*>(tag)->Proceed();
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
