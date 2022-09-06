/**
  ******************************************************************************
  * @file           : Queue.cpp
  * @brief          : Queue class for buffering data
  ******************************************************************************
  * @attention
  *
  *
  * @author         : Milad Sabri 
  * @date           : Jan-2022
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "Queue.hpp"

/* Constructor ---------------------------------------------------------------*/
/**
* @brief Construtor of Queue class
*/
Queue::Queue(){

}
/* Modifiers -----------------------------------------------------------------*/
/**
* @brief Add element at the end of queue
* @param data
*/
void Queue::add(uint8_t data){
	this->buffer.push_back(data);
}
/**
* @brief Add elements at the end of queue
* @param data* data to add to Queue
* @param size size of data 
*/
void Queue::addData(uint8_t *data,uint16_t size){
	uint16_t iterator;
	for(iterator = 0 ; iterator < size ; iterator++)
		this->buffer.push_back(data[iterator]);
}

/**
* @brief Erase front element of queue
* @param None
* @retval None
*/
void Queue::eraseFront(){
	buffer.erase(buffer.begin());
}

/**
* @brief Erase elements of queue
* @param None
* @retval None
*/
void Queue::erase(uint16_t len){
	buffer.erase(buffer.begin(),buffer.begin()+len);
}
/**
* @brief Extract element at the first of queue
* @param data  removed data to pass other functions
* @retval Return correction of this operation
*/
bool Queue::remove(uint8_t &data){
	data = buffer.front();
	buffer.erase(buffer.begin());
}
/**
* @brief Extract elements at the first of queue
* @param Data to pass other functions 
* @param len length of data  
* @retval Return correction of this operation
*/
bool Queue::removeData(uint8_t *data, uint16_t len){
	uint16_t iterator;
	if(buffer.size() < len)
		return false;
	for(iterator = 0 ; iterator < len ; iterator++){
			data[iterator] =  *(buffer.begin() +iterator);
	}
	buffer.erase(buffer.begin(),buffer.begin()+len);
	return true;
}
/* Capacity ------------------------------------------------------------------*/
/**
* @brief Check queue is empty or not
* @param None
* @retval  Boolean: if queue is empty return true ,else return false
*/
bool Queue::isEmpty(){
 return this->buffer.empty();
}
/**
* @brief Clear queue
* @param None
* @retval None
*/
void Queue::flush(){
	buffer.clear();
}
/* Search --------------------------------------------------------------------*/
/**
* @brief show front data in queue
* @param None
* @retval Return front of queue
*/
uint8_t Queue::front()
{
	return this->buffer.front();
}

/**
* @brief Find first element in queue that is equal to item
* @param item  item which want to find 
* @param loc loc is the location in queue
* @retval Return correction of this operation
*/
bool Queue::find(uint8_t item,uint16_t &loc){
	uint16_t iterator;
	for(iterator = 0 ; iterator < buffer.size() ; iterator++)
		if( buffer[iterator] == item )
		{
			loc  = iterator;
			return true;
		}
	return false;
}

/**
* @brief Find first chain of elements in queue that is equal to data array
* @param data[] data which want to find 
* @param loc loc is the location of first element in queue
* @retval Return correction of this operation
*/
bool Queue::find(const char data[],uint16_t &loc){
	uint16_t iterator,index = 0;
	uint16_t len = strlen(data);
	for(iterator = 0 ; iterator < buffer.size() ; iterator++){
		if( buffer[iterator] == data[index] ){
			if(index == len-1)
				return true;
			if(index == 0)
				loc = iterator+len;
			index++;		
		}
		else{
			index = 0;
		}
	}
	return false;
}
