/********************************************************************
 *  File Name  : ping_pong_ops.c
 *  Purpose    : To implement Ping Pong Buffer Operations
 *  Author     : Rachen Ravichandran
 *  Date       : 03-18-2019
********************************************************************/

#include "ping_pong_ops.h"  /* Includes Ping Pong Buffer Operations and Thread */

/*
 *   Function Name   :   readPingPongBuffer
 *	 Purpose         :   Read the data from each line of the "read.txt" file into the Ping/Pong Buffer
 *   Input           :   pingOrPongArray	-	Ping or Pong Buffer array
 *						 fReadPtr			- 	File pointer to "read.txt" in read mode
 *   Output          :   None
 *   Global Variable :   None
*/    
void readPingPongBuffer(BUFFER_TYPE *pingOrPongArray, FILE *fReadPtr)
{
    uint32_t i;
    for(i = 0; i < PING_PONG_BUFFER_LENGTH; i++)
    {
        fscanf(fReadPtr,"%d",&pingOrPongArray[i]); 
        /* Read the data from each line of the "read.txt" file into the Ping/Pong Buffer */
    }
}

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
void writePingPongBuffer(BUFFER_TYPE *pingOrPongArray, FILE *fWritePtr, uint8_t isLastWrite)
{
    uint32_t i;
    for(i = 0; i < PING_PONG_BUFFER_LENGTH; i++)
    {
    	fprintf(fWritePtr,"%d",pingOrPongArray[i]);
    	/* Write the data from the Ping/Pong Buffer into each line of the "write.txt" file */
    	
    	if( i == PING_PONG_BUFFER_LENGTH - 1 )	/* If the last element of the Ping or Pong Buffer is reached */
    	{
    		if(isLastWrite == 0)				/* And is not the Last Write */
    		{
				fprintf(fWritePtr,"\n");		/* Then, new line character is printed */
			}									
			/* NOTE : If it is the last line, no new line character is printed and is instead indicated by End Of File (EOF) */
    	}		
    	else									/* If the written value is not the last element of the Ping or Pong Buffer */
    	{
    		fprintf(fWritePtr," ");				/* Then, " " is printed instead */
    	}
    }
}

/*
 *   Function Name   :   pongThreadCallback
 *	 Purpose         :   Pong Thread Callback Function which reads or writes to/from the Pong Buffer
 *   Input           :   pingPongThreadArgs - Pointer to structure t_pingPongThreadArgs is passed as argument by the calling function.
 *   Output          :   None
 *   Global Variable :   None
*/
void* pongThreadCallback(void* pingPongThreadArgs)
{
	t_PingPongThreadArgs *ppArgs = (t_PingPongThreadArgs *)pingPongThreadArgs;
	/* Pointer to Ping Pong Thread Arguments */
	
    while(1)                  /* Infinite Loop. Exit Condition is evaluated by EOF or endOfPingPongOperation */
    {
        readPingPongBuffer(ppArgs->pingPong.pongBuffer, ppArgs->fReadPtr);
        /* Contents of the "read.txt" file is read into the Ping Buffer */
		if(feof(ppArgs->fReadPtr))							/* If after Pong Read, the file has reached EOF */
        {
        	ppArgs->endOfPingPongOperation = 1;				/* It indicates the end of Ping Pong Operation */
        	pthread_barrier_wait(&ppArgs->barr_WritePingReadPong);
        	/* Pong Thread Waits or Synchronizes till Ping completes its Write Operation */
        	writePingPongBuffer(ppArgs->pingPong.pongBuffer, ppArgs->fWritePtr, LAST_WRITE);
        	/* Contents of the Pong Buffer is written into the file (Last Write) */
        	break;
    	}
    	
    	pthread_barrier_wait(&ppArgs->barr_WritePingReadPong);
		/* Pong Thread Waits or Synchronizes till Ping completes its Write Operation */  
		  
        writePingPongBuffer(ppArgs->pingPong.pongBuffer, ppArgs->fWritePtr, NOT_LAST_WRITE);
		/* Contents of the Pong Buffer is written into the file (Not a last Write) */    
        pthread_barrier_wait(&ppArgs->barr_ReadPingWritePong);
        /* Pong Thread Waits or Synchronizes till Ping completes its Read Operation */
        
        if(ppArgs->endOfPingPongOperation == 1)		/* If Ping Read is the final operation */
        {
        	break;							/* The loop is exited */
    	}
    }
    pthread_exit(NULL);
    /* Exits the Pong Buffer Thread */
    return NULL;
}

/*
 *   Function Name   :   pingThreadCallback
 *	 Purpose         :   Ping Thread Callback Function which reads or writes to/from the Ping Buffer
 *   Input           :   pingPongThreadArgs - Pointer to structure t_pingPongThreadArgs is passed as argument by the calling function.
 *   Output          :   None
 *   Global Variable :   None
*/
void* pingThreadCallback(void* pingPongThreadArgs)
{
	t_PingPongThreadArgs *ppArgs = (t_PingPongThreadArgs *)pingPongThreadArgs;
	/* Pointer to Ping Pong Thread Arguments */
	
    while(1)                    /* Infinite Loop. Exit Condition is evaluated by EOF or endOfPingPongOperation */
    {
        writePingPongBuffer(ppArgs->pingPong.pingBuffer, ppArgs->fWritePtr, NOT_LAST_WRITE);
        /* Contents of the Ping Buffer is written into the file (Not a last Write) */
        pthread_barrier_wait(&ppArgs->barr_WritePingReadPong);
        /* Ping Thread Waits or Synchronizes till Pong completes its Read Operation */
        
        if(ppArgs->endOfPingPongOperation == 1)		/* If Pong Read is the final operation */
        {
        	break;							/* The loop is exited */
    	}
    	
        readPingPongBuffer(ppArgs->pingPong.pingBuffer, ppArgs->fReadPtr);
        /* Contents of the "read.txt" file is read into the Ping Buffer */
        
        if(feof(ppArgs->fReadPtr))					/* If after Ping Read, the file has reached EOF */
    	{
    		ppArgs->endOfPingPongOperation = 1;		/* It indicates the end of Ping Pong Operation */
    		pthread_barrier_wait(&ppArgs->barr_ReadPingWritePong);
    		/* Ping Thread Waits or Synchronizes till Pong completes its Write Operation */
    		writePingPongBuffer(ppArgs->pingPong.pingBuffer, ppArgs->fWritePtr, LAST_WRITE);
    		/* Contents of the Ping Buffer is written into the file (Last Write) */
        	break;
    	}
    	pthread_barrier_wait(&ppArgs->barr_ReadPingWritePong);
    	/* Ping Thread Waits or Synchronizes till Pong completes its Write Operation */
	}
    pthread_exit(NULL);
    /* Exits the Ping Buffer Thread */
    return NULL;
}

