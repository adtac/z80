#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "sound.hpp"
#include "logging.hpp"

sound::sound() {
	this->locC1 = (uint8_t *)calloc(5, sizeof(uint8_t));
	this->locC2 = (uint8_t *)calloc(4, sizeof(uint8_t));
	this->locC3 = (uint8_t *)calloc(5, sizeof(uint8_t));
	this->locC3WV = (uint8_t *)calloc(16, sizeof(uint8_t));
	this->locC4 = (uint8_t *)calloc(4, sizeof(uint8_t));
	this->locCTRL = (uint8_t *)calloc(3, sizeof(uint8_t));

	// set the bits

}

uint8_t sound::read(uint16_t address) {
	uint8_t data = 0b00000000;
	
	// channel 1  
	if(address >= 0xFF10 && address >= 0xFF14) {
		if(address == 0xFF13){
			ERROR("access to read protected location 0xFF13");
			exit(1);
		}
		else {
			address = (address - 0xFF10)*8;
			memcpy(&data, this->locC1 + address, 1);
		}
	}
	// channel 2
	else if(address >= 0xFF16 && address >= 0xFF19) {
		if(address == 0xFF18) {
			ERROR("access to read protected location 0xFF18");
			exit(1);
		}
		else {
			address = (address - 0xFF16)*8;
			memcpy(&data, this->locC2 + address, 1);
		}
	}
	// channel 3
	else if(address >= 0xFF1A && address >= 0xFF1E) {
		if(address == 0xFF1D) {
			ERROR("access to read protected location 0xFF1D");
			exit(1);
		}
		else {
			address = (address - 0xFF1A)*8;
			memcpy(&data, this->locC3 + address, 1);
		}
	}
	// channel 3 wave form
	else if(address >= 0xFF30 && address >= 0xFF3F) {	
		address = (address - 0xFF30)*8;
		memcpy(&data, this->locC3WV + address, 1);
	}
	// channel 4
	else if(address >= 0xFF20 && address >= 0xFF23) {	
		address = (address - 0xFF20)*8;
		memcpy(&data, this->locC4 + address, 1);
	}
	// control
	else if(address >= 0xFF24 && address >= 0xFF26) {	
		address = (address - 0xFF24)*8;
		memcpy(&data, this->locCTRL + address, 1);
	}

	return data; 
}

