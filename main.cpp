#include "mbed.h"
#include <chrono>
#include <cstdlib>
#include <stdio.h>
#include <cstring>

#define Buff_Length 512  // Larger buffer allows carrying larger messages

BufferedSerial pc(USBTX, USBRX, 115200); // Serial communication
DigitalOut led(LED1);  // Onboard LED

char buffer[Buff_Length] = {0}; // Buffer for sending data
char rx_buffer[Buff_Length] = {0}; // Buffer for receiving data

// x, y coordinates for the robot
int x = 0;
int y = 0;

int Battery = rand() % 30 + 50; // Randomly generated robot battery

// Function to process received commands
void processCommand(char *command) {
    char *token = strtok(command, " "); // Tokenize by space

// Basic onboard LED ON/OFF Function
    if (strcmp(token, "LED_ON") == 0) 
    {
        led = 1;
    } 
    else if (strcmp(token, "LED_OFF") == 0) 
    {
        led = 0;
    } 

    if (Battery > 15) // if batterry above 15 the robot can be controlled
    {
        if (strcmp(token, "POS1") == 0) // right pos
        {
            x = 15;
            y = 0;
        } 
        else if (strcmp(token, "POS2") == 0) // left pos
        {
            x = -18;
            y = 0;
        } 
        else if (strcmp(token, "POS3") == 0) // up pos
        {
            y = 23;
            x = 0;
        } 
        else if (strcmp(token, "POS4") == 0)  // down pos
        {
            y = -28;
            x = 0;
        } 
        else if (strcmp(token, "HOME") == 0) // home
        {
            x = 0;
            y = 0;

        } 
    }
    else if (strcmp(token, "RECHARGE") == 0) // recharging the robot
    {
        Battery = 100;
    } 
    
    else // dealing with unkown command
    {
        snprintf(buffer, Buff_Length, "Unknown Command: %s\r\n", command);
        pc.write(buffer, strlen(buffer));
    }
}

// Main function
int main()
{
    pc.set_blocking(false); // Non-blocking serial communication
    printf("Nucleo Ready. Waiting for commands...\r\n");


    while (true) {
        if ( Battery > 15 )  // if battery above 15 the battery drops
        {
            Battery --;
        }
        else  // if battery below 15 robot sets to home and waits for recharge
        {
            x = 0;
            y = 0;
        }
        //printing JSON message
        int length = snprintf(buffer, Buff_Length,"{Data: Robot Location {X: %d}, {Y: %d}}\r\n{Data: Battery: %d}\r\n", x, y, Battery);
        pc.write(buffer, length);

        // Read and process incoming commands
        if (pc.readable()) {
            int i = 0;
            char c;
            while (pc.read(&c, 1) == 1) { // if \r or \n has been identified process the line using processcommand() function
                if (c == '\n' || c == '\r') { 
                    rx_buffer[i] = '\0'; 
                    processCommand(rx_buffer); 
                    i = 0; // Reset buffer
                } else {
                    rx_buffer[i++] = c; // Store received character
                }
            }
        }

        ThisThread::sleep_for(chrono::seconds(1)); // Delay for 1 second
    }
}
