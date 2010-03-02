#include <v8.h>
#include <node.h>
#include <node_events.h>
#include <stdlib.h>
#include <string.h>

using namespace v8;
using namespace node;


class Weirdy : public EventEmitter
{
public:
  static void Init(v8::Handle<Object> target) 
  {
    HandleScope scope;
    
    Local<FunctionTemplate> t = FunctionTemplate::New(New);
    
    t->Inherit(EventEmitter::constructor_template);
    t->InstanceTemplate()->SetInternalFieldCount(1);
    
    NODE_SET_PROTOTYPE_METHOD(t, "connect", Connect);
    
    target->Set(v8::String::NewSymbol("Weirdy"), t->GetFunction());
  }

protected:
  Weirdy() {}
  ~Weirdy() {}

  static Handle<Value> New(const Arguments& args) {
    HandleScope scope;

    Weirdy* w = new Weirdy();
    w->Wrap(args.This());
    return args.This();
  }

  static int eio_AfterConnect(eio_req *req) {
    printf("eio_AfterConnect; rc = %d\n", req->result);
    ev_unref(EV_DEFAULT_UC);
  }

  static int eio_Connect(eio_req *req) {
    // Note: this function is executed in the thread pool! CAREFUL

    printf("%d\n", sizeof(req->data));
    printf("%p\n", req->data);

    int *foo = (int*)(req->data);
    printf("** %p\n", foo);

    // this should print 420 but crashes instead
    //printf("** %d\n", *(int*)(req->data));

    req->result = 666;
    return 0;
  }

  static Handle<Value> Connect(const Arguments& args) {
    HandleScope scope;

    int *foo = (int *) malloc(sizeof(int));
    *foo = 420;

    printf("foo is %d\n", *foo);

    printf("&foo = %p", foo);

    eio_req *req = eio_custom(eio_Connect, EIO_PRI_DEFAULT, eio_AfterConnect, foo);

    printf("data after %p\n", req->data);
    ev_ref(EV_DEFAULT_UC);

    return Undefined();
  }

};

extern "C" void init (v8::Handle<Object> target)
{
  Weirdy::Init(target);
}
