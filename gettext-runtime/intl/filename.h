/* Minimal stand-in for gnulib's filename.h. */
#ifndef _FILENAME_H
#define _FILENAME_H

#include <string.h> /* strchr */

#if defined _WIN32 || defined __CYGWIN__ || defined __EMX__ || defined __DJGPP__
# define ISSLASH(C) ((C) == '/' || (C) == '\\')
# define HAS_DEVICE(P) \
    ((((P)[0] >= 'A' && (P)[0] <= 'Z') || ((P)[0] >= 'a' && (P)[0] <= 'z')) \
     && (P)[1] == ':')
# define FILE_SYSTEM_PREFIX_LEN(P) (HAS_DEVICE (P) ? 2 : 0)
# define IS_ABSOLUTE_FILE_NAME(P) \
    (ISSLASH ((P)[0]) || (HAS_DEVICE (P) && ISSLASH ((P)[2])))
#else
# define ISSLASH(C) ((C) == '/')
# define FILE_SYSTEM_PREFIX_LEN(P) 0
# define IS_ABSOLUTE_FILE_NAME(P) ISSLASH ((P)[0])
#endif
#define IS_RELATIVE_FILE_NAME(P) (! IS_ABSOLUTE_FILE_NAME (P))
#define IS_FILE_NAME_WITH_DIR(P) \
  (strchr ((P), '/') != NULL || strchr ((P), '\\') != NULL \
   || FILE_SYSTEM_PREFIX_LEN (P) != 0)

#endif /* _FILENAME_H */
