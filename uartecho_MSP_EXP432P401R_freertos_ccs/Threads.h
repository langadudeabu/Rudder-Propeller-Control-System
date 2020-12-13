/*
 * Threads.h
 *
 *  Created on: Nov 14, 2019
 *      Author: Sean
 */

#ifndef THREADS_H_
#define THREADS_H_

/* POSIX Header files */
#include <pthread.h>
#include <semaphore.h>

sem_t sema4;


#define COMMANDLENGTH 20

#define echo 0
pthread_t uartthread_handler; // used in main_freertos.c to latch on to the BluetoothThread
pthread_t RPMreader_handler;
pthread_t posfeedback_handler;
pthread_t feedback_handler;
pthread_t setPosition_handler;
pthread_t SetPosition_handler;

void *setToTarget(void *);
void *setToZero(void *);
void *ReadPosition(void *);
void *RPM_reader(void *);
void *sendMeasurements(void *);
void *positionFeedback(void *);
void *bluetooth2(void *);

#endif /* THREADS_H_ */
