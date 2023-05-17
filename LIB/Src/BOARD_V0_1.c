/*
    add thư viện
    project -> properties -> c/c++ general -> Path and symbols
    tab: Soure location -> link forder -> choose "link to folder in the file system"
*/
#include "BOARD_V0_1.h"

const char code7seg[] = {0xD7, 0x14, 0xCD, 0x5D, 0x1E, 0x5B, 0xDB, 0x15, 0xDF, 0x5F};
//uint8_t *data = (uint8_t*)malloc(NUMBER_OF_IC74595 * sizeof(uint8_t)); // phải chạy trong main mới chịu
uint8_t s7seg[7] = {};


void Scan7seg(){
	static uint32_t timeCurrentFor7segment = 0;
	static uint8_t index = 0;
	if(index < 7){
		if(HAL_GetTick() - timeCurrentFor7segment >= 1) {
			timeCurrentFor7segment = HAL_GetTick();
			*(data) = 0x01 << index;
			*(data + 1) = s7seg[index];
			IC74595_SendData(data, NUMBER_OF_IC74595);
			index++;
		}
	}
	else{
		index = 0;
	}
}
