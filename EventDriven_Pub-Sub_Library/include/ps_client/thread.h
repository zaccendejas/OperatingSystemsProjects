//Thread.h
//interface of the Thread class
#pragma once
#include "MutexCheckerMacros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef void *(*ThreadFunc)(void *);

class Thread{
	public:
		Thread(){}
        void start(ThreadFunc func, void *arg){
	        DID_IT_WORK(pthread_create(&thread, NULL, func, arg)); //create task
        }
        void join(void **result){
	        DID_IT_WORK(pthread_join(thread, result)); //wait for task
        }

        void detach(){
	        DID_IT_WORK(pthread_detach(thread)); //detaches the thread... once a thread is detached it is no longer joinable... 
        }

	private:
		pthread_t thread;
};
