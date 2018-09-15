# Xmotion Control HMI
## Designed exclusively for our Xmotion Gen2 Embedded Control (Single-board motion control solution)
### Also compatible with Rpi, Beagle Bone, or virtually any other embeded linux system. GUI runs without the need for X window system and outputs graphics directly to the framebuffer. Also reads keyboard and mouse directly from /dev/input/eventX devices

# Dependencies
The only linked library is python - this will later be changed after the python linuxcnc interface has been replaced with a pure c implementation. Right now there is virtually no documention on LinuxCNC NML in C but an example of this is included at /test/test.cpp that works and will be devopled later after the gcode viewer is finished.

# Building
make clean

make

# Configuring (This will eventually be added to make install)
mkdir /etc/Xmotion

cp config_example.ini /etc/Xmotion/config.ini

# Running
Launch script (./launch) calls "linuxcnc configs/Gen2/Gen2.ini" and the Xmotion binary is called from Gen2.ini's display= setting. You need to create a linuxcnc config in the conventional way to interface with your physical machine and set your display variable to suit. I use the launch script in the dev environment to reduce iteration time. After "make install" has been issued, any linuxcnc ini in /home/$USER/linuxcnc/configs/* can have display=Xmotion instead of display=axis for ex.




