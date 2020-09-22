/********************************************************************
 *  File Name  : main.c
 *  Purpose    : To implement a Ping Pong Buffer operation
 *  Author     : Rachen Ravichandran
 *  Date       : 03-18-2019
********************************************************************/

#include <stdio.h>          /* Includes Standard input output definitions */
#include <time.h>			/* Includes time operations */
#include "ping_pong_ops.h"  /* Includes Ping Pong Buffer Operations and Thread */

int main(void)
{
    char choice;
    t_PingPongThreadArgs pingPongThreadParams;
	pingPongThreadParams.fReadPtr = fopen("read.txt","r");
	/* File open "read.txt" in read mode */
	pingPongThreadParams.fWritePtr = fopen("write.txt","w");
	/* File open "write.txt" in write mode */	
    if (pingPongThreadParams.fReadPtr == NULL || pingPongThreadParams.fWritePtr == NULL)  /* File open error in "read.txt" or "write.txt" */
    {
        perror("ERROR : File Operation Unsuccessful ");
    }
    else
    {
        pthread_t pingThread;           /* Ping Thread ID */
        uint8_t pingThreadSuccess;      /* Return value of Ping Buffer Thread Success */
        pthread_t pongThread;           /* Pong Thread ID */
	    uint8_t pongThreadSuccess;      /* Return value of Pong Buffer Thread Success */
        uint32_t startTime;				/* Records the time value at the beginning of Ping Pong Operation */
        
        printf("PING PONG BUFFER : Read and write operation will begin.\n");
        startTime = clock();			/* Records the time value at the beginning of Ping Pong Operation */ 

	    /* Initialization of barriers with 2 threads Synchronization */
	    pthread_barrier_init(&pingPongThreadParams.barr_WritePingReadPong, NULL, 2);
		pthread_barrier_init(&pingPongThreadParams.barr_ReadPingWritePong, NULL, 2);
	    
		readPingPongBuffer(pingPongThreadParams.pingPong.pingBuffer, pingPongThreadParams.fReadPtr);    /* Initial Setup */
	    /* Contents of the first line of "read.txt" file is read into the Ping Buffer initially */

        pingThreadSuccess = pthread_create(&pingThread, NULL, pingThreadCallback, (void *)&pingPongThreadParams);
        /* Creates the Ping Buffer Thread */
        pongThreadSuccess = pthread_create(&pongThread, NULL, pongThreadCallback, (void *)&pingPongThreadParams );
	    /* Creates the Pong Buffer Thread */
        if(pingThreadSuccess != 0 || pongThreadSuccess != 0)
        {
        	perror("ERROR : Ping/Pong Thread Creation Unsuccessful ");
    	}
    	else
    	{
	        pthread_join(pingThread,NULL);
	        pthread_join(pongThread,NULL);
	        /*Waits till Ping Buffer Thread Execution is complete */
	        printf("\nRead and write operation is complete. The contents of ***read.txt*** is copied into ***write.txt***\n"
	               "Time taken to complete Ping Pong Buffer Operation : %ld milliseconds", 
				   clock() - startTime);	/* Determines the total time taken at for Ping Pong Operation from startTime */
	        fclose(pingPongThreadParams.fReadPtr);       /* Close the file "read.txt" */
	        fclose(pingPongThreadParams.fWritePtr);      /* Close the file "write.txt" */
    	}
    	/* Deallocation of barriers */
		pthread_barrier_destroy(&pingPongThreadParams.barr_WritePingReadPong);
		pthread_barrier_destroy(&pingPongThreadParams.barr_ReadPingWritePong);
    }
    printf("\n\n\nPress Enter key to continue . . .");
    scanf("%c",&choice);
    pthread_exit(NULL);
    /*Exit the Main Thread Execution */
    return 0;
}
