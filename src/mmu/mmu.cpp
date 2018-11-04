#include <stdlib.h>
#include <stdio.h>
#include "mmu.hpp"
#include "logging.hpp"

using namespace std;

mmu::mmu(joypad * j, cartridge * crt) {	
	this->wram = (uint8_t * )calloc(32*1024, sizeof(uint8_t));
	this->vram = (uint8_t * )calloc(16*1024, sizeof(uint8_t));
	this->locUKNS = (uint8_t * )calloc(8, sizeof(uint8_t));
	this->locFF4D = 0;
	this->locFF56 = 0;
	this->locFF70 = 0;
	this->INTENBL = 0;
	this->INTFLAG = 0;
	this->j = j;
	this->crt = crt;
}	


uint8_t mmu::read8(uint16_t address) {

	// all readable locations

	if(address >= 0x0000 && address <= 0x3fff) {
		// 16KB ROM banks
		return this->crt->read(address);
	}
	else if(address >= 0x8000 && address <= 0x9FFF) {
		;// 8KB VRAM bank 0 .. 1
	}
	else if(address >= 0xA000 && address <= 0xBFFF) {
		// 8KB External RAM
		return this->crt->read(address); 
	}
	else if(address >= 0xC000 && address <= 0xCFFF) {
		// 4KB WRAM bank 0 
		address -= 0xC000;
		return *(wram + address);
	}
	else if(address >= 0xD000 && address <= 0xDFFF) {
		// 4KB WRAM bank 1 .. n
		address -= 0xD000;
		return *(wram + locFF70*8*1024 + address);
	}
	else if(address >= 0xE000 && address <= 0xFDFF) {
		;// ECHO thing 
	}
	else if(address >= 0xFE00 && address <= 0xFE9F) {
		;// Sprite attribute table (OEM)
	}
	else if(address >= 0xFEA0 && address <= 0xFEFE) {
		;// Not Usable
	}
	else if(address >= 0xFF00 && address <= 0xFF7F) {
		//I/O
		if(address == 0xFF00)
			return this->j->read(address);
		else if(address >= 0xFF01 && address <= 0xFF02)
			;// serial com
		else if(address >= 0xFF04 && address <= 0xFF07)
			;// time and divider register
		else if(address == 0xFF0F) {
			// interrupt flag 
			return INTFLAG;
		}
		else if(address >= 0xFF10 && address <= 0xFF3F)
			;// sound 
		else if(address >= 0xFF40 && address <= 0xFF4B || \
			address >= 0xFF4F && address <= 0xFF55 ||     \
			address >= 0xFF68 && address <= 0xFF6B || address == 0xFF6F)
			;// lcd
		else if(address == 0xFF4D) {
			// speed 
			uint8_t readmask = 0b10000001;
			return locFF4D & readmask;
		}
		else if(address == 0xFF56) {
			// infrared comm.
			uint8_t readmask = 0b11000011;
			return locFF56 & readmask;	
		}
		else if(address == 0xFF6C) {
			//unknown
			uint8_t data = *(locUKNS+0);
			return data;
		}
		else if(address == 0xFF70) {
			// ram bank number
			uint8_t readmask = 0b00000111;
			return locFF70 & readmask;
		}
		else if(address >= 0xFF72 && address <= 0xFF77) {
			// unknown
			address -= 0xFF72;
			uint8_t data = *(locUKNS + 1 + address);
			return data;
		}
		else {
			ERROR("unrouted read address at MMU");
			exit(1);
		}
	}
	else if(address >= 0xFF80 && address <= 0xFFFE) {
		// HRAM
	}
	else if(address == 0xFFFF) {
		return INTENBL;
	}
	else {
		ERROR("Invalid read address at MMU");
		exit(0);
	}
}

