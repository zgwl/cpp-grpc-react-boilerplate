#include "greeter_say_hello_call_data.h"

#include <string>
#include <unordered_map>

#include "greeter_base_call_data.h"

using grpc::ServerCompletionQueue;
using helloworld::Greeter;

GreeterCallDataSayHello::GreeterCallDataSayHello(
    Greeter::AsyncService* service, ServerCompletionQueue* cq,
    std::unordered_map<std::string, int>* dict)
    : GreeterCallDataT(service, cq) {
  dict_ = dict;
  Proceed();
}

void GreeterCallDataSayHello::WaitForRequest() {
  service_->RequestSayHello(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void GreeterCallDataSayHello::AddNextToCompletionQueue() {
  // Spawn a new GreeterCallDataSayHello instance to serve new client.
  new GreeterCallDataSayHello(service_, cq_, dict_);
}

void GreeterCallDataSayHello::HandleRequest() {
  std::string prefix("Hello ");
  const auto& name = request_.name();
  if (dict_->find(name) == dict_->end()) {
    reply_.set_message(prefix + name);
    dict_->insert({name, 1});
  } else {
    const auto times = dict_->at(name);
    reply_.set_message(prefix + name + ", " + std::to_string(times) + " times");
    (*dict_)[name]++;
  }
}