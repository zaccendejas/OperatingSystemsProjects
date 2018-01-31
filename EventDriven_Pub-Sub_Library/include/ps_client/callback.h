//Callback.h
//interface of callback class

#ifndef callback_H
#define callback_H

#include <string>
#include <iostream>

struct Message{
    std::string type;        // Message type (MESSAGE, IDENTIFY, SUBSCRIBE, UNSUBSCRIBE, RETRIEVE, DISCONNECT)
    std::string topic;       // Message topic
    std::string sender;      // Message sender
	size_t      nonce;       // Sender's nonce
	std::string body;        // Message body
};

class Callback{
	public:
        //virtual method that must be implemented by a sub/child class
        //This is called whenever Callback should be used to process a Message
		virtual void run(Message &m) = 0; 
};

class EchoCallback : public Callback {
    public:
        void run(Message &m); 
};
#endif
