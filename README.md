# wall-e
Robot's remote control application suite. 

##Description
Suite consists of two applications: Control application and Remote application. Former is designed to control periferal components and sensors of robot. It provides graphical UI and communication with remote side. Last one is used for remote configuration, monitoring and control. Communication between applications is based on TCP protocol. Both applications are built with open sourse version of Qt. Video and audio streams are implemented using gstreamer and VLC packages. 

Remote application:

<img src="presentation/wall_e_remote_app.jpg">

Below is reference car robot platform based on AT91SAM9G45-EKES board. The main features are web-camera with video and audio support, four motors, battery, battery sensor, ultrasonic sensor, hall sensor, WiFi AP, horn and headlights. The robot has its own touchscreen display for in place diagnostic and configuration. 
 
<img src="presentation/at91sam9g45_platform_robot_side.jpg" width="608">

<img src="presentation/at91sam9g45_platform_robot_front.jpg" width="608">


## Build

### Remote application requirements:
Windows 7/Kubuntu 14.04: Qt 5.3.2 32bit, g++ 4.9.2, VLC library 2.1.4.

### Control application requirements:
Embedded linux: buildroot-2013.05 (Qt embedded 4.8.4, g++ 4.7.3).  
Kubuntu 14.04: Qt 4.8.4, g++ 4.7.3

##Demo run
Control application and remote application can run on the same PC for demo purposes.  

###Host requirements:
Kubuntu 14.04, gstreamer 0.10.36 + plugins.

###Execute applications:
1. Create ethernet device alias with 192.168.3.1 IP:  
$ sudo ifconfig eth0:0 192.168.3.1 up
2. Execute desktop build of Control application.
3. Execute Remote application and press "Connect" button.
4. After connection is established connection status is "Connected", video from PC web-camera is shown and audio from PC microphone can be heard in headphones.  

##AT91SAM9G45-EKES car robot platform

1. Download Buildroot 2013.05 or later from http://buildroot.uclibc.org/downloads/ and build root FS using config located in platform/at91sam9g45-ekes/buildroot-2013.05 folder.
2. Build Linux 2.6.30 with patches provided along with AT91SAM9G45-EKES boad and config located in platform/at91sam9g45-ekes/linux-2.6.30/ folder.
3. Linux config file turns on support of PCF8591 8-bit ADC used for battery voltage measurements. In order to use this I2C device add its name and address to I2C devices table and recompile kernel:  
arch/arm/mach-at91/board-sam9m10g45ek.c  
static struct i2c_board_info __initdata ek_i2c_devices[] = {  
&nbsp;&nbsp;&nbsp;&nbsp;...  
&nbsp;&nbsp;&nbsp;&nbsp;{  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;I2C_BOARD_INFO("pcf8591", 0x48),  
&nbsp;&nbsp;&nbsp;&nbsp;},  
};
4. Program board with default AT91SAM9G45-EKES bootloaders, custom kernel and root FS.
5. Execute ts_calibrate in order to calibrate touchcreen.
6. Add touchscreen settings export to /etc/profile:  
export TSLIB_TSDEVICE=/dev/input/event1  
export TSLIB_TSEVENTTYPE=INPUT  
export TSLIB_CONFFILE=/etc/ts.conf  
export TSLIB_CALIBFILE=/etc/pointercal  
export QWS_MOUSE_PROTO="Tslib:/dev/input/event1"  
7. To be continued...


