# Xmotion Control HMI
## Designed exclusively for our Xmotion Gen2 Embedded Control (Single-board motion control solution)
### Also compatible with Rpi, Beagle Bone, or virtually any other embeded linux system. GUI runs without the need for X window system and outputs graphics directly to the framebuffer. Also reads keyboard and mouse directly from /dev/input/eventX devices

# Dependencies
python

# Building
## LinuxCNC machine
make clean
make release

## Build as "Simulator", can execute binary without LinuxCNC RTAPI Running
make clean
make sim

## Build with debuging output, requires linuxcnc to be running
make clean
make debug

# Configuring (This will eventually be added to make install)
mkdir /etc/Xmotion

cp config_example.ini /etc/Xmotion/config.ini

# Running
Launch script (./launch) calls "linuxcnc configs/Gen2/Gen2.ini" and the Xmotion binary is called from Gen2.ini's display= setting. You need to create a linuxcnc config in the conventional way to interface with your physical machine and set your display variable to suit. I use the launch script in the dev environment to reduce iteration time. After "make install" has been issued, any linuxcnc ini in /home/$USER/linuxcnc/configs/* can have display=Xmotion instead of display=axis for ex.
