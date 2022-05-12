/*
 * MTi2.h
 *  Created on: 2022年5月12日
 *      Author: liwei
 */
 
#ifndef INC_MTI2_H_
#define INC_MTI2_H_
 
#include "mbed.h"
/*#include "main.h"*/
 
//define opcode
#define ProtInfo    0x01
#define ConfigProt  0x02
#define Control     0x03
#define PipeStat    0x04
#define NotiPipe    0x05
#define MeasPipe    0x06
 
class MTi2Class{
    private:
        DigitalOut cs_MTi;
        SPI spi_MTi;
        int len;
        int ctrl_len;
        uint16_t notificationSize;
        uint16_t measurementSize;
        uint8_t buffer[40];
        uint8_t ctrlBuf[20];
        uint8_t FW[4];

        typedef union{
            uint32_t data1;
            float data2;
        }imu_data;

        imu_data eul[3];
        imu_data acc[3];
        imu_data gry[3];

    public:
        MTi2Class(int freq,PinName MISO,PinName MOSI,PinName SCLK,PinName CS);
        ~MTi2Class();
        void MTi2_Init();
        void SendOpcode(uint8_t Opcode);
        uint8_t ReadProtInfo();
        void ConfigureProt(_Bool M,_Bool N,_Bool O,_Bool P);
        void PipeStatus();
        void NotificationPipe();
        void MeasurementPipe();
        void ControlPipe();
        void ReadData();

        float euler[3];
        float accel[3];
        float omega[3];
};

float   lpf(float input, float output_old, float frequency);    //lpf discrete
 
#endif /* INC_MTI2_H_ */