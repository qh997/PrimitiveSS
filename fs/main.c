#include "fs/fs.h"
#include "sys/proc.h"
#include "types.h"

static void fs_init()
{

}

void task_fs()
{
    fs_init();

    struct proc_msg msg;

    while (TRUE) {
        send_recv(RECV, ANY, &msg);

        switch (msg.type) {
            case DEV_OPEN:
                break;

            default:
                break;
        }
    }
}
