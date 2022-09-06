/**
  ******************************************************************************
  * @file           : Queue.hpp
  * @brief          : Queue header
  ******************************************************************************
  * @attention
  *
  *
  * @author         : Milad Sabri
  * @company        : Pardisan-CO
  * @date           : Jan-2022
  ******************************************************************************
  */
/* Header definition ---------------------------------------------------------*/
#ifndef __QUEUE_HPP
#define __QUEUE_HPP
/** @defgroup Queue Queue
  * @brief Queue to store data based on CPP vector library
  * @{
  */
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
/* Private macro -------------------------------------------------------------*/


/* Class definition ----------------------------------------------------------*/
/**
 *  @class Queue Class
	@brief Define Functions and private variables which needed
 */
class Queue
{
	public:
		// Constructor ---------------------------------------
		Queue();
		// Modifiers -----------------------------------------
		void add(uint8_t data);
		void addData(uint8_t *data,uint16_t size);
		void eraseFront();
		void erase(uint16_t len);
		bool remove(uint8_t &data);
		bool removeData(uint8_t *data, uint16_t len);
		// Capacity ------------------------------------------
		bool isEmpty();
		void flush();
		// Search --------------------------------------------
		uint8_t front();
		bool find(uint8_t item,uint16_t &loc);
		bool find(const char data[],uint16_t &loc);
		// Variables -----------------------------------------
		std::vector<uint8_t> buffer;
	private:
		
};

#endif // __QUEUE_HPP
