Need to install usbipd-win on host side if using WSL2 
For wsl2 environments (the one i'm using), internal webcam has to be re-connected after a reboot.
Binds are persistent however, you shouldn't need to reshare. I've left a bunch of notes for getting WSL2 Ubuntu instance to access usb/internal cameras.

###From windows command prompt (run as admin)###

###check to see if video is shared to usbipd clients### 
usbipd list 

###rebind if not shared anymore####
usbipd bind --force --busid=<BUSID>

###Attach to WSL2###
usbipd attach --wsl --busid=<BUSID>


###I combined like 4-5 other solutions across different forums since they were all outdated or didn't work in some way for my WSL2 instance###
#grab updated sources##
sudo apt update 
sudo apt upgrade
##install needed dependencies##

##TODO: finish this below section in case i need to rebuild another kernel
##rebuilt the WSL2 kernel to include these configurations###
[*]
[*]
[*]
[*]
