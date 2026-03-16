// Ilkay Alakay <ilkayxda@aol.com>. Published under GNU GPLv3(.0)
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    if (geteuid() != 0) {
        fprintf(stderr, "iinitt: please run this program as root\n");
        return 1;
    }
    
    kill(1, SIGINT);
    return 0;
}
