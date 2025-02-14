#include "mbed.h"
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdio.h>

#define BLINKING_RATE 10ms
#define MSG_BUFF_LENGTH 32
BufferedSerial pc(USBTX, USBRX, 115200);
char MSG[32];
int length;
char buff[32] = {0};
DigitalOut led(LED1);
DigitalOut led1(D2);

// main() runs in its own thread in the OS
int main()
{
    length = snprintf(MSG, MSG_BUFF_LENGTH,"\r\nProgramme Starting\r\n");
    pc.write(MSG,length);

    while (true) {
        led = !led;
        if (uint32_t num = pc.read(buff, sizeof(buff)))
        {
            if (strcmp(buff, "1") == 0 )
            {
                led1 = !led1;
                length = snprintf(MSG, MSG_BUFF_LENGTH, "\r\nReceived Command LED1\r\n");
                pc.write(MSG,length);
            }
            else if (strcmp(buff, "s") == 0 )
            {
                length = snprintf(MSG, MSG_BUFF_LENGTH, "\r\n{Status :{LED1:%d}}\r\n", led1.read());
                pc.write(MSG,length);
            }
        }
        ThisThread::sleep_for(BLINKING_RATE);
        

    }
}

