// Ilkay Alakay <ilkayxda@aol.com>. Licensed under GNU GPLv3<.0>
// requires Eudev!!! <https://github.com/ilkayBedrock/eudev-iinitt>
#include <stdio.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/reboot.h>

// the main service starter function, do not touch anything on here
void start_service(char *path) {
    pid_t pid = fork();
    if (pid < 0) {
    perror("fork failed");
    exit(1);
    }
    if (pid == 0) {
        execl(path, path, NULL);
        perror("exec failed");
        exit(1);
    }
}

// dbus requires special flags to run at the SYSROOT level
void start_dbus() {
    pid_t pid = fork();
    if (pid < 0) {
    perror("fork failed");
    exit(1);
    }
    if (pid == 0) {
        execl("/usr/bin/dbus-daemon",
              "dbus-daemon",
              "--system",
              "--nofork",
              NULL);
        exit(1);
    }
}

// agetty (getty) starter - i do personally not use getty or any CON manager, just bash/fish/sh. the service lines are configured like this. delete the line which contains "start_service("/bin/bash");" and replace it with: start_getty();
void start_getty() {
    pid_t pid = fork();
    if (pid < 0) {
    perror("fork failed");
    exit(1);
    }
    if (pid == 0) {
        execl("/sbin/agetty",
              "agetty",
              "--noreset",
              "--noclear",
              "--issue-file=/etc/issue:/etc/issue.d:/run/issue.d:/usr/lib/issue.d - ${TERM}",
              NULL);
        perror("agetty");
        exit(1);
    }
}

void do_reboot() {
    write(1, "iinitt: rebooting...\n", 17);
    sync();
    reboot(RB_AUTOBOOT);
}

void do_poweroff() {
    write(1, "iinitt: shut downing root...\n", 17);
    sync();
    reboot(RB_POWER_OFF);
}

void handle_signal(int sig) {

    if (sig == SIGTERM) {
        do_poweroff();
    }

    if (sig == SIGINT) {
        do_reboot();
    }
}

int main() {
    signal(SIGPIPE, SIG_IGN);
    // showcase to iinitt (version etc...) and clearing old outputs
    printf("\033[2J\033[H");
    printf("iinitt v0.0.2: ilkay STARTING...\n");
    // showcase and clear parts: end; filesystem mounting: if your service requires another partmount, add that here
     //  # partname # mountpoint # servname # folder permission (if needed)
    mount("proc", "/proc", "proc", 0, "");
    mount("sysfs", "/sys", "sysfs", 0, "");
    mount("devtmpfs", "/dev", "devtmpfs", 0, "");
    mkdir("/run", 0755);
    mount("tmpfs", "/run", "tmpfs", 0, "");
    mkdir("/tmp", 1777);
    mount("tmpfs", "/tmp", "tmpfs", 0, "");
    mkdir("/run/dbus", 0755);
    mkdir("/dev/pts", 0755);
    mount("devpts", "/dev/pts", "devpts", 0, "");
    if (fork() == 0) {
    	execl("/sbin/udevd", "udevd", "--daemon", NULL);
    	perror("udevd");
    	exit(1);
    }
    sleep(1);
    if (fork() == 0) {
    	execl("/sbin/udevadm", "udevadm", "trigger", "--action=add", NULL);
    	perror("udevadm trigger");
    	exit(1);
    }
    if (fork() == 0) {
        execl("/sbin/udevadm", "udevadm", "settle", NULL);
        perror("udevadm settle");
        exit(1);
    }
    printf("filesystems, eudev daemon and /run dir mounted/started successfully\n");
    // add your services to here
    start_dbus();
    sleep(1);
    start_service("/usr/sbin/NetworkManager");
    start_service("/bin/bash");

    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);
    reboot(RB_ENABLE_CAD);

    while (1) {
    pid_t pid = wait(NULL);
    printf("process %d exited\n", pid);
    }

    return 0;
}
