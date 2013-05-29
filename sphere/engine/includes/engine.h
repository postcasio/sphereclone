#ifndef ENGINE_H_
#define ENGINE_H_

#define ENGINE_VERSION "1.8"

#ifdef WIN32

#define ALIGNED _declspec(align(2))

#else

#define ALIGNED __attribute__((aligned(2)))

#endif

#endif