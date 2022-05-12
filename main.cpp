#include <stdio.h>
#include "mbed.h"
#include <math.h>
#include <stdlib.h>
#include "MTi2.h"
 
#define GRAVITY 9.81f
#define pi 3.14159265358979323846f
MTi2Class IMU(1000000,PB_15,PB_14,PB_13,PC_4);//Create MTi2 Class (Frequency, MOSI, MISO, SCLK, CS)
RawSerial pc(USBTX,USBRX);
Ticker main_function; //interrupt
 
bool button_state = false;
float dt = 0.01; // sec
 
void Start_read();
 
bool flag = false;
 
int main(void)
{
    pc.baud(115200);

    IMU.MTi2_Init();//Initialize MTi-2
    
    main_function.attach_us(&Start_read,dt*1000000);//Ticker

    while (1) {
        if(flag == true){
            IMU.ReadData();//Read imu data
            flag = false;//Clear flag
            printf("%f,%f,%f\r\n",IMU.euler[0],IMU.euler[1],IMU.euler[2]);//print data
        }
    }
 
}
 
void Start_read()//interrupt function by Ticker
{
    flag = true;//Set flag
}
