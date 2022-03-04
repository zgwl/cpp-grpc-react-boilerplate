#ifndef SRC_GREETER_SAY_HELLO_CALL_DATA
#define SRC_GREETER_SAY_HELLO_CALL_DATA

#include <grpcpp/grpcpp.h>

#include <unordered_map>

#include "greeter_base_call_data.h"

using grpc::ServerCompletionQueue;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

class GreeterCallDataSayHello
    : public GreeterCallDataT<HelloRequest, HelloReply> {
 public:
  GreeterCallDataSayHello(Greeter::AsyncService* service,
                          ServerCompletionQueue* cq,
                          std::unordered_map<std::string, int>* dict);

  virtual ~GreeterCallDataSayHello() = default;

  virtual void WaitForRequest() override;

  virtual void AddNextToCompletionQueue() override;

  virtual void HandleRequest() override;

 private:
  std::unordered_map<std::string, int>* dict_;
};

#endif