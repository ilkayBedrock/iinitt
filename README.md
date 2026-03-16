# iinitt
ilkay(s/own) init
a suckless init without runlevels, and with only one configuration file
!! you need eudev (https://github.com/ilkayBedrock/eudev-iinitt) and your own hardware modules, the init does not have any modprobing protocols/services, you can use the start_service() function and auto load your modules at boot. systemd-tools also in dependencies. set your hostname either, i did not coded a hostname changer. first generate a machine id before using this init. dbus requires one. ELogind (https://github.com/ilkayBedrock/elogind-iinitt) is also a need for TTYs and/or DE/WM sessions. !!

# installation
!!!! i did learnt how to write makefiles
make -j$(nproc) && (#) make install
--i dont know how to write Makefiles... manual install this piece of shi--
@ first compile the code:
$ gcc iinitt.c -o iinitt
@ then move the init executable to /sbin:
'# mv iinitt /sbin
!!@!! make sure that executable is not a dynamic executable!
@ make the executable r/w/e:
'# chmod 777 /sbin/iinitt
!!@!! then boot with the kernel parameters:
init=/sbin/iinitt

# installation of the scripts (reboot & poweroff)
!!!!! they now come with the Makefile.
!!@!! compile them, following the install guide
@ move them to /sbin:
'# mv reboot /sbin && mv poweroff /sbin
!!@!! if 'reboot' and 'poweroff' exists, move them with another name

# wiki
https://iinitt.nekoweb.org/
