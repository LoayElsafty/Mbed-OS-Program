#include "mbed.h"
#include <chrono>
#include <stdio.h>

#define Buff_Lenth 64

int length;
char buffer[Buff_Lenth] = {0};
BufferedSerial pc(USBTX, USBRX, 115200);

// main() runs in its own thread in the OS
int main()
{
    while (true) {
        length = snprintf(buffer, Buff_Lenth, "\r\nHello");
        pc.write(buffer, length);
        ThisThread::sleep_for(chrono::seconds(1));

    }
}

