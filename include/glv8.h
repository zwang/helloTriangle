#ifndef _GLV8_H
#define _GLV8_H

#include <V8/v8.h>

using namespace v8;

void bindGL(Isolate *isolate, Local<ObjectTemplate> &gl);

#endif
