// ProtectedQueue.h by Zac Cendejas && Will Fritz
// Implements a queue with locks so that multiple threads can use it

#pragma once

#include "MutexCheckerMacros.h"

#include <queue>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class ProtectedQueue{
	public:
		ProtectedQueue(){
			Pthread_mutex_init(&Lock, NULL);
			Pthread_cond_init(&Fill, NULL);
		}

		void push(const std::string &Message){
			Pthread_mutex_lock(&Lock);
			Correspondence.push(Message);
			Pthread_cond_signal(&Fill);
			Pthread_mutex_unlock(&Lock);
		}

		std::string pop(){
			Pthread_mutex_lock(&Lock);
			while (Correspondence.empty()) Pthread_cond_wait(&Fill, &Lock);
			std::string Message = Correspondence.front();
			Correspondence.pop();
			Pthread_mutex_unlock(&Lock)
			return Message;
		}

		bool empty(){
			bool isEmpty;
			Pthread_mutex_lock(&Lock);
			isEmpty = Correspondence.empty();
			Pthread_mutex_unlock(&Lock)
			return isEmpty;
		}

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// PUBLIC SO THAT GTEST HAS ACCESS TO IT //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		std::queue<std::string> Correspondence; // Bc using Messages would get confusing

	private:
		pthread_mutex_t Lock;
		pthread_cond_t	Fill;
};
