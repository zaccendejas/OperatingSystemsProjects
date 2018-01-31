#include "ps_client/callback.h"

void EchoCallback::run(Message &m){
	//run the echo callback that supposedly displays the message to stdout??
    std::cout << m.body << std::endl;
}
