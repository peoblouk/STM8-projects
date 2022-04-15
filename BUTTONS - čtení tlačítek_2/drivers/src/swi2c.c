/*
 * swi2c.c
 *
 * Created: 03.08.2021 21:49:31
 *  Author: mdudk
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include "swi2c.h"

#ifndef F_CPU
#error "swi2c needs defined F_CPU"
#endif

#define RESET 0

// read num bytes to array data from slave with desired I2C slv_addres, from desired pointer (address)
// Generates I2C sequence SLA+W + 1Byte (Address) + RST + SLA+R + num*Byte (data)
// returns 0xaa if bus is busy too long (function cant start I2C transfer)
// returns 0xff if bus error (blocked bus)
// returns 0x01 if slave not present (NACK)
// returns 0x00 if success
// slave address in 8bit representation (left aligned 7bit value)
uint8_t swi2c_read_buf(uint8_t slv_addr, uint8_t address, uint8_t* data, uint16_t num){
	uint8_t i=0,bit;
	uint8_t ack;
	uint8_t mask;

	// --- Generate START ---
	if(swi2c_START()){return 0xaa;}

	// --- SLA+W ---
	mask=0b1<<7;
	while(mask){
		if(swi2c_writebit(slv_addr & mask)){return 0xff;}
		mask = mask >>1;
	}
	ack=swi2c_readbit();
	if(ack){
		if(swi2c_STOP()){return 0xff;}
		return ack;
	}

	// --- Data address (pointer in slave) ---
	mask=0b1<<7;
	while(mask){
		if(swi2c_writebit(address & mask)){return 0xff;}
		mask = mask >>1;
	}
	ack=swi2c_readbit();
	if(ack){
		if(swi2c_STOP()){return 0xff;}
		return ack;
	}
	
	// --- Generate RESTART ---
	if(swi2c_RESTART()){return 0xff;}

	// --- SLA+R ---
	mask=0b1<<7;
	while(mask){
		if(swi2c_writebit((slv_addr | 0b1) & mask)){return 0xff;}
		mask = mask >>1;
	}
	ack=swi2c_readbit();
	if(ack){
		if(swi2c_STOP()){return 0xff;}
		return ack;
	}


	// --- Data ---
	for(i=0;i<num;i++){
		mask=0b1<<7;
		while(mask){
			bit = swi2c_readbit();
			if(bit==0){data[i] &=~mask;}
			else if(bit==1){data[i] |=mask;}
			else{swi2c_STOP();return 0xff;}
			mask = mask >>1;
		}
		if((i+1)==num){
			if(swi2c_writebit(1)){return 0xff;} // NACK
			}else{
			if(swi2c_writebit(0)){return 0xff;} // ACK
		}
		
	}
	// --- STOP ---
	if(swi2c_STOP()){return 0xff;}
	return 0;
}


// send num bytes from array data to slave with desired I2C slv_addres, to desired pointer (address)
// Generates I2C sequence SLA+W + 1Byte (Address) + num*Byte (data)
// returns 0xaa if bus is busy too long (function cant start I2C transfer)
// returns 0xff if bus error (blocked bus)
// returns 0x01 if slave not present (NACK)
// returns 0x00 if success
// slave address in 8bit representation (left aligned 7bit value)
uint8_t swi2c_write_buf(uint8_t slv_addr, uint8_t address, uint8_t* data, uint16_t num){
	uint8_t i;
	uint8_t ack;
	uint8_t mask;

	// --- Generate START ---
	if(swi2c_START()){return 0xaa;}

	// --- SLA+W ---
	mask=0b1<<7;
	while(mask){
		if(swi2c_writebit(slv_addr & mask)){return 0xff;}
		mask = mask >>1;
	}
	ack=swi2c_readbit();
	if(ack){
		if(swi2c_STOP()){return 0xff;}
		return ack;
	}

	// --- Data address (pointer in slave) ---
	mask=0b1<<7;
	while(mask){
		if(swi2c_writebit(address & mask)){return 0xff;}
		mask = mask >>1;
	}
	ack=swi2c_readbit();
	if(ack){
		if(swi2c_STOP()){return 0xff;}
		return ack;
	}

	// --- Data ---
	for(i=0;i<num;i++){
		mask=0b1<<7;
		while(mask){
			if(swi2c_writebit(data[i] & mask)){return 0xff;}
			mask = mask >>1;
		}
		ack=swi2c_readbit();
		if(ack){
			if(swi2c_STOP()){return 0xff;}
			return ack;
		}
	}

	// --- STOP ---
	if(swi2c_STOP()){return 0xff;}
	return 0;
}



// test (ACK/NACK) of selected I2C slave address (like a ping - testing slave present on bus)
// slave address in 8bit representation (left aligned 7bit value)
// returns 0 if is slave present on bus
// returns 1 if slave is not present on bus
// returns 0xff if timeout error
// returns 0xaa if busy bus
uint8_t swi2c_test_slave(uint8_t slvaddr){
	uint8_t ack;
	uint8_t mask=0b1<<7;
	if(swi2c_START()){return 0xaa;}
	while(mask){
		if(swi2c_writebit(slvaddr & mask)){return 0xff;}
		mask = mask >>1;
	}
	ack=swi2c_readbit();
	if(swi2c_STOP()){return 0xff;}
	return ack;
}

void swi2c_init(void){
	SCL_HIGH;
	SDA_HIGH;
	SWI2C_SCL_PORT.OUTCLR = SWI2C_SCL_PIN;
	SWI2C_SDA_PORT.OUTCLR = SWI2C_SDA_PIN;
}

// --- Private functions ---

// read bit value from bus
// returns 0 if read 0
// returns 1 if read 1
// returns 0xff if timeout condition
uint8_t swi2c_readbit(void){
	uint8_t timeout=SWI2C_TIMEOUT;
	uint8_t retval;
	SDA_HIGH; // release SDA
	SWI2C_READ_DEL;
	SCL_HIGH;
	while(SCL_stat() == RESET && timeout){timeout--;}
	if(timeout==0){return 0xff;}
	SWI2C_THIGH_DEL;
	if(SDA_stat() == RESET){retval = 0;}else{retval=1;}
	SCL_LOW;
	SWI2C_THDDAT_DEL;
	return retval;
}

// write one bit on bus
// returns 0xff if SCL line blocked (timeout)
// returns 0 if success
uint8_t swi2c_writebit(uint8_t bit){
	uint8_t timeout=SWI2C_TIMEOUT;
	SWI2C_TIDLE_DEL;
	if(bit){SDA_HIGH;}else{SDA_LOW;} // set desired SDA value
	SWI2C_TSUDAT_DEL; 
	SCL_HIGH;
	while(SCL_stat() == RESET && timeout){timeout--;} // wait until SCL is not high
	if(timeout==0){SDA_HIGH; return 0xff;} // generate timeout error if SCL is held Low too long
	SWI2C_THIGH_DEL;
	SCL_LOW;
	SWI2C_THDDAT_DEL; 
	return 0;
}

// generate RESTART bit
// returns 1 if bus is busy (SDA or SCL is Low)
// return 0 if success
uint8_t swi2c_RESTART(void){
	uint8_t timeout=SWI2C_TIMEOUT;
	SWI2C_TIDLE_DEL;
	SDA_HIGH;
	while(SDA_stat() == RESET && timeout){timeout--;}
	if(timeout==0){SCL_HIGH; return 0xff;}
	SWI2C_TSUDAT_DEL;
	SCL_HIGH;
	while(SCL_stat() == RESET && timeout){timeout--;}
	if(timeout==0){return 0xff;}
	SWI2C_TSUSTA_DEL;
	SDA_LOW;
	SWI2C_THDSTA_DEL;
	SCL_LOW;
	SWI2C_THDDAT_DEL;	
	return 0;
}

// generate START bit
// returns 0xff if bus is busy (SDA or SCL is Low)
// return 0 if success
uint8_t swi2c_START(void){
	uint8_t timeout=SWI2C_TIMEOUT;
	while((SCL_stat() == RESET || SDA_stat() == RESET) && timeout){timeout--;}
	if(timeout == 0){return 0xff;}
	SWI2C_TSUSTA_DEL;	
	SDA_LOW;
	SWI2C_THDSTA_DEL;
	SCL_LOW;
	SWI2C_THDDAT_DEL;
	return 0;
}

// generate STOP bit
// return 0xff if timeout error (SCL is held low too long)
// return 0 if success
uint8_t swi2c_STOP(void){
	uint8_t timeout=SWI2C_TIMEOUT;
	uint8_t retval = 0;
	SWI2C_TIDLE_DEL;
	SDA_LOW;
	SWI2C_TSUDAT_DEL;
	SCL_HIGH;
	while(SCL_stat() == RESET && timeout){timeout--;}
	if(timeout==0){retval = 0xff;}
	SWI2C_TSUSTO_DEL;
	SDA_HIGH;
	SWI2C_TBUF_DEL;
	return retval;
}

// Try to recover bus from failure
// returns 0 if bus is free (success)
// returns 0xff if SCL is hold low too long
// returns 0xee if SDA is still held low
uint8_t swi2c_recover(void){
	uint8_t timeout=SWI2C_TIMEOUT;
	uint8_t i;
	SCL_HIGH; // release both lines
	SDA_HIGH;
	SWI2C_RECOVERY_DEL;
	// if both lines are High => everything OK, Bus is free
	if(SCL_stat() != RESET && SDA_stat() != RESET){return 0;}
	// if some slave holds SDA in LOW
	if(SDA_stat() == RESET){
		for(i=0;i<9;i++){ // try nine times try to read one bit and pray for SDA release
			SCL_LOW;
			SWI2C_RECOVERY_DEL;
			SCL_HIGH;
			while(SCL_stat() == RESET && timeout){timeout--;}
			if(timeout==0){return 0xff;}
			SWI2C_RECOVERY_DEL;
			if(SDA_stat() != RESET){ // if slave released SDA line, generate STOP
				return(swi2c_STOP());
			}
		}
		return 0xee;
	}
	// if somebody holds SCL we are out ...
	return 0xff;
}
