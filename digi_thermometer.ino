#include<reg51.h>
sfr LCD = 0xA0; //P2 = LCD data pins
sbit RS=P0^5; //Register Select(RS) pin of 16*2 lcd
sbit RW=P0^6; //Read/write(RW) pin of 16*2 lcd
sbit EN=P0^7; //Enable(E) pin of 16*2 lcd

sfr ADC = 0x90;
sbit RD_ADC=P3^0;
sbit WR_ADC=P3^1;
sbit INTR=P3^2;

void LCD_CMD(unsigned char x);
void LCD_DATA(unsigned char w);
void LCD_INI(void);
void Send_Data(unsigned char *str);
void msDelay(unsigned int);

void convert_display(unsigned char);
unsigned char i, value;

void main(void)
{
  msDelay(20);
  LCD_INI();
  Send_Data("Temperature");
  
  INTR=1;
  RD_ADC=1;
  WR_ADC=1;
  
  while(1)
  {
  
    WR_ADC=0;
    msDelay(1);
    WR_ADC=1;
    while(INTR==1);
    RD_ADC=0;
    value=ADC;
    convert_display(value);
    msDelay(1000);
    RD_ADC=1;
  }

}

void convert_display(unsigned char value)
{
  unsigned char x1,x2,x3;
  
  LCD_CMD(0xc6);
  
  x1=(value/5);
  x1=x1+(0x30);
  x2=value%5;
  x2=x2+(0x30);
  x3=0xDF;
  
  LCD_DATA(x1);
  LCD_DATA(x2);
  LCD_DATA(x3);
  LCD_DATA('C');
}
void LCD_CMD(unsigned char x)
{
  LCD = x;
  RS = 0;
  RW = 0;
  EN = 1;
  msDelay(10);
  EN = 0;
  return;
}
void LCD_DATA(unsigned char w)
{
  LCD = w;
  RS = 1;
  RW = 0;
  EN = 1;
  msDelay(1);
  EN = 0;
  return;
}
void Send_Data(unsigned char *Str)
{
  while(*Str)
  LCD_DATA(*Str++);
}

void LCD_INI(void)
{
  msDelay(100);
  LCD_CMD(0x38);
  LCD_CMD(0x0E);
  LCD_CMD(0x01);
}
void msDelay(unsigned int Time)
{
  unsigned int y,z;
  for(y=0; y<Time; y++)
  for(z=0; z<500; z++);
}
