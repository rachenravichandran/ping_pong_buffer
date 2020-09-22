/********************************************************************
 *  File Name  : ping_pong_ops.h
 *  Purpose    : To implement Ping Pong Buffer Operations
 *  Author     : Rachen Ravichandran
 *  Date       : 03-18-2019
********************************************************************/

#ifndef PING_PONG_OPS_H
#define PING_PONG_OPS_H

#include <stdio.h>
#include <pthread.h>        /* Includes POSIX Thread definitions */
#include "ping_pong.h"      /* Includes the Ping Pong Buffer Structure definitions */

#define NOT_LAST_WRITE 0	/* Indicates the Write operation of Ping or Pong Buffer is not the last line */
#define LAST_WRITE 1		/* Indicates the Write operation of Ping or Pong Buffer is the last line */

typedef struct pingPongThreadArgs{
		t_PingPongBuffer pingPong;
		/* Instance variable of Ping Pong Buffer Structure */
		FILE *fReadPtr;
		/* File pointer to "read.txt" in read mode */
		FILE *fWritePtr;
		/* File pointer "write.txt" in write mode */
		uint8_t endOfPingPongOperation;
		/* If 1, Ping Pong Operation has reached its end, i.e., last data in the "read.txt" file is read */
		/* Else 0, if Ping Pong Operation has not reached its end */
		pthread_barrier_t barr_WritePingReadPong;
		/* POSIX Thread Based Barrier used for Thread Synchronization. */
		/* This barrier waits until Ping Write operation in Ping Thread and Pong Read operation in pong Thread is complete */
		pthread_barrier_t barr_ReadPingWritePong;
		/* POSIX Thread Based Barrier used for Thread Synchronization. */
		/* This barrier waits until Ping Write operation in Ping Thread and Pong Read operation in pong Thread is complete */
}t_PingPongThreadArgs;

/*
 *   Function Name   :   readPingPongBuffer
 *	 Purpose         :   Read the data from each line of the "read.txt" file into the Ping/Pong Buffer
 *   Input           :   pingOrPongArray	-	Ping or Pong Buffer array
 *						 fReadPtr			- 	File pointer to "read.txt" in read mode
 *   Output          :   None
 *   Global Variable :   None
*/        
void readPingPongBuffer(BUFFER_TYPE *pingOrPongArray, FILE *fReadPtr);

/*
 *   Function Name   :   writePingPongBuffer
 *	 Purpose         :   Write the data from the Ping/Pong Buffer into each line of the "write.txt" file.
 *   Input           :   pingOrPongArray	-	Ping or Pong Buffer array 
 *						 fReadPtr			- 	File pointer to "write.txt" in write mode
 *						 isLastWrite 		- 	Indicates whether the write operation is last operation into the file.
 *									   		    It can take NOT_LAST_WRITE (0) or LAST_WRITE (1) as its values.
 *   Output          :   None
 *   Global Variable :   None
*/
void writePingPongBuffer(BUFFER_TYPE *pingOrPongArray, FILE *fWritePtr, uint8_t isLastWrite);

/*
 *   Function Name   :   pongThreadCallback
 *	 Purpose         :   Pong Thread Callback Function which reads or writes to/from the Pong Buffer
 *   Input           :   pingPongThreadArgs - Pointer to structure t_pingPongThreadArgs is passed as argument by the calling function.
 *   Output          :   None
 *   Global Variable :   None
*/
void* pongThreadCallback(void* pingPongThreadArgs);

/*
 *   Function Name   :   pingThreadCallback
 *	 Purpose         :   Ping Thread Callback Function which reads or writes to/from the Ping Buffer
 *   Input           :   pingPongThreadArgs - Pointer to structure t_pingPongThreadArgs is passed as argument by the calling function.
 *   Output          :   None
 *   Global Variable :   None
*/
void* pingThreadCallback(void* pingPongThreadArgs);

#endif      /* end PING_PONG_OPS_H */
