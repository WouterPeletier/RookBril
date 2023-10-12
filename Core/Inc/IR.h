#include <stm32f4xx.h>
#include <stdio.h>

typedef enum IRMode {Send, Receive} IRMode;

typedef struct IRPacket{
    uint8_t IRAddress;
    uint8_t IRData;
} IRPacket;

void receive();