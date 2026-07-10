/* Simplified stand-in for gnulib's localename.h/localename.c.

   Real gnulib handles many platform-specific cases (Windows registry UI
   language, macOS CoreFoundation, per-thread locale objects, ...); this
   only implements the common, portable path: consult the environment
   variables POSIX/glibc setlocale() itself already consults (LC_ALL,
   the category-specific variable, LANG), falling back to querying
   setlocale() directly. That's sufficient for gettext's own use of these
   functions (deciding which .mo catalog to load), just not a full
   drop-in replacement of gnulib's localename module for other callers. */
#ifndef _LOCALENAME_H
#define _LOCALENAME_H

#include <locale.h>
#include <stdlib.h>

static inline const char *
gl_locale_name_posix (int category, const char *categoryname)
{
  const char *locale = setlocale (category, NULL);
  return locale != NULL ? locale : "C";
}

static inline const char *
gl_locale_name_environ (int category, const char *categoryname)
{
  const char *locale = getenv ("LC_ALL");
  if (locale == NULL || locale[0] == '\0')
    locale = getenv (categoryname);
  if (locale == NULL || locale[0] == '\0')
    locale = getenv ("LANG");
  return (locale != NULL && locale[0] != '\0') ? locale : NULL;
}

static inline const char *
gl_locale_name_thread_unsafe (int category, const char *categoryname)
{
  const char *locale = gl_locale_name_environ (category, categoryname);
  return locale != NULL ? locale : gl_locale_name_posix (category, categoryname);
}

static inline const char *
gl_locale_name_default (void)
{
  return "C";
}

#endif /* _LOCALENAME_H */
