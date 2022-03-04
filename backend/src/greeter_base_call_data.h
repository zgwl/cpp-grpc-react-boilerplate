#ifndef SRC_GREETER_BASE_CALL_DATA
#define SRC_GREETER_BASE_CALL_DATA

#include <grpcpp/grpcpp.h>

#include "base_call_data.h"
#include "protos/helloworld.grpc.pb.h"

using grpc::ServerAsyncResponseWriter;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;
using helloworld::Greeter;

template <class RequestType, class ReplyType>
class GreeterCallDataT : public BaseCallData {
 public:
  GreeterCallDataT(Greeter::AsyncService* service, ServerCompletionQueue* cq)
      : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE){};

  virtual ~GreeterCallDataT() = default;

  virtual void Proceed() override {
    if (status_ == CREATE) {
      status_ = PROCESS;
      WaitForRequest();
    } else if (status_ == PROCESS) {
      AddNextToCompletionQueue();
      HandleRequest();
      status_ = FINISH;
      responder_.Finish(reply_, Status::OK, this);
    } else {
      GPR_ASSERT(status_ == FINISH);
      // Once in the FINISH state, deallocate ourselves.
      delete this;
    }
  }

 protected:
  Greeter::AsyncService* service_;
  ServerCompletionQueue* cq_;
  ServerContext ctx_;

  RequestType request_;
  ReplyType reply_;
  ServerAsyncResponseWriter<ReplyType> responder_;

  enum CallStatus { CREATE, PROCESS, FINISH };
  CallStatus status_;
};

#endif