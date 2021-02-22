#include "sht30.h"

u8 sht30_data_buffer[6];
u16 SHT30_temperature;
u16 SHT30_humidity;

 int  crc8_compute(u8 *check_data, u8 num_of_data)
{
 	uint8_t bit;        // bit mask
 uint8_t crc = 0xFF; // calculated checksum
 uint8_t byteCtr;    // byte counter

 // calculates 8-Bit checksum with given polynomial
 for(byteCtr = 0; byteCtr < num_of_data; byteCtr++) {
     crc ^= (check_data[byteCtr]);
 	//crc校验，最高位是1就^0x31
     for(bit = 8; bit > 0; --bit) {
         if(crc & 0x80) {
             crc = (crc << 1) ^ 0x31;
         }  else {
             crc = (crc << 1);
         }
     }
 }
 return crc;
}
int sht30_crc8_check(u8 *p,u8 num_of_data,u8 CrcData)
{
  uint8_t crc;
  crc = crc8_compute(p, num_of_data);// calculates 8-Bit checksum
 if(crc != CrcData) 
 {   
     return 1;           
 }
 return 0;
}


void SHT_Init(void)
{
   delay_ms(250); 
   //0x2130  表示周期模式 周期1ms
   	IIC_Start();
   	IIC_Send_Byte(0x88);
   	IIC_Wait_Ack();
   	IIC_Send_Byte(0x21);
   	IIC_Wait_Ack();
   	IIC_Send_Byte(0x30);
   	IIC_Wait_Ack();
     IIC_Stop();
   
   delay_ms(150); 
}

void sht30_read_temp_humi(u8 *p)
{
//这里和前面的一样，也是写一个命令给SHT30，这个命令是访问SHT30转换结果的寄存器的
   IIC_Start();
   IIC_Send_Byte(0x88);
   IIC_Wait_Ack();
   IIC_Send_Byte(0xe0);
   IIC_Wait_Ack();
   IIC_Send_Byte(0x00);
   IIC_Wait_Ack();
//下面是开始读取数据，其中的数组p存放结果，前三个存放温度值，后三个是湿度值，在前三个温度值里面，
//p[0]是温度的高八位，p[1]是低八位，p[2]是CRC校验，有关CRC校验的知识我是百度上面看的，
//要是各位不懂得话，可以不用crc校验，直接用p[0]、p[1]就可以转换出来温度的值。
   IIC_Start();
   IIC_Send_Byte(0x89);
   IIC_Wait_Ack();
   
   //前五次读取都要发送ack信号，最后一次就不用发了。
   p[0] = IIC_Read_Byte(1);
   p[1] = IIC_Read_Byte(1);
   p[2] = IIC_Read_Byte(1);
   p[3] = IIC_Read_Byte(1);
   p[4] = IIC_Read_Byte(1);
   p[5] = IIC_Read_Byte(0);
   IIC_Stop();
}

int sht30_data_process(void)
{
  u8 temporary[3];
  u16 data;
  u8 crc_result;
  //sht30_data_buffer这个变量是我程序里面使用的存放SHT30寄存器读到的值的数组，
  //定义类型为 unsigned short int   sht30_data_buffer[6]
  sht30_read_temp_humi(sht30_data_buffer);
  //先处理温度的相关数据，位于数组的前三个
  temporary[0]=sht30_data_buffer[0];
  temporary[1]=sht30_data_buffer[1];
  temporary[2]=sht30_data_buffer[2];
  //crc校验
  crc_result=sht30_crc8_check(temporary,2,temporary[2]);
  //crc校验要是不成功就返回1，同时不会更新温度值
  if(crc_result==0)
  {
  //把2个8位数据拼接为一个16位的数据
  data=((u16)temporary[0] << 8) | temporary[1];
  //温度转换，将16位温度数据转化为10进制的温度数据，这里保留了一位小数，SHT30_temperature这是一个全局变量，至于为什么变量名字里面有个.不懂得各位可以百度一下c语言结构体的相关说明。
  SHT30_temperature = (int)((175.0 * ((float)data) / 65535.0 - 45.0) *10.0);
  }
  else
  {
  return 1;
  }
  temporary[0]=sht30_data_buffer[3];
  temporary[1]=sht30_data_buffer[4];
  temporary[2]=sht30_data_buffer[5];
  //crc校验
  crc_result=sht30_crc8_check(temporary,2,temporary[2]);
  	if(crc_result==0)
  {
  //参考上面温度的代码
  data=((u16)temporary[0] << 8) | temporary[1];
  SHT30_humidity = (int)((100.0 * (float)data / 65535.0) *10.0); 
  return 0;
  }
  else
  {
  return 2;
  }
}


