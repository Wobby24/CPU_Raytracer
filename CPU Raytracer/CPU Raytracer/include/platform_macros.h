// platform_macros.h
#ifndef PLATFORM_MACROS_H
#define PLATFORM_MACROS_H

#if defined(__GNUC__) || defined(__clang__)
#define MAYBE_UNUSED [[maybe_unused]]
#else
#define MAYBE_UNUSED
#endif

#endif
