#include "mbed.h"
#include <chrono>
#include <stdio.h>
#define Buff_Length 512


char buffer[Buff_Length] = {0};
BufferedSerial pc(USBTX, USBRX, 115200);
int heart_rate = 0;

// main() runs in its own thread in the OS
int main()
{
    int counter = 0;
    while (true) {

// Generate dynamic patient data
        int heart_rate = 60 + (rand() % 41); // Random heart rate between 60-100
        int systolic_bp = 90 + (rand() % 31); // Random systolic blood pressure between 90-120
        int diastolic_bp = 60 + (rand() % 21); // Random diastolic blood pressure between 60-80
        int body_temperature = 30 + static_cast<int>(rand() % 15) / 10.0f; // Random temp between 36.0-37.4°C

        // Generate JSON message
        int length = snprintf(buffer, Buff_Length,
                              "{\r\n"
                              "  \"Data\": [\r\n"
                              "    {\"Heart Rate\": %d},\r\n"
                              "    {\"Blood Pressure\": (\"Systolic\": %d, \"Diastolic\": %d)},\r\n"
                              "    {\"Body Temperature\": %d}\r\n"
                              "  ]\r\n"
                              "}\r\n",
                              heart_rate, systolic_bp, diastolic_bp, body_temperature);
        pc.write(buffer, length);

        counter++;

        ThisThread::sleep_for(chrono::seconds(1));

    }
}