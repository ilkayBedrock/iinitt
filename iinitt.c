// Ilkay Alakay <ilkay@astronix.org>. Licensed under GNU GPLv3<.0>
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
#include <string.h>

void start_service(char *path){
    pid_t pid = fork();
    if (pid < 0){
        perror("fork failed");
        return;
    }
    if (pid == 0) {
        setsid();
        execl(path, path, NULL);
        perror("exec failed");
        exit(1);
    }
}

void start_dbus(){
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return;
    }
    if (pid == 0){
        setsid();
        execl("/usr/bin/dbus-daemon", "dbus-daemon", "--system", "--nofork", NULL);
        perror("dbus-daemon failed");
        exit(1);
    }
}

void start_getty(){
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return;
    }
    if (pid == 0){
        setsid();
        execl("/sbin/agetty", "agetty", "--noreset", "--noclear", 
              "--issue-file=/etc/issue:/etc/issue.d:/run/issue.d:/usr/lib/issue.d - ${TERM}", NULL);
        perror("agetty failed");
        exit(1);
    }
}

void do_reboot(){
    write(1, "iinitt: rebooting...\n", strlen("iinitt: rebooting...\n"));
    sync();
    mount(NULL, "/", NULL, MS_REMOUNT | MS_RDONLY, NULL);
    sync();
    reboot(RB_AUTOBOOT);
}

void do_poweroff(){
    write(1, "iinitt: shutting down root...\n", strlen("iinitt: shutting down root...\n"));
    sync();
    mount(NULL, "/", NULL, MS_REMOUNT | MS_RDONLY, NULL);
    sync();
    reboot(RB_POWER_OFF);
}

void handle_signal(int sig){
    if (sig == SIGTERM){
        do_poweroff();
    }
    if (sig == SIGINT){
        do_reboot();
    }
}

int main(){
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);
    reboot(RB_ENABLE_CAD);

    printf("\033[2J\033[H");
    printf("iinitt v5.4.1: ilkay STARTING...\n");
    umask(022);

    if (mount(NULL, "/", NULL, MS_REMOUNT, NULL) < 0) {
        perror("Failed to remount rootfs RW");
    }
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

    pid_t udev_pid = fork();
    if (udev_pid == 0){
        execl("/sbin/udevd", "udevd", "--daemon", NULL);
        perror("udevd");
        exit(1);
    }
    sleep(1);
    if (fork() == 0){
        execl("/sbin/udevadm", "udevadm", "trigger", "--action=add", NULL);
        perror("udevadm trigger");
        exit(1);
    }
    wait(NULL);

    if (fork() == 0){
        execl("/sbin/udevadm", "udevadm", "settle", NULL);
        perror("udevadm settle");
        exit(1);
    }
    wait(NULL);

    printf("Filesystems, eudev daemon and /run dir mounted/started successfully\n");

    start_dbus();
    sleep(1);
    
    start_service("/usr/sbin/NetworkManager"); 
    sleep(1);
    start_service("/usr/libexec/elogind"); 
    sleep(1);
    start_service("/usr/bin/syslog-ng"); 
    start_service("/usr/bin/chronyd"); 
    start_service("/usr/bin/crond"); 
    start_service("/bin/bash");

    while (1){
        pid_t pid;
        int status;
        while ((pid = waitpid(-1, &status, WNOHANG)) > 0){
            printf("iinitt: process %d exited\n", pid);
        }
        sleep(1);
    }

    return 0;
}
