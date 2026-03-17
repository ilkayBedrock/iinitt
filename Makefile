all: program1 program2 program3

program1: iinitt.c
	gcc iinitt.c -o iinitt
program2: reboot.c
	gcc reboot.c -o reboot
program3: poweroff.c
	gcc poweroff.c -o poweroff

install: all
	mkdir -p /sbin
	mv iinitt /sbin/
	mv reboot /sbin/
	mv poweroff /sbin/
	chmod 777 /sbin/{iinitt,reboot,poweroff}

clean:
	rm -rf /sbin/iinitt /sbin/reboot /sbin/poweroff
