#ifndef SRC_GREETER_CLEAR_HELLO_HISTORY_CALL_DATA
#define SRC_GREETER_CLEAR_HELLO_HISTORY_CALL_DATA

#include <grpcpp/grpcpp.h>

#include <unordered_map>

#include "greeter_base_call_data.h"

using google::protobuf::Empty;
using grpc::ServerCompletionQueue;
using helloworld::Greeter;

class GreeterCallDataClearHelloHistory : public GreeterCallDataT<Empty, Empty> {
 public:
  GreeterCallDataClearHelloHistory(Greeter::AsyncService* service,
                                   ServerCompletionQueue* cq,
                                   std::unordered_map<std::string, int>* dict);

  virtual ~GreeterCallDataClearHelloHistory() = default;

  virtual void WaitForRequest() override;

  virtual void AddNextToCompletionQueue() override;

  virtual void HandleRequest() override;

 private:
  std::unordered_map<std::string, int>* dict_;
};

#endif