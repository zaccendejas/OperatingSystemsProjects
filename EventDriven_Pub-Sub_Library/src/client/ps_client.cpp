#include "ps_client/client.h"

// extern global variable
bool ENDTHIS = false;

Client::Client(const char *host, const char *port, const char *cid){
	Host =	host;
	Port =	port;
	CID  =	cid;
    std::string CIDS = CID;
    for (char c : CIDS) Nonce += int(c); // Uses the ascii value of each char in CID to create a unique nonce that's a sum of them all
	Pthread_mutex_init(&TableLock, NULL);
}

void Client::publish(const char *topic, const char *message, size_t length){
    std::string topicS = topic;
    std::string messageS = message;
	std::string PubMessage = "PUBLISH " + topicS + " " + std::to_string(length) + " " + messageS;
	OutgoingMessages.push(PubMessage);
}


void Client::subscribe(const char *topic, Callback *callback){
	std::string Topic = topic;
	Pthread_mutex_lock(&TableLock);
	CallbackTable.emplace(Topic, callback);
	Pthread_mutex_unlock(&TableLock);
	std::string SubMessage = "SUBSCRIBE " + Topic + "\n";
	OutgoingMessages.push(SubMessage);
}


void Client::unsubscribe(const char *topic){
    std::string topicS = topic;
	std::string UnsubMessage = "UNSUBSCRIBE " + topicS + "\n";
	OutgoingMessages.push(UnsubMessage);
}

void Client::run(){
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	//			START CONNECTING TO SERVER				//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	int SocketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (SocketFD < 0) perror("Error opening socket\n");
	struct sockaddr_in ServerAddress;
	struct hostent *Server = gethostbyname(Host);
	if (Server == NULL) {
		std::cerr << "Error: no such host\n";
		return;
	}
	bzero((char *) &ServerAddress, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	bcopy((char *)Server->h_addr, (char *)&ServerAddress.sin_addr.s_addr, Server->h_length);
    int portInt;
    std::stringstream helperStream;
    helperStream << Port;
    helperStream >> portInt;
	ServerAddress.sin_port = htons(portInt);
	if (connect(SocketFD, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress)) < 0) std::cerr << "Error: Can't connect\n";
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	//				CONNECTED TO THE SEVER				//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	Connection = fdopen(SocketFD, "r+");				// Turn the socket into a file stream
    std::string CIDS = CID;
	std::string IdentifyMessage = "IDENTIFY " + CIDS + " " + std::to_string(Nonce) + "\n";
	fputs(IdentifyMessage.c_str(), Connection);		// Send the identify message
	char Response[BUFSIZ];
	fgets(Response, BUFSIZ, Connection);			// Receive the response from the server
	if (Response[0] == '4'){
		std::cerr << "Nonce does not match for the user\n";
		exit(EXIT_FAILURE);
	}
    FileWithMessages out;
    FileWithMessages in;
    out.myQueue = &OutgoingMessages;
    out.myFile = Connection;
    in.myQueue = &IncomingMessages;
    in.myFile = Connection;
    in.myCallbackTable = &CallbackTable;
	in.myTableLock = &TableLock;
	SendMessages.start(SendToServer, &out);
	RetrieveMessages.start(ReceiveFromServer, &in);
	SendMessages.detach();
	RetrieveMessages.detach();
	while (!shutdown()){
		CallbackMessages.start(CreateMessages, &in);
		Message *NewMessage;
		CallbackMessages.join((void **)&NewMessage);
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// LOOP THROUGH THE CALLBACK TABLE AND SEND THE MESSAGE TO MATCHING TOPICS //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		Pthread_mutex_lock(&TableLock);
		for (auto x : CallbackTable){
			if (x.first == NewMessage->topic){
				x.second->run(*NewMessage);
			}
		}
		delete NewMessage;
		Pthread_mutex_unlock(&TableLock);
	}
	fclose(Connection);
	close(SocketFD);

}

void Client::disconnect(){
    std::string cidS = CID;
    std::string nonceS = std::to_string(Nonce);
	std::string PubMessage = "DISCONNECT " + cidS + " " + nonceS + "\n";
	OutgoingMessages.push(PubMessage);
}

bool Client::shutdown(){
	return ENDTHIS;
}


void *SendToServer(void *arg){
    FileWithMessages *args = (FileWithMessages *)arg;
	ProtectedQueue *q = args->myQueue;
    FILE * Connection = args->myFile;
	while(1){
		std::string Outgoing = q->pop();
		fputs(Outgoing.c_str(), Connection);
	}
    return NULL;
}

