#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
//#include <error.h>

#define LOG_MAX_LEN 12

int
_vscnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
    int i;

    i = vsnprintf(buf, size, fmt, args);

    /*
     * The return value is the number of characters which would be written
     * into buf not including the trailing '\0'. If size is == 0 the
     * function returns 0.
     *
     * On error, the function also returns 0. This is to allow idiom such
     * as len += _vscnprintf(...)
     *
     * See: http://lwn.net/Articles/69419/
     */
    if (i <= 0) {
        return 0;
    }

    if (i < size) {
        return i;
    }

    return size - 1;
}

int
_scnprintf(char *buf, size_t size, const char *fmt, ...)
{
    va_list args;
    int i;

    va_start(args, fmt);
    i = _vscnprintf(buf, size, fmt, args);
    va_end(args);

    return i;
}


#define mc_snprintf(_s, _n, ...)        snprintf((char *)_s, _n, __VA_ARGS__)
#define mc_scnprintf(_s, _n, ...)       _scnprintf((char *)_s, _n, __VA_ARGS__)
#define mc_vscnprintf(_s, _n, _f, _a)   _vscnprintf((char*) _s, _n, _f, _a)

void
_log(const char *file, int line, int panic, const char *fmt, ...)
{
//    struct logger *l = &logger;
    int len, size; //errno_save;
    char buf[LOG_MAX_LEN], *timestr;
    va_list args;
    struct tm *local;
    time_t t;
    ssize_t n;

//    if (l->fd < 0) {
//        return;
//    }

//    errno_save = errno;
    len = 0;            /* length of output buffer */
    size = LOG_MAX_LEN; /* size of output buffer */

    t = time(NULL);
    local = localtime(&t);
    timestr = asctime(local);

    len += mc_scnprintf(buf + len, size - len, "[%.*s] %s:%d ",
                        strlen(timestr) - 1, timestr, file, line);

	// XXX: attention for the return value from snprintf
	//		it's not the char write into buf, 
	//		but the whole len of built string.
    //len += snprintf(buf + len, size - len, "[%.*s] %s:%d ",
    //                    strlen(timestr) - 1, timestr, file, line);

    va_start(args, fmt);
    len += mc_vscnprintf(buf + len, size - len, fmt, args);
    va_end(args);

    buf[len++] = '\n';

    n = write(2, buf, len);
    if (n < 0) {
       // l->nerror++;
    }

//	printf("%d\n", n);
//    errno = errno_save;

    if (panic) {
        abort();
    }
}

int
main(int argc, char **argv)
{
	_log(__FILE__, __LINE__, 0, "error");
	return 0;
}

