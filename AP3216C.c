#include "AP3216C.h"
#include "i2cmaster.h"
#include "platform.h"

void ap3216c_init(void) 
{
    // 1. Reset the device (optional but recommended)
    AP3216C_WriteByte(REG_SYSTEM_CONG, 0x04); 
    // Wait for reset (approx 10ms)
    PLATFORM_DelayMS(50);
	
	// 2. Configure PS Gain (Register 0x10)
    // Bits [1:0] = 11 (8X Gain)
    // Bits [3:2] = 11 (Highest Pulse Count for max range)
    // Binary: 0000 1111 -> Hex: 0x0F
    AP3216C_WriteByte(0x20, 0x0F); 

    // 3. Set PS Mean Time (Register 0x11) - Optional
    // Controls the measurement frequency. 0x00 is the fastest.
    AP3216C_WriteByte(0x23, 0x00);
	
    // 4. Enable Proximity Sensor (PS) and Ambient Light Sensor (ALS)
    // 0x03 enables both. Use 0x01 for ALS only or 0x02 for PS only.
    AP3216C_WriteByte(REG_SYSTEM_CONG, 0x02);
}

uint16_t ap3216c_read_proximity(void) 
{
    uint8_t low, high;
    uint16_t ps_data;

    // Read low byte first
    low = AP3216C_ReadByte(REG_PS_DATA_L);
    // Read high byte
    high = AP3216C_ReadByte(REG_PS_DATA_H);
	
    /* Note: In the AP3216C, the PS data is 10-bit or 14-bit 
       depending on configuration. Usually, it's:
       low:  [bit 0: IR_unstable, bit 1: Object_detect, bits 4-7: data]
       high: [bits 0-5: data]
    */
    
    // Check if data is valid (Object Detect bit 1)
    // Simple raw data assembly:
    ps_data = ((uint16_t)(high & 0x3F) << 4) | (low & 0x0F);

    return ps_data;
}


//-------------------------------
// Write data to 8 bit register to AP3216C 
//-------------------------------
void AP3216C_WriteByte(uint8_t reg, uint8_t value) 
{
	I2C1_Start();							// Start condition
	Write_Byte_I2C1(AP3216C_ADDR);			// Write device address
	Write_Byte_I2C1(reg);					// Write address of register
	Write_Byte_I2C1(value);					// DATA_L
	I2C1_Stop();              				// Send a STOP condition on the TWI bus.
}

//-------------------------------
// Read 16-bit register value from AP3216C 
//-------------------------------
uint8_t AP3216C_ReadByte(uint8_t addr)
{
   	uint8_t Data=0;
	
	I2C1_Start();				// Start condition
	Write_Byte_I2C1(AP3216C_ADDR);			// Write device address
	Write_Byte_I2C1(addr);			// Write address of register
	I2C1_Start();				// Start condition
	Write_Byte_I2C1(AP3216C_ADDR+1);			// Write device address
	Data = Read_Byte_I2C1(NO_ACK);
	I2C1_Stop();              // Send a STOP condition on the TWI bus.		
	
	return Data;
}

