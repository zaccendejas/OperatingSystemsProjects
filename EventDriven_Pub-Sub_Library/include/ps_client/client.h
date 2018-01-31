// Client.h: PS Client Library -------------------------------------------------
#pragma once

#include "protectedQueue.h"
#include "thread.h"
#include "callback.h"
#include "MutexCheckerMacros.h"

#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// struct needed for thread creation
struct FileWithMessages {
    ProtectedQueue * myQueue;
    FILE * myFile;
    std::multimap<std::string, Callback*> *myCallbackTable;
	pthread_mutex_t *myTableLock;
};

// global to end the program
extern bool ENDTHIS;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// FUNCTIONS FOR DOING THINGS //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void *SendToServer(void *);
void *ReceiveFromServer(void *);
void *CreateMessages(void *);

class Client{
	public:
		Client(const char *, const char *, const char *);				//Constructor w/ specified host server and client identify
		void publish(const char *, const char *, size_t);		//schedules a message to be published to the server
		void subscribe(const char *, Callback *);						//schedules a subscription to be sent to the server + records a Callback
		void unsubscribe(const char *);										//schedules an unsibscribe + removes all Callbacks
		void disconnect();															//schedules disconnect + causes client shutdown
		void run();																	//starts required background messaging threads and processes incoming messages
		bool shutdown();
                //returns whether or not the client should shut down
                //notify all threads and functions that it is time to stop processing
                /*
                void *SendToServer(void *);
                void *ReceiveFromServer(void *);
                void *CreateMessages(void *);
                */
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// MADE PUBLIC SO GTESTS CAN TEST THE CLIENT //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		ProtectedQueue OutgoingMessages;
		ProtectedQueue IncomingMessages;
		std::multimap<std::string, Callback*> CallbackTable;
	private:
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// 	VARIABLES USED FOR VARIABLE THINGS //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		
		const char *Host = NULL;
		const char *Port = "9611";
		const char *CID =  NULL;
		int Nonce = 0;

		int SocketFD;
		FILE *Connection = NULL;

		pthread_mutex_t TableLock;

		Thread SendMessages;
		Thread RetrieveMessages;
		Thread CallbackMessages;
};

// vim: set expandtab sts=4 sw=4 ts=8 ft=cpp: ----------------------------------
