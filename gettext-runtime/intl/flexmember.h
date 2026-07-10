/* Minimal stand-in for gnulib's flexmember.h. */
#ifndef _FLEXMEMBER_H
#define _FLEXMEMBER_H

#include <stddef.h> /* offsetof */

#ifndef FLEXIBLE_ARRAY_MEMBER
#define FLEXIBLE_ARRAY_MEMBER /* empty; C99 flexible array members are supported */
#endif

#define FLEXSIZEOF(type, member, arrayfld) \
  (offsetof (type, member) + sizeof ((type *) 0)->arrayfld)

#define FLEXNSIZEOF(type, member, n) \
  (offsetof (type, member) + (n) * sizeof (((type *) 0)->member[0]))

#endif /* _FLEXMEMBER_H */