int mmu::write8(uint16_t address, uint8_t data) {

	// all writeable locations

	if(address >= 0x0000 && address <= 0x7fff) {
		// toggle ram rtc, set rom bank, set external ram bank, latch clock
		return this->crt->write(address, data);
	}
	else if(address >= 0x8000 && address <= 0x9FFF) {
		;// 8KB VRAM bank 0 .. 1
	}
	else if(address >= 0xA000 && address <= 0xBFFF) {
		// 8KB External RAM
		return this->crt->write(address, data); 
	}
	else if(address >= 0xC000 && address <= 0xCFFF) {
		// 4KB WRAM bank 0 
		address -= 0xC000;
		*(wram + address) = data;
		return 0;
	}
	else if(address >= 0xD000 && address <= 0xDFFF) {
		// 4KB WRAM bank 1 .. n
		address -= 0xD000;
		*(wram + locFF70*8*1024 + address) = data;
		return 0;
	}
	else if(address >= 0xE000 && address <= 0xFDFF) {
		;// ECHO thing 
	}
	else if(address >= 0xFE00 && address <= 0xFE9F) {
		;// Sprite attribute table (OEM)
	}
	else if(address >= 0xFEA0 && address <= 0xFEFE) {
		;// Not Usable
	}
	else if(address >= 0xFF00 && address <= 0xFF7F) {
		//I/O
		if(address == 0xFF00)
			return this->j->write(address, data);
		else if(address >= 0xFF01 && address <= 0xFF02)
			;// serial com
		else if(address >= 0xFF04 && address <= 0xFF07)
			;// time and divider register
		else if(address == 0xFF0F) {
			// interrupt flag 
			INTFLAG = data;
			return 0;
		}
		else if(address >= 0xFF10 && address <= 0xFF3F)
			;// sound 
		else if(address >= 0xFF40 && address <= 0xFF4B || \
			address >= 0xFF4F && address <= 0xFF55 ||     \
			address >= 0xFF68 && address <= 0xFF6B || address == 0xFF6F)
			;// lcd
		else if(address == 0xFF4D) {
			//speed
			uint8_t writemask = 0b00000001;
			locFF4D = (locFF4D & ~writemask) | (data & writemask);
			return 0;
		}
		else if(address == 0xFF56) {
			//infrared
			uint8_t writemask = 0b11000001;
			locFF56 = (locFF56 & ~writemask) | (data & writemask);
			return 0;
		}
		else if(address == 0xFF6C) {
 			// unknown
 			locUKNS[0] = data;
 			return 0;
		}
		else if(address == 0xFF70) {
			// ram bank
			uint8_t writemask = 0b00000111;
			if(data & writemask == 0x00)
				locFF4D = 0x01;
			else
				locFF4D = (data & writemask);
			return 0;
		}
		else if(address >= 0xFF72 && address <= 0xFF77) {
			// unknown
			locUKNS[address + 1 - 0xFF72] = data;
			return 0;
		} 
		else {
			ERROR("unrouted write address at MMU/IO");
			exit(1);
		}

	}
	else if(address >= 0xFF80 && address <= 0xFFFE) {
		;// HRAM
	}
	else if(address == 0xFFFF) {
		INTENBL = data;
		return 0;
	}
	else {
		ERROR("Invalid write address at MMU");
		exit(1); 
	}
}

uint16_t mmu::read16(uint16_t address) {
	uint8_t data1 = this->read8(address);
	uint8_t data2 = this->read8(address+1);
	uint16_t data;
	memcpy(&data, &data2, 1);
	memcpy(&data + 1, &data1, 1);
	return data;
}

int mmu::write16(uint16_t address, uint16_t data) {
	uint8_t data1, data2;
	memcpy(&data1, &data + 1, 1);
	memcpy(&data2, &data, 1);
	if(!this->write8(address, data1)) {
		return this->write8(address + 1, data2);
	}
	return 1;
}


void mem_seeks(mmu * m) {

	printf("hello1 !\n");

	int address, st;
	
	while((st = scanf("%x", &address))) {
		uint8_t data = m->read8(address);
		printf("querying for loc 0x%x : %x\n", address, data);
		
	}


}
