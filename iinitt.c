# Ilkay Alakay <ilkayxda@aol.com>. Licensed under GNU GPLv3<.0>
# requires Eudev!!! <https://github.com/ilkayBedrock/eudev-iinitt>
#include <stdio.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

# the main service starter function, do not touch anything on here
void start_service(char *path) {
    pid_t pid = fork();

    if (pid == 0) {
        execl(path, path, NULL);
        perror("exec failed");
        exit(1);
    }
}

# dbus requires special flags to run at the SYSROOT level
void start_dbus() {
    pid_t pid = fork();
    if (pid == 0) {
        execl("/usr/bin/dbus-daemon",
              "dbus-daemon",
              "--system",
              "--nofork",
              NULL);
        exit(1);
    }
}

# agetty (getty) starter - i do personally not use getty or any CON manager, just bash/fish/sh. the service lines are configured like this. delete the 71th line and replace it with: start_getty();
void start_getty() {
    pid_t pid = fork();

    if (pid == 0) {
        execl("/sbin/agetty",
              "agetty",
              "--noclear",
              "tty1",
              "115200",
              NULL);
        perror("agetty");
        exit(1);
    }
}

int main() {
    # showcase to iinitt (version etc...) and clearing old outputs
    system("clear"); # achieved by stdlib.h. please contact me if you do see/seen any errors
    printf("iinitt v0.0.1: ilkay STARTING...\n");
    # showcase and clear parts: end; filesystem mounting: if your service requires another partmount, add that here
       # partname # mountpoint # servname # folder permission (if needed)
    mount("proc", "/proc", "proc", 0, "");
    mount("sysfs", "/sys", "sysfs", 0, "");
    mount("devtmpfs", "/dev", "devtmpfs", 0, "");
    mount("tmpfs", "/run", "tmpfs", 0, "");
    mount("tmpfs", "/tmp", "tmpfs", 0, "");
    mkdir("/run", 0755);
    mkdir("/run/dbus", 0755);
    execl("/sbin/udevd", "udevd", "--daemon", NULL);
    execl("/sbin/udevadm", "udevadm", "trigger", "--action=add", NULL);
    printf("filesystems, eudev daemon and /run dir mounted/started successfully\n");
    # add your services to here
    start_dbus();
    sleep(1);
    start_service("/usr/sbin/NetworkManager");
    start_service("/bin/bash");

    while (1) {
        wait(NULL);
    }

    return 0;
}
