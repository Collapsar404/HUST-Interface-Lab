#if defined(D_NEXYS_A7)
   #include <bsp_printf.h>
   #include <bsp_mem_map.h>
   #include <bsp_version.h>
#else
   PRE_COMPILED_MSG("no platform was defined")
#endif
#include <psp_api.h>

#include <i2c.h>
#include <ADT7420.h>

#define IO_LEDR     0x80100000
#define IO_SWs		0x80100008

#define PWM_BASE    0x80120000

#define SegEn_ADDR      0x80001038
#define SegDig_ADDR     0x8000103C

#define SPI_BASE        0x80001100
#define SPI_ENABLE      0x80001120
#define SPI_DATA        0x80001110

#define READ_IO(dir) (*(volatile unsigned *)dir)
#define WRITE_IO(dir, value) { (*(volatile unsigned *)dir) = (value); }

#define Delay 1000000

void delay(int num)
{
    volatile int i;
    for(i=0; i<num; i++);
}

void SPI_INIT() {
    WRITE_IO(SPI_BASE, 0x3);
    delay(Delay);

    WRITE_IO(SPI_BASE, 0x43);
    delay(Delay);
}

void SPI_WRITE(unsigned data) {    
    WRITE_IO(SPI_DATA, data);
    delay(Delay);
}

unsigned SPI_READ() {
    unsigned data;
    
    data = READ_IO(SPI_DATA);
    delay(Delay);
    
    return data;
}




void LCD_WRITE(unsigned int data) {
    WRITE_IO(SPI_ENABLE, 0);
    delay(Delay);

    // SPI_WRITE(0xA);
    // delay(Delay);
    
    
    
    SPI_WRITE(data);
    delay(Delay);

    WRITE_IO(SPI_ENABLE, 1);
    delay(Delay);
}

void HelloWorld(){
    WRITE_IO(SPI_ENABLE, 1);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x1B);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x5B);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x30);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x6A);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x48);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x45);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x4C);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x4C);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x4F);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x20);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x57);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x4F);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x52);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x4C);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x44);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x21);
    delay(Delay);
    WRITE_IO(SPI_ENABLE, 0);
    delay(Delay);
}

void TempDisp(int temp){
    temp = temp / 16;
    unsigned int a = temp / 10;
    unsigned int b = temp % 10;

    WRITE_IO(SPI_ENABLE, 1);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x1B);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x5B);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x30);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x6A);
    delay(Delay);


    WRITE_IO(SPI_DATA, 'T' - 'A' + 0x41);
    delay(Delay);
    WRITE_IO(SPI_DATA, 'E' - 'A' + 0x41);
    delay(Delay);
    WRITE_IO(SPI_DATA, 'M' - 'A' + 0x41);
    delay(Delay);
    WRITE_IO(SPI_DATA, 'P' - 'A' + 0x41);
    delay(Delay);
    WRITE_IO(SPI_DATA, 'E' - 'A' + 0x41);
    delay(Delay);
    WRITE_IO(SPI_DATA, 'R' - 'A' + 0x41);
    delay(Delay);
    WRITE_IO(SPI_DATA, 'A' - 'A' + 0x41);
    delay(Delay);
    WRITE_IO(SPI_DATA, 'T' - 'A' + 0x41);
    delay(Delay);
    WRITE_IO(SPI_DATA, 'U' - 'A' + 0x41);
    delay(Delay);
    WRITE_IO(SPI_DATA, 'R' - 'A' + 0x41);
    delay(Delay);
    WRITE_IO(SPI_DATA, 'E' - 'A' + 0x41);
    delay(Delay);

    WRITE_IO(SPI_DATA, 0x3A);   //':'
    delay(Delay);

    WRITE_IO(SPI_DATA, 0x30 + a);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0x30 + b);
    delay(Delay);
    WRITE_IO(SPI_DATA, 0xDF);
    delay(Delay);
    WRITE_IO(SPI_DATA, 'C' - 'A' + 0x41);
    delay(Delay);

    
    WRITE_IO(SPI_DATA, 0x6A);
    delay(Delay);

    WRITE_IO(SPI_ENABLE, 0);
    delay(Delay);
}

int main() {
    int displayData=0;
    int LastTemp = -300;
    int AlarmTemp = 25;                     //alarm temperature,25 centigrade
    WRITE_IO(SegEn_ADDR, 0x0);
    WRITE_IO(SegDig_ADDR, displayData);
    WRITE_IO(IO_LEDR, 0x000f);
    WRITE_IO(PWM_BASE, 0);
    SPI_INIT();
    uartInit();
    ADT7420_Init();
    delay(Delay);

    HelloWorld();
    
    while(1) {
        WRITE_IO(IO_LEDR, 0x00ff);
        displayData = ADT7420_ReadTemp();
        WRITE_IO(IO_LEDR, 0xffff);
        WRITE_IO(SegDig_ADDR, displayData);
        while (displayData != LastTemp)     //refresh lcd
        {
            LastTemp = displayData;
            TempDisp(displayData);
        }
        while (LastTemp / 16 >= AlarmTemp)  //alarm
        {
            WRITE_IO(PWM_BASE, 990000);
            WRITE_IO(IO_LEDR, 0xffff);
            delay(Delay);
            WRITE_IO(IO_LEDR, 0x0000);
            delay(Delay);
        }
    }
    return 0;
}