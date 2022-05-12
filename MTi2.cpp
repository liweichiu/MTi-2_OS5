#include "MTi2.h"

//Constructor & construct cs pin & spi pin
MTi2Class::MTi2Class(int freq,PinName MOSI,PinName MISO,PinName SCLK,PinName CS):cs_MTi(CS),spi_MTi(MOSI,MISO,SCLK){
    //Set SPI
    spi_MTi.format(8,3);
    spi_MTi.frequency(freq);
}
//Initialize MTi-2 setting
void MTi2Class::MTi2_Init(){
    cs_MTi = 1;
    ConfigureProt(1,0,0,0);//Configure DRDY
 
}

void MTi2Class::ControlPipe(){
    cs_MTi = 0;
        SendOpcode(Control);//send opcode
        for(int i = 0;i<ctrl_len;i++){//read data
            buffer[i] = spi_MTi.write(ctrlBuf[i]);
        }
    cs_MTi = 1;
}

void MTi2Class::MeasurementPipe(){
//    printf("MeasurementPipe \r\n");
    cs_MTi = 0;
    SendOpcode(MeasPipe);//send opcode
    for(int i = 0;i<measurementSize;i++){//read data
        buffer[i] = spi_MTi.write(0x00);
    }
    cs_MTi = 1;
}

//Check pipestatus : measure size & nitification size
void MTi2Class::PipeStatus(){
    len = 4;
    cs_MTi = 0;
    SendOpcode(PipeStat);//send opcode
    for(int i = 0;i<len;i++){//read data
        buffer[i] = spi_MTi.write(0x00);
    }
    cs_MTi = 1;
    notificationSize = buffer[0] | (buffer[1]<<8);
    measurementSize = buffer[2] | (buffer[3]<<8);
//    printf("nSize:%d\r\n",notificationSize);
//    printf("mSize:%d\n",measurementSize);
}
//Send op code
void MTi2Class::SendOpcode(uint8_t Opcode)
{
//    printf("SendOpcode \r\n");
    FW[0] = spi_MTi.write((int)Opcode);
    
    for(uint8_t i = 0;i<3;i++){// 3 fillword 
        FW[i+1] = spi_MTi.write((int)i);
    }
}
 
uint8_t MTi2Class::ReadProtInfo(){
    len = 2;
    cs_MTi = 0;
    SendOpcode(ProtInfo);//send opcode
    for(int i = 0;i<len;i++){//read data
        buffer[i] = spi_MTi.write(0x00);
    }
    cs_MTi = 1;
    if(FW[0]!=0xFA||FW[1]!=0xFF||FW[2]!=0xFF||FW[3]!=0xFF){
        printf("Error!!\n");
    }
    return buffer[1];
}
 
void MTi2Class::ConfigureProt(_Bool M,_Bool N,_Bool O,_Bool P)
{
//    printf("ConfigureProt \r\n");
    uint8_t config = (M<<3) | (N<<2) | (O<<1) | (P<<0);
    cs_MTi = 0;
    SendOpcode(ConfigProt);
    spi_MTi.write(config);
    cs_MTi = 1;

}

void MTi2Class::NotificationPipe(){
    cs_MTi = 0;
    SendOpcode(NotiPipe);//send opcode
    for(int i = 0;i<notificationSize;i++){//read data
        buffer[i] = spi_MTi.write(0x00);
    }
    cs_MTi = 1;
}

//Read MTi-2 data
void MTi2Class::ReadData(){
    PipeStatus();//Check pipe statue : measurement size
    wait_us(10);
    MeasurementPipe();//Read measurement data
    int len1,len2,len3,data_bytes;
    if(buffer[0] == 0x36){//Check data is correct
        if(buffer[2]== 0x20&&buffer[3]== 0x30){//Read Euler Angle
            len1 = buffer[4];//data length
            data_bytes = len1/3;//3 dimension data
            for(int j=0;j<3;j++){
                uint32_t temp = (buffer[5+j*data_bytes]<<24) | (buffer[6+j*data_bytes]<<16) | (buffer[7+j*data_bytes]<<8) | (buffer[8+j*data_bytes]);
                eul[j].data1 = temp;
                euler[j] = eul[j].data2;
            }
        }
        if(buffer[4+len1+1]== 0x40&&buffer[4+len1+2]== 0x20){//Read Acceleration
            len2 = buffer[4+len1+3];//data length
            data_bytes = len2/3;//3 dimension data
            for(int j=0;j<3;j++){
                uint32_t temp = (buffer[8+len1+j*data_bytes]<<24) | (buffer[9+len1+j*data_bytes]<<16) | (buffer[10+len1+j*data_bytes]<<8) | (buffer[11+len1+j*data_bytes]);
                acc[j].data1 = temp;
                accel[j] = acc[j].data2;
            }
        }
        if(buffer[7+len1+len2+1]== 0x80&&buffer[7+len1+len2+2]==0x20){//Read Gryoscope
            len3 = buffer[7+len1+len2+3];//data length
            data_bytes = len3/3;//3 dimension data
            for(int j=0;j<3;j++){
                uint32_t temp = (buffer[11+len1+len2+j*data_bytes]<<24) | (buffer[12+len1+len2+j*data_bytes]<<16) | (buffer[13+len1+len2+j*data_bytes]<<8) | (buffer[14+len1+len2+j*data_bytes]);
                gry[j].data1 = temp;
                omega[j] = gry[j].data2;
            }
        }
 
    }
}