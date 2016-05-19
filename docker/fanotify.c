#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <sys/fanotify.h>

int main(int argc, char** argv) {
    int fan;
    char buf[4096];
    char fdpath[32];
    char path[PATH_MAX + 1];
    ssize_t buflen, linklen;
    struct fanotify_event_metadata *metadata;

    // Init fanotify structure
    fan = fanotify_init(FAN_CLASS_NOTIF, O_RDONLY);

    // Watch open/access events on root mountpoint
    fanotify_mark(
        fan,
        FAN_MARK_ADD | FAN_MARK_MOUNT,
        FAN_ACCESS | FAN_OPEN,
        -1, "/"
    );

    while(1) {
        buflen = read(fan, buf, sizeof(buf));
        metadata = (struct fanotify_event_metadata*)&buf;

        while(FAN_EVENT_OK(metadata, buflen)) {
            if (metadata->mask & FAN_Q_OVERFLOW) {
                printf("Queue overflow!\n");
                continue;
            }

            // Resolve path, using automatically opened fd
            sprintf(fdpath, "/proc/self/fd/%d", metadata->fd);
            linklen = readlink(fdpath, path, sizeof(path) - 1);
            path[linklen] = '\0';
            printf("%s\n", path);

            close(metadata->fd);
            metadata = FAN_EVENT_NEXT(metadata, buflen);
        }
    }
}

// https://blog.yadutaf.fr/2015/04/25/how-i-shrunk-a-docker-image-by-98-8-featuring-fanotify/
// gcc -g fanotify.c --static -o fanotify
