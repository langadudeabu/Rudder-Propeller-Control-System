/*
 * HeartBeat.h
 *
 *  Created on: Nov 14, 2019
 *      Author: Sean
 */

/* POSIX Header Files */
#include <pthread.h>

#ifndef HEARTBEAT_H_
#define HEARTBEAT_H_

pthread_t heartbeatThread_handler;
void *heartbeat(void *arg0);


#endif /* HEARTBEAT_H_ */
