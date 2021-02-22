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
 	//crcУ�飬���λ��1��^0x31
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
   //0x2130  ��ʾ����ģʽ ����1ms
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
//�����ǰ���һ����Ҳ��дһ�������SHT30����������Ƿ���SHT30ת������ļĴ�����
   IIC_Start();
   IIC_Send_Byte(0x88);
   IIC_Wait_Ack();
   IIC_Send_Byte(0xe0);
   IIC_Wait_Ack();
   IIC_Send_Byte(0x00);
   IIC_Wait_Ack();
//�����ǿ�ʼ��ȡ���ݣ����е�����p��Ž����ǰ��������¶�ֵ����������ʪ��ֵ����ǰ�����¶�ֵ���棬
//p[0]���¶ȵĸ߰�λ��p[1]�ǵͰ�λ��p[2]��CRCУ�飬�й�CRCУ���֪ʶ���ǰٶ����濴�ģ�
//Ҫ�Ǹ�λ�����û������Բ���crcУ�飬ֱ����p[0]��p[1]�Ϳ���ת�������¶ȵ�ֵ��
   IIC_Start();
   IIC_Send_Byte(0x89);
   IIC_Wait_Ack();
   
   //ǰ��ζ�ȡ��Ҫ����ack�źţ����һ�ξͲ��÷��ˡ�
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
  //sht30_data_buffer����������ҳ�������ʹ�õĴ��SHT30�Ĵ���������ֵ�����飬
  //��������Ϊ unsigned short int   sht30_data_buffer[6]
  sht30_read_temp_humi(sht30_data_buffer);
  //�ȴ����¶ȵ�������ݣ�λ�������ǰ����
  temporary[0]=sht30_data_buffer[0];
  temporary[1]=sht30_data_buffer[1];
  temporary[2]=sht30_data_buffer[2];
  //crcУ��
  crc_result=sht30_crc8_check(temporary,2,temporary[2]);
  //crcУ��Ҫ�ǲ��ɹ��ͷ���1��ͬʱ��������¶�ֵ
  if(crc_result==0)
  {
  //��2��8λ����ƴ��Ϊһ��16λ������
  data=((u16)temporary[0] << 8) | temporary[1];
  //�¶�ת������16λ�¶�����ת��Ϊ10���Ƶ��¶����ݣ����ﱣ����һλС����SHT30_temperature����һ��ȫ�ֱ���������Ϊʲô�������������и�.�����ø�λ���԰ٶ�һ��c���Խṹ������˵����
  SHT30_temperature = (int)((175.0 * ((float)data) / 65535.0 - 45.0) *10.0);
  }
  else
  {
  return 1;
  }
  temporary[0]=sht30_data_buffer[3];
  temporary[1]=sht30_data_buffer[4];
  temporary[2]=sht30_data_buffer[5];
  //crcУ��
  crc_result=sht30_crc8_check(temporary,2,temporary[2]);
  	if(crc_result==0)
  {
  //�ο������¶ȵĴ���
  data=((u16)temporary[0] << 8) | temporary[1];
  SHT30_humidity = (int)((100.0 * (float)data / 65535.0) *10.0); 
  return 0;
  }
  else
  {
  return 2;
  }
}


