/*
 * log.h
 *
 * This file define timeout values in debug and production mode
 * as well as abstraction of printf in debug mode
 *  Created on: Mar 6, 2021
 *      Author: Saloni Shah
 */
#ifndef LOG_H_
#define LOG_H_

//abstraction for printf in debug mode only
//define values of timeout for debug and production mode
#ifdef DEBUG
#define LOG PRINTF 		//take LOG as printf in debug mode
#define TIMEOUT 500		//timeout of 5 sec for STOP and GO led
#define WAR_TIMEOUT 300		//timeout of 3 sec for WARNING led
#else
#define LOG(...)
#define TIMEOUT 2000		//timeout of 20 sec for STOP and GO led in production mode
#define WAR_TIMEOUT 500		//timeout of 5 sec for WARNING led in production mode
#endif

#endif /* LOG_H_ */
