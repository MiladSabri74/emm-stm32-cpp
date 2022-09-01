/**
  ******************************************************************************
  * @file           Memory.cpp
  * @brief          Memory class for read/write data from EEPROM IC
  ******************************************************************************
  * @attention
  *
  *
  * @author         Milad Sabri
  * @date           Jan-2022
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "Memory.hpp"

/* Extern Variables ----------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;

/* Constructor ---------------------------------------------------------------*/
/**
* @brief Construtor of Memory class
*/
Memory::Memory(){
	writeEnable = 1;
}

/* Management ----------------------------------------------------------------*/

/**
* @brief Management of write request in EEPROM IC 
		 you should call it in main while of project to manage write requests
		 of other classes
* @param None
* @retval None
*/
void Memory::writeManagement(){
	if(writeEnable == 0)
		return;
	if(writeRequests.empty())
		return;
	uint16_t addr = writeRequests.front().address;
	uint16_t size = writeRequests.front().size;
	uint8_t data[256];
	writeBuffer.removeData(data,size);
	write(addr,data,size);
	writeRequests.erase(writeRequests.begin());
}

/* Requests ------------------------------------------------------------------*/

/**
* @brief Request to Write in EEPROM IC
* @param page page of EEPROM 
* @param offset offset of address
* @param data[] data which should be written into EEPROM
* @param size size of data
* @retval None 
*/
void Memory::writeRequest(uint16_t page,uint16_t offset, uint8_t data[],uint16_t size){
	WriteRequestParameters request;
	request.address = (page * PAGE_SIZE) + offset;
	request.size = size;
	writeBuffer.addData(data,size);
	writeRequests.push_back(request);
}

/**
* @brief Request to Write in EEPROM IC
* @param address address of EEPROM
* @param data[] data which should be written into EEPROM
* @param size size of data
* @retval None
*/
void Memory::writeRequest(uint16_t address, uint8_t data[],uint16_t size){
	WriteRequestParameters request;
	request.address = address;
	request.size = size;
	writeBuffer.addData(data,size);
	writeRequests.push_back(request);
}

/**
* @brief Erase selected page of EEPROM IC
* @param page page of EEPROM
* @retval None
*/
void Memory::erasePageRequest(uint16_t page){
	WriteRequestParameters request;
	request.address = page * PAGE_SIZE;
	request.size = PAGE_SIZE;
	uint8_t data[PAGE_SIZE];
	memset(data,0xFF,PAGE_SIZE);
	writeBuffer.addData(data,PAGE_SIZE);
	writeRequests.push_back(request);
}

/* Read ----------------------------------------------------------------------*/
/**
* @brief Read from EEPROM
* @param page page of EEPROM 
* @param offset offset of address
* @param data data that read from EEPROM
* @param size size of data
* @retval None
*/
void Memory::read(uint16_t page,uint16_t offset,uint8_t *data,uint16_t size){
	
	int paddrPosition = log((double)PAGE_SIZE)/log(2.0);
	uint16_t startPage = page;
	uint16_t endPage = page + ((size+offset)/PAGE_SIZE);
	uint16_t numOfPages = (endPage-startPage) + 1;
	uint16_t pos = 0;

	for (int i=0; i < numOfPages; i++){
		uint16_t memAddress = startPage << paddrPosition | offset;
		uint16_t bytesRemaining = bytesToWrite(size, offset);
		HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDRESS, memAddress,
                     2, &data[pos], bytesRemaining, 1000);
		startPage += 1;
		offset=0;
		size = size-bytesRemaining;
		pos += bytesRemaining;
	}
}
/**
* @brief Read from EEPROM
* @param address address of EEPROM
* @param data data that read from EEPROM 
* @param size szie of data
* @retval None 
*/
void Memory::read(uint16_t address, uint8_t *data,uint16_t size){
	
	uint16_t page = address / PAGE_SIZE;
	uint16_t offset = address % PAGE_SIZE;
	int paddrPosition = log((double)PAGE_SIZE)/log(2.0);
	uint16_t startPage = page;
	uint16_t endPage = page + ((size+offset)/PAGE_SIZE);
	uint16_t numOfPages = (endPage-startPage) + 1;
	uint16_t pos = 0;

	for (int i=0; i < numOfPages; i++){
		uint16_t memAddress = startPage << paddrPosition | offset;
		uint16_t bytesRemaining = bytesToWrite(size, offset);
		HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDRESS, 
                     memAddress, 2, &data[pos], bytesRemaining, 1000);
		startPage += 1;
		offset=0;
		size = size-bytesRemaining;
		pos += bytesRemaining;
	}
}
/* Writes --------------------------------------------------------------------*/
/**
* @brief Write to EEPROM IC
* @param address address of EEPROM
  @param data data that write to EEPROM
  @param size size of data
* @retval None
*/
void Memory::write(uint16_t addr,uint8_t *data, uint16_t size){
	uint16_t page = addr / PAGE_SIZE;
	uint16_t offset = addr % PAGE_SIZE;
	// Find out the number of bit, where the page addressing starts
	int paddrposition = log((double)PAGE_SIZE)/log(2.0);

	// calculate the start page and the end page
	uint16_t startPage = page;
	uint16_t endPage = page + ((size+offset)/PAGE_SIZE);

	// number of pages to be written
	uint16_t numofpages = (endPage-startPage) + 1;
	uint16_t pos=0;

	// write the data
	for (int i=0; i<numofpages; i++)
	{
		/* calculate the address of the memory location
		 * Here we add the page address with the byte address
		 */
		uint16_t MemAddress = startPage<<paddrposition | offset;
		 // calculate the remaining bytes to be written
		uint16_t bytesremaining = bytesToWrite(size, offset); 
		// write the data to the EEPROM
		HAL_I2C_Mem_Write(EEPROM_I2C,EEPROM_ADDRESS,
                      MemAddress,2,&data[pos],bytesremaining,2000);  
		// increment the page, so that a new page address can be selected for further write
		startPage += 1; 
		// since we will be writing to a new page, so offset will be 0		
		offset=0;  
		// reduce the size of the bytes
		size = size-bytesremaining;
		// update the position for the data buffer
		pos += bytesremaining;  
		// Write cycle delay (5ms)
		HAL_Delay (5);  
	}
}
/* Essentials functions ------------------------------------------------------*/
/**
* @brief Calculate number of bytes that can be write
* @param size size of data
  @param offset offset of address
* @retval return number of bytes 
*/
uint16_t Memory::bytesToWrite(uint16_t size, uint16_t offset){
	if ((size+offset)<PAGE_SIZE) return size;
	else return PAGE_SIZE-offset;
}
