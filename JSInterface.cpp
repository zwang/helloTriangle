#include "v8.h"
#include "JSInterface.h"

void JSInterface::ThrowV8Exception(v8::Isolate *isolate, const char *message) {
    isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, message).ToLocalChecked()));
}

void JSInterface::ParameterCheckFailed(v8::Isolate *isolate) {
    JSInterface::ThrowV8Exception(isolate, "wrong parameter type");
}


