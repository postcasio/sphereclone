#ifndef HELPERS_H_
#define HELPERS_H_

#include <v8.h>

#define V8_FUNC(func) v8::FunctionTemplate::New(func)->GetFunction()

#ifndef DEBUG
#define DEBUG 0
#else
#define DEBUG 1
#endif

#define debug(str) if (DEBUG) { printf(str); }

#endif