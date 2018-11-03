#include "joypad.hpp"
#include "joypad.hpp"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


joypad::joypad() {
	
	this->locFF00 = (uint8_t *)calloc(1, sizeof(uint8_t));

	// set the control bits 
	* this->locFF00 = 0b00001111;
	
}

uint8_t joypad::read(uint16_t address) {
	uint8_t data;
	memcpy(&data, this->locFF00, 8);
	return data; 
}

int joypad::write(uint16_t address, uint8_t data) {
	uint8_t write_mask = 0b00110000; // corresponding to 4-5 bits of the locFFOO
	data = data & write_mask;
	*this->locFF00 = (*this->locFF00 & (~write_mask)) | data;
	return 0;
}

void joypad::printMemory() {
	printf("0xFF00 : %08lu\n", *this->locFF00);
} 




// int main() {

// 	joypad *j = new joypad();

// 	j->printMemory();
// 	j->write(NULL, 0b00100000);

// 	j->printMemory();

// }

