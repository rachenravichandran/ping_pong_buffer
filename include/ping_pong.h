/********************************************************************
 *  File Name  : ping_pong.h
 *  Purpose    : To implement a Ping Pong Buffer structure
 *  Author     : Rachen Ravichandran
 *  Date       : 03-18-2019
********************************************************************/

#ifndef PING_PONG_H
#define PING_PONG_H

#include <stdint.h>                  /* Includes Standard INTEGER type definitions */

#define PING_PONG_BUFFER_LENGTH 5   /* Maximum Ping Pong Buffer Length */
#define BUFFER_TYPE signed int      /* Ping Pong Buffer Data type */

/* Structure definition for Ping Pong Buffer */
typedef struct ping_pong{
                          BUFFER_TYPE pingBuffer[PING_PONG_BUFFER_LENGTH];           /* Ping Buffer Array */
                          BUFFER_TYPE pongBuffer[PING_PONG_BUFFER_LENGTH];           /* Pong Buffer Array */
}t_PingPongBuffer;

#endif      /* end PING_PONG_H */
