all: program1 program2 program3

program1: iinitt.c
	gcc iinitt.c -o iinitt
program2: reboot.c
	gcc reboot.c -o reboot
program3: poweroff.c
	gcc poweroff.c -o poweroff

install: all
	mkdir -p /sbin
	cp iinitt /sbin/
	cp reboot /sbin/
	cp poweroff /sbin/

clean:
	rm -rf /sbin/iinitt /sbin/reboot /sbin/poweroff
