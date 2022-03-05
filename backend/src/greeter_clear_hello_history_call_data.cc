#include "greeter_clear_hello_history_call_data.h"

#include <unordered_map>

#include "greeter_base_call_data.h"

using grpc::ServerCompletionQueue;
using helloworld::Greeter;

GreeterCallDataClearHelloHistory::GreeterCallDataClearHelloHistory(
    Greeter::AsyncService* service, ServerCompletionQueue* cq,
    std::unordered_map<std::string, int>* dict)
    : GreeterCallDataT(service, cq), dict_(dict) {
  Proceed();
}

void GreeterCallDataClearHelloHistory::WaitForRequest() {
  service_->RequestClearHelloHistory(&ctx_, &request_, &responder_, cq_, cq_,
                                     this);
}

void GreeterCallDataClearHelloHistory::AddNextToCompletionQueue() {
  // Spawn a new GreeterCallDataClearHelloHistory instance to serve new client.
  new GreeterCallDataClearHelloHistory(service_, cq_, dict_);
}

void GreeterCallDataClearHelloHistory::HandleRequest() { dict_->clear(); }