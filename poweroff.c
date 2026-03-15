#include <signal.h>
#include <unistd.h>

int main() {
    kill(1, SIGTERM);
    return 0;
}
