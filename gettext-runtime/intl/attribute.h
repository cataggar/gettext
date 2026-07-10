/* Minimal stand-in for gnulib's attribute.h: nothing in the reduced file
   set this build.zig compiles actually uses ATTRIBUTE_* macros from it
   (bison already defines its own YY_ATTRIBUTE_* with fallbacks in the
   generated plural.c), so this is intentionally empty. */
#ifndef _GL_ATTRIBUTE_H
#define _GL_ATTRIBUTE_H

#ifndef FALLTHROUGH
#define FALLTHROUGH ((void) 0)
#endif

#endif
