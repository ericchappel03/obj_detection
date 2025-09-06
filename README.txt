Need to install usbipd-win on host side if using WSL2 
I used Cmake 3.5 to compile with OpenCV, consult the OpenCV documentation on how to install and use Cmake with OpenCV
For wsl2 environments (the one i'm using), internal webcam has to be re-connected after a reboot.
Binds are persistent however, you shouldn't need to reshare. I've left a bunch of notes for getting WSL2 Ubuntu instance to access usb/internal cameras.

###From windows command prompt (run as admin)###

###check to see if video is shared to usbipd clients### 
usbipd list 

###rebind if not shared anymore####
usbipd bind --force --busid=<BUSID>

###Attach to WSL2###
usbipd attach --wsl --busid=<BUSID>

##check to make sure it's attached##
usbipd list

##Check if you can see it in Ubuntu instance##
v4l2-ctl --list-devices 

##Verify it can access Camera properly on cheese and/or guvcview##
#guvcview worked for me but cheese didn't because of incorrect paths or missing drivers but was not an issue for OpenCV#
sudo apt-get install cheese guvcview

cheese #then just run 
guvcview #either one of these to check and you should see your camera feed

###IF YOU HAVE VIDEO INPUT ISSUES WITH OPENCV in WSL2###
#Your WSL2 kernel likely doesn't have the proper drivers to use the webcam since the WSL2 kernel does not include camera
#drivers or USB support and you'll need to rebuild your kernel with those added configurations


###I pulled from 4-5 other solutions across different forums since they were all outdated or didn't work for my WSL2 instance, this is notes on how I got my webcam functioning with OpenCV on WSL2 Ubuntu###
#grab updated sources##
sudo apt update 
sudo apt upgrade

##install needed dependencies##
sudo apt install -y build-essential flex bison \
libgtk2.0-dev libelf-dev libncurses-dev autoconf \
libudev-dev libtool zip unzip v4l-utils libssl-dev \
python3-pip cmake git iputils-ping net-tools
sudo apt-get install v4l2-utils #Also installed this for V4L2 webcam devices to cover my bases

###grab right version of your wsl2 kernel I used most recent version###
sudo git clone https://github.com/microsoft/WSL2-Linux-Kernel.git

##configure your kernel##
cd WSL2-Linux-Kernel
sudo cp /proc/config.gz config
sudo mv config .config
make menuconfig KCONFIG_CONFIG=Microsoft/config-wsl

##rebuilt the WSL2 kernel to include these configurations###
General setup --> Local version --> add usb-add as suffix so you can verify new kernel later on 
[ ]Networking Support --> [*]Networking Support 
Device Drivers --> [*]Network Device Support
Device Drivers --> [*]USB support --> [*] Support for Host-side USB 
Device Drivers --> Network Device Support --> [*] USB Network Adapters --> [*] Multi-purpose USB networking framekwork 
Device Drivers --> Network Device Support --> [*] USB Network Adapters --> -M- CDC Ethernet support(smart devices such as cable modems)
Device Drivers --> Network device support --> USB network adapters --> [*] Host for RNDIS and ActiveSync devices
Device Drivers --> [*] Multimedia Support --> [*] Filter media drives
Device Drivers --> Multimedia Support --> [*] Autoselect ancillary drivers
Device Drivers --> Multimedia Support --> Media device types --> [*] Cameras and video grabbers
Device Drivers --> USB Support --> [*] USB/IP Support
Device Drivers --> Multimedia Support --> Media drivers --> Media USB Adapters --> [*] USB Video Class(UVC)
Device Drivers --> Multimedia Support --> Media drivers --> Media USB Adapters --> [*] UVC input events device support
Device Drivers --> Multimedia Support --> Media drivers --> Media USB Adapters --> [*] GSPCA based webcams

##save then exit .config##

#compile new kernel and its modules
make KCONFIG_CONFIG=Microsoft/config-wsl -j$(nproc)
sudo make KCONFIG_CONFIG=Microsoft/config-wsl modules -j$(nproc)
sudo make modules_install -j$(nproc)
sudo make install -j$(nproc)

#copy new kernel to windows path and add path to WSL config files
sudo  cp vmlinux /mnt/c/Users/{your windows username}/vmlinux
#make a .wslconfig file if doesn't already exist at C:/Users/{your username}/.wslconfig
#put this in .wslconfig file 
[wsl2]
kernel=C:\\Users\\{your windows username}\\vmlinux

#shutdown wsl instance then start it up#
##check to make sure new kernel is running and should see your usb-add or own suffix after kernel name##
uname -r -v 