#if !defined __GNUC__
# define __attribute__(x) /*nothing*/
#endif

#ifndef offsetof
# define offsetof(type, member) \
     (((char *) &(((type *) NULL)->member)) - ((char *) (type *) NULL))
#endif

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

/* macros */
#ifndef MAX
# define MAX(a, b)      (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a, b)      (((a) < (b)) ? (a) : (b))
#endif
#define CLAMP(val, min, max) MIN(MAX(min, val), max)

#undef isdigit
#define isdigit(a) ((unsigned char)((a) - '0') <= 9)

