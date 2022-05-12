Description:
Code for STM32.
MTi2.cpp and MTi2.h are code for SPI commnication with MTi-2-T imu sensor.
It contains basic operations such as sending command, receive pipe status, configure pipe, etc.
It also can read euler angle, acceleration, and anular velocity by ReadData() function.
Reading other data can be done by changing code in ReadData() and setting MTi-2-T from MT Manager.

Target Hardware:
NUCLEO-F446RE

Reference:
MT Manager:https://www.xsens.com/software-downloads
MTi-2 datasheet:https://www.mouser.tw/datasheet/2/693/mti_1_series_datasheet-1358521.pdf
Xsens low level documentation:https://www.xsens.com/hubfs/Downloads/Manuals/MT_Low-Level_Documentation.pdf

How To Use MTi2.cpp & MTi2.h?
1) Import MTi2.cpp & MTi2.h
2) Declare MTi2Class with SPI frequency,pins in main.cpp( e.g. MTi2Class IMU(freq,MOSI,MISO,SCLK,CS) )
3) Call MTi2_init() for initialization( e.g. IMU.MTi2_init() )
4) Call ReadData() to read MTi-2 data( e.g. IMU.ReadData() )
5) Access data by calling public member euler,accel,omega( e.g. IMU.euler[i],IMU.accel[j],IMU.omega[k] )