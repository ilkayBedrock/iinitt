# iinitt
ilkay(s/own) init
a init without runlevels, and with only one configuration file
!! you need eudev (https://github.com/ilkayBedrock/eudev-iinitt) and your own hardware modules, the init does not have any modprobing protocols/services. systemd-tools also in dependencies. first generate a machine id before using this init. dbus requires one !!

# installation
i dont know to write Makefiles... manual install this piece of shi
@ first compile the code:
$ gcc iinitt.c -o iinitt
@ then move the init executable to /sbin:
'# mv iinitt /sbin
!!@!! make sure that executable is not a dynamic executable!
@ make the executable r/w/e:
'# chmod 777 /sbin/iinitt
!!@!! then boot with the kernel parameters:
init=/sbin/iinitt
