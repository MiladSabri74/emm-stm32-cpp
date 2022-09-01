/**
  ******************************************************************************
  * @file           Memory.hpp
  * @brief          Memory header
  ******************************************************************************
  * @attention
  *
  *
  * @author         Milad Sabri
  * @date           Jan-2022
  ******************************************************************************
  */
/* Header definition ---------------------------------------------------------*/

#ifndef __MEMORY_HPP
#define __MEMORY_HPP
/** @defgroup Memory Memory
  * @brief Memory management base on AT24C512 eeprom IC with HAL library
  * @{
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "Queue.hpp"
#include <math.h>
/* Private macro -------------------------------------------------------------*/
/** @defgroup Memory_Private_Constants
  * @brief this constants defined base on AT24C512 you can changed them 
	with your desired eeprom IC 
  * @{
  */
#define EEPROM_ADDRESS          0xA0
#define PAGE_SIZE               0x80    // Per byte
#define PAGE_NUM                0x200   // Number of pages
#define EEPROM_I2C				&hi2c1
/**
  * @}
  */
/* Private structure ---------------------------------------------------------*/
/** @defgroup Memory_Types Memory Types
  * @{
  */
/**
  * @brief  Memory structure of parameters of data that you want to stored in EEPROM
  */
struct WriteRequestParameters
{
	uint16_t address;
	uint16_t size;
};
/**
  * @}
  */
/* Class definition ----------------------------------------------------------*/
/**
 *  @class Memory Class
	@brief Define Functions and private variables which needed
 */
class Memory
{
	public:
		// Constructor ---------------------------------------
		Memory(); 
		// Management ----------------------------------------
		void writeManagement(); 
		// Requests ------------------------------------------
		void writeRequest(uint16_t page,uint16_t offset,uint8_t data[],uint16_t size);
		void writeRequest(uint16_t address, uint8_t data[],uint16_t size);
		void erasePageRequest(uint16_t page);
		// Read ----------------------------------------------
		void read(uint16_t page,uint16_t offset,uint8_t *data,uint16_t size);
		void read(uint16_t address, uint8_t *data,uint16_t size);
		uint8_t writeEnable;
	private:
		// Writes --------------------------------------------
		void write(uint16_t addr, uint8_t *data, uint16_t size);
		// Essentials functions ------------------------------
		uint16_t bytesToWrite(uint16_t size, uint16_t offset);
		// Variables -----------------------------------------
	  std::vector<WriteRequestParameters> writeRequests;
		Queue	writeBuffer;
};
/**
  * @}
  */
/**
  * @}
  */
  
#endif // __MEMORY_HPP
