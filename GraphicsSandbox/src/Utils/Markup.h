#ifndef MARKUP_H
#define MARKUP_H

#define shared   static
#define persist  static
#define global   static
#define internal static

#include <stdint.h>

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float  real32;
typedef double real64;

#define STRINGIZE_INTERNAL(_) #_
#define STRINGIZE(_) STRINGIZE_INTERNAL(_)
#define PASTE(_, __) _##__
#define UNUSED(_) ((void)(_))

#if defined(_WIN32) || defined(_WIN64)
  #define WINDOWS
#elif defined (__APPLE__)
  #define APPLE
#elif defined(__linux__)
  #define LINUX
#else
  #define UNKNOWN_OS
#endif

#if defined(_M_IX86) || defined(__i386)
  #define X86_PROCESSOR
#elif defined(_M_X64) || defined(__x86_64__)
  #define X64_PROCESSOR
#elif defined(__PPC__) || defined(__ppc__) || defined(__powerpc__)
  #define PP32_PROCESSOR
#elif defined (__PPC64__) || defined(__ppc64__)
  #define PP64_PROCESSOR
#else
  #define UNKNOWN_PROCESSOR
#endif

#if defined(_MSC_VER) 
  #define MS_COMPILER
#elif (defined(__GNUC__) || defined(__GNUG__) && !defined(__clang__) && !defined(__INTEL_COMPILER))
  #define GNU_COMPILER
#elif defined(__clang__)
  #define CLANG_COMPILER
#else
  #define UNKNOWN_COMPILER
#endif

#if defined(MS_COMPILER)
  #define inline __forceinline
#else
  #define inline inline
#endif

#if defined(MS_COMPILER)
  #define NOTE(ATTRIBUTOR, MESSAGE) \
    __pragma(message(__FILE__ "(" STRINGIZE(__LINE__) ") : NOTE(" ATTRIBUTOR ") => [" MESSAGE "]")) 
#else
  #define NOTE(ATTRIBUTOR, MESSAGE)
#endif

#if defined(MS_COMPILER)
  #define TODO(ATTRIBUTOR, MESSAGE) \
    __pragma(message(__FILE__ "(" STRINGIZE(__LINE__) ") : TODO(" ATTRIBUTOR ") => [" MESSAGE "]")) 
#else
  #define TODO(ATTRIBUTOR, MESSAGE)
#endif


#endif
