/*
 * swi2c.h
 *
 * Created: 03.08.2021 21:48:42
 *  Author: mdudk
 
 Bit-bang variant of I2C single Master library. 
 Using:
 1. Select used GPIOs by MACRO
 2. Select speed by MACRO
 3. Call swi2c_init() before calling any swi2c functions
 4. Call desired functions
 
 Prerequisites:
 Library use _delay_us() to timing. util/delay.h need defined F_CPU. In Atmel studio go to Project->Properties->Toolchain->AVR/GNU C Compiler->Symbols->Defined Symbols
 and write F_CPU=xxxxxx (your used frequency)
 
 Remarks:
 Librabry is interruptible and itself do not use interrupts.
 Timing is not precise and usualy you do not need precise communication frequency.
 */ 


#ifndef SWI2C_H_
#define SWI2C_H_

// ---------------------------- PART TO MODIFY -------------------------------------
// select used GPIOs
#define SWI2C_SCL_PORT PORTA
#define SWI2C_SCL_PIN	PIN6_bm
#define SWI2C_SDA_PORT PORTA
#define SWI2C_SDA_PIN	PIN5_bm

// Select communication speed (uncomment only one variant)
//#define SWI2C_STANDARD_MODE_SPEED									// strict timing for "Standard mode", real speed about 70-100kHz	
#define SWI2C_FAST_MODE_SPEED										  // strict timing for "Fast mode", real speed about 130-200kHz
//#define SWI2C_EMPIRICAL_TIMING_FAST_MODE_16MHZ_O1 // manually optimise timing for maximum speed with 16MHz F_CPU and -O1 optimisation

// communication speed definitions (times), all in microseconds
#ifdef SWI2C_STANDARD_MODE_SPEED
// "standard mode (<100kHz)"
#define TSUSTA 4.7 // Setup time for a repeated Start condition
#define THDSTA 4.0 // Hold time (repeated) Start condition
#define TLOW	4.7 // Low period of SCL Clock
#define THIGH	4.0 // High period of SCL Clock
#define THDDAT 0.3 // Data hold time to bridge undefined SCL low falling region
#define TSUDAT 0.25 // Data setup time
#define TSUSTO 4 // Setup time for Stop condition
#define TBUF 4.7 // Bus free time between a Stop and Start condition
#define TRECOVER 5 // hold and low and high time for bus recovery

#elif defined SWI2C_FAST_MODE_SPEED
// "Fast mode (<400kHz)"
#define TSUSTA 0.6 // Setup time for a repeated Start condition
#define THDSTA 0.6 // Hold time (repeated) Start condition
#define TLOW	1.3 // Low period of SCL Clock
#define THIGH	0.6 // High period of SCL Clock
#define THDDAT 0.3 // Data hold time to bridge undefined SCL low falling region
#define TSUDAT 0.1 // Data setup time
#define TSUSTO 0.6 // Setup time for Stop condition
#define TBUF 1.3 // Bus free time between a Stop and Start condition
#define TRECOVER 1.5 // hold and low and high time for bus recovery

#elif defined SWI2C_EMPIRICAL_TIMING_FAST_MODE_16MHZ_O1
// "Fast mode (<400kHz), adjusted to maximum speed with 16MHz and -O1 optimisation"
#define TSUSTA 0.6 // Setup time for a repeated Start condition
#define THDSTA 0.6 // Hold time (repeated) Start condition
#define TLOW	0 // Low period of SCL Clock
#define THIGH	0.0 // High period of SCL Clock
#define THDDAT 0.0 // Data hold time to bridge undefined SCL low falling region
#define TSUDAT 0.0 // Data setup time
#define TSUSTO 0.6 // Setup time for Stop condition
#define TBUF 1.3 // Bus free time between a Stop and Start condition
#define TRECOVER 1.5 // hold and low and high time for bus recovery

#else
#error "Define I2C Speed"
#endif

#define SWI2C_TIMEOUT 0xff // maximum number of waiting cycles

// ---------------------------- DO NOT MODIFY -------------------------------------

#define SCL_HIGH SWI2C_SCL_PORT.DIRCLR = SWI2C_SCL_PIN
#define SCL_LOW SWI2C_SCL_PORT.DIRSET = SWI2C_SCL_PIN
#define SDA_HIGH SWI2C_SDA_PORT.DIRCLR = SWI2C_SDA_PIN
#define SDA_LOW SWI2C_SDA_PORT.DIRSET = SWI2C_SDA_PIN

#define SWI2C_TSUSTA_DEL _delay_us(TSUSTA)
#define SWI2C_THDSTA_DEL _delay_us(THDSTA)
#define SWI2C_TSUSTO_DEL _delay_us(TSUSTO)
#define SWI2C_THDDAT_DEL _delay_us(THDDAT)
#define SWI2C_TSUDAT_DEL _delay_us(TSUDAT)
#define SWI2C_TIDLE_DEL _delay_us(TLOW-TSUDAT)
#define SWI2C_READ_DEL _delay_us(TLOW)
#define SWI2C_THIGH_DEL _delay_us(THIGH)
#define SWI2C_TBUF_DEL _delay_us(TBUF)
#define SWI2C_RECOVERY_DEL _delay_us(TRECOVER)

#define SCL_stat() (SWI2C_SCL_PORT.IN & SWI2C_SCL_PIN)
#define SDA_stat() (SWI2C_SDA_PORT.IN & SWI2C_SDA_PIN)

// user functions
void swi2c_init(void); // init GPIOs as Open Drain outputs (optionaly adjust output values to zero - for easy open drain manipulation - platform dependent)
uint8_t swi2c_test_slave(uint8_t slvaddr); // check if slave with selected adres is present on bus
uint8_t swi2c_write_buf(uint8_t slv_addr, uint8_t address, uint8_t* data, uint16_t num); // SLA+W - 1B(Address) - num*1B(data)
uint8_t swi2c_read_buf(uint8_t slv_addr, uint8_t address, uint8_t* data, uint16_t num); // SLA+W - 1B(Address) - RST - SLA+R + num*1B(data)
uint8_t swi2c_recover(void);

// private functions
uint8_t swi2c_writebit(uint8_t bit);
uint8_t swi2c_readbit(void);
uint8_t swi2c_START(void);
uint8_t swi2c_RESTART(void);
uint8_t swi2c_STOP(void);

#endif /* SWI2C_H_ */