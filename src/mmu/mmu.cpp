#include "mmu.hpp"
#include "stdlib.h"

using namespace std;

mmu::mmu(joypad * j, cartridge * crt) {	
	this->wram = (uint8_t * )calloc(32*1024, sizeof(uint8_t));
	this->vram = (uint8_t * )calloc(16*1024, sizeof(uint8_t));
	this->j = j;
	this->crt = crt;
}	


uint8_t mmu::read8(uint16_t address) {

	// all readable locations

	if(address < 0x0000 && address <= 0x3fff) {
		// 16KB ROM banks
		this->crt->read(address);
	}
	else if(address >= 0x8000 && address <= 0x9FFF) {
		// 8KB VRAM bank 0 .. 1
	}
	else if(address >= 0xA000 && address <= 0xBFFF) {
		// 8KB External RAM
		this->crt->read(address); 
	}
	else if(address >= 0xC000 && address <= 0xCFFF) {
		// 4KB WRAM bank 0 
	}
	else if(address >= 0xD000 && address <= 0xDFFF) {
		// 4KB WRAM bank 1 .. n
	}
	else if(address >= 0xE000 && address <= 0xFDFF) {
		// ECHO thing 
	}
	else if(address >= 0xFE00 && address <= 0xFE9F) {
		// Sprite attribute table (OEM)
	}
	else if(address >= 0xFEA0 && address <= 0xFEFE) {
		// Not Usable
	}
	else if(address >= 0xFF00 && address <= 0xFF7F) {
		//I/O	
		if(address == 0xFF00)
			this->j->read(address);
	}
	else if(address >= 0xFF80 && address <= 0xFFFE) {
		// HRAM
	}
	else if(address == 0xFFFF) {
		// 4KB WRAM bank 0 
	}
	else {
		// error
	}
}

int mmu::write8(uint16_t address, uint8_t data) {

	// all writeable locations

	if(address >= 0x0000 && address <= 0x7fff) {
		// toggle ram rtc, set rom bank, set external ram bank, latch clock
		this->crt->write(address, data);
	}
	else if(address >= 0x8000 && address <= 0x9FFF) {
		// 8KB VRAM bank 0 .. 1
	}
	else if(address >= 0xA000 && address <= 0xBFFF) {
		// 8KB External RAM
		this->crt->read(address); 
	}
	else if(address >= 0xC000 && address <= 0xCFFF) {
		// 4KB WRAM bank 0 
	}
	else if(address >= 0xD000 && address <= 0xDFFF) {
		// 4KB WRAM bank 1 .. n
	}
	else if(address >= 0xE000 && address <= 0xFDFF) {
		// ECHO thing 
	}
	else if(address >= 0xFE00 && address <= 0xFE9F) {
		// Sprite attribute table (OEM)
	}
	else if(address >= 0xFEA0 && address <= 0xFEFE) {
		// Not Usable
	}
	else if(address >= 0xFF00 && address <= 0xFF7F) {
		//I/O
		if(address == 0xFF00)
			this->j->write(address, data);
	}
	else if(address >= 0xFF80 && address <= 0xFFFE) {
		// HRAM
	}
	else if(address == 0xFFFF) {
		// 4KB WRAM bank 0 
	}
	else {
		// error
	}
}


