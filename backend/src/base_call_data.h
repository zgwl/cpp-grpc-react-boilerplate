#ifndef SRC_BASE_CALL_DATA
#define SRC_BASE_CALL_DATA

class BaseCallData {
 public:
  BaseCallData() {}
  virtual ~BaseCallData() = default;
  virtual void Proceed() = 0;

 protected:
  virtual void WaitForRequest() = 0;
  virtual void AddNextToCompletionQueue() = 0;
  virtual void HandleRequest() = 0;
};

#endif