void *ReceiveFromServer(void *arg){
    FileWithMessages *args = (FileWithMessages *)arg;
	ProtectedQueue *q = args->myQueue;
    FILE * Connection = args->myFile;
	while(1){
		char ServerResponse[BUFSIZ];
		fgets(ServerResponse, BUFSIZ, Connection);
		std::string FinalResponse = ServerResponse;
		q->push(FinalResponse);
		if (ServerResponse[0] == 'M'){	// Checks if a message is being retrieved, parses out the length, and gets the messages
			std::stringstream ParseBuffer(FinalResponse);
			int BodyLength;
			while (ParseBuffer >> BodyLength){}
			fgets(ServerResponse, BodyLength, Connection);
			FinalResponse = ServerResponse;
			q->push(FinalResponse);
		}
	}
    return NULL;
}

void *CreateMessages(void *arg){
    FileWithMessages *args = (FileWithMessages *)arg;
	ProtectedQueue *q = args->myQueue;
    std::multimap<std::string, Callback*> *CallbackTable = args->myCallbackTable;
	pthread_mutex_t *TableLock = args->myTableLock;

	std::string ServerResponse = q->pop();
	Message *CallbackMessage = new Message;
	if (ServerResponse[0] == 'M'){
		std::string MessageBody = q->pop();
		std::stringstream ParseBuffer(ServerResponse);
		std::string Trash;
		ParseBuffer >> Trash >> CallbackMessage->topic;
		ParseBuffer >> Trash >> CallbackMessage->sender;
		CallbackMessage->body = ServerResponse + MessageBody;
		CallbackMessage->type = "MESSAGE";
	}
	else if (ServerResponse[0] == '2' && ServerResponse[4] == 'D'){
		ENDTHIS = true;	// Shutdown returns this
		std::stringstream ParseBuffer(ServerResponse);
		std::string Trash;
		ParseBuffer >> Trash;
		ParseBuffer >> Trash >> CallbackMessage->sender;
		ParseBuffer >> Trash >> CallbackMessage->nonce;
		CallbackMessage->type = "DISCONNECT";
		CallbackMessage->body = ServerResponse;
	}
	else if (ServerResponse[0] == '2' && ServerResponse[4] == 'P'){
		std::stringstream ParseBuffer(ServerResponse);
		std::string TheTopic;
		while (ParseBuffer >> TheTopic){}
		CallbackMessage->topic = TheTopic;
		CallbackMessage->type = "PUBLISH";
		CallbackMessage->body = ServerResponse;
	}
	else if (ServerResponse[0] == '2' && ServerResponse[4] == 'U'){
		std::stringstream ParseBuffer(ServerResponse);
		std::string TheTopic;
		while (ParseBuffer >> TheTopic){}
		CallbackMessage->topic = TheTopic;
		CallbackMessage->type = "UNSUBSCRIBE";
		CallbackMessage->body = ServerResponse; 
		Pthread_mutex_lock(TableLock);
		std::multimap<std::string, Callback *>::iterator it = CallbackTable->find(TheTopic);
		CallbackTable->erase(it);
		Pthread_mutex_unlock(TableLock);
	}
	else if (ServerResponse[0] == '2' && ServerResponse[4] == 'S'){
		std::stringstream ParseBuffer(ServerResponse);
		std::string TheTopic;
		while (ParseBuffer >> TheTopic){}
		CallbackMessage->topic = TheTopic;
		CallbackMessage->type = "SUBSCRIBE";
		CallbackMessage->body = ServerResponse;
	}
	else if (ServerResponse[0] == 2 && ServerResponse[4] == 'I'){
		std::stringstream ParseBuffer(ServerResponse);
		std::string Trash;
		ParseBuffer >> Trash;
		ParseBuffer >> Trash;
		ParseBuffer >> Trash >> CallbackMessage->sender;
		ParseBuffer >> Trash >> CallbackMessage->nonce;
		CallbackMessage->type = "IDENTIFY";
		CallbackMessage->body = ServerResponse;
	}
	else if (ServerResponse[0] == '4' && ServerResponse[3] == '2'){
		std::stringstream ParseBuffer(ServerResponse);
		std::string Trash;
		ParseBuffer >> Trash;
		ParseBuffer >> Trash >> CallbackMessage->nonce;
		while (ParseBuffer >> Trash) {}
		CallbackMessage->sender = Trash;
		CallbackMessage->type = "IDENTIFY";
		CallbackMessage->body = ServerResponse;
	}
	else if (ServerResponse[0] == '4' && ServerResponse[3] == '3'){
		std::stringstream ParseBuffer(ServerResponse);
		std::string TheTopic;
		while (ParseBuffer >> TheTopic){}
		CallbackMessage->topic = TheTopic;
		CallbackMessage->type = "SUBSCRIBE";
		CallbackMessage->body = ServerResponse;
	}
	else if (ServerResponse[0] == '4' && ServerResponse[3] == '4'){
		std::stringstream ParseBuffer(ServerResponse);
		std::string TheTopic;
		while (ParseBuffer >> TheTopic){}
		CallbackMessage->topic = TheTopic;
		CallbackMessage->type = "UNSUBSCRIBE";
		CallbackMessage->body = ServerResponse;
	}else{
		CallbackMessage->body = ServerResponse;
	}
	return (void *)CallbackMessage;

}
