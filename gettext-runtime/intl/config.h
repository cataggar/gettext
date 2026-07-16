/* Hand-written stand-in for gettext-runtime/intl's autoconf-generated
   config.h. Starts minimal; see build.zig for target
   (Linux/Windows/macOS). */

#define PACKAGE "gettext-runtime"
#define VERSION "0.26"
#define LOCALEDIR "/usr/share/locale"
#define LOCALE_ALIAS_PATH "/usr/share/locale"

#define ENABLE_NLS 1
#define HAVE_ICONV 0
#ifndef __APPLE__
#define HAVE_MEMPCPY 1
#endif
#define HAVE_UNISTD_H 1
#define HAVE_GETCWD 1
/* mingw's CRT doesn't have stpcpy (a GNU/glibc extension); dcigettext.c
   provides its own static fallback when this isn't set. */
#ifndef _WIN32
#define HAVE_STPCPY 1
#endif

#define FLEXIBLE_ARRAY_MEMBER /* empty; C99 flexible array members supported */
#define HAVE_STDINT_H_WITH_UINTMAX 1
#define HAVE_INTTYPES_H 1

/* localcharset.c (vendored from libiconv): #if (not #ifdef) checked. */
#ifdef _WIN32
#define HAVE_LANGINFO_CODESET 0
#else
#define HAVE_LANGINFO_CODESET 1
#endif

/* Signals "standalone libintl build" (as opposed to _LIBC, i.e. building
   as part of glibc itself) on every target, not just Windows -- e.g. it
   controls the PLURAL_PARSE symbol renaming in plural-exp.h that
   plural.c's bison-generated code needs to match, and gates a
   Linux-only extern decl of plural_eval() in plural-exp.h that would
   otherwise conflict with eval-plural.h's own (static) definition. */
#define IN_LIBINTL 1

#ifdef _WIN32
#define WOE32 1
#define WOE32DLL 0
/* dcigettext.c calls wgetcwd(NULL, 0) (a glibc/gnulib GNU extension);
   mingw's CRT equivalent is _wgetcwd, which supports the same
   auto-allocate-on-NULL-buffer behavior. */
#include <direct.h>
#define wgetcwd _wgetcwd
#endif
