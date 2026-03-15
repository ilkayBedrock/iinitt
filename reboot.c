// Ilkay Alakay <ilkayxda@aol.com>. Published under GNU GPLv3(.0)
#include <signal.h>
#include <unistd.h>

int main() {
    kill(1, SIGINT);
    return 0;
}
