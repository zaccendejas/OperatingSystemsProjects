// CallbackTable.h by Zac Cendejas && Will Fritz
// Creates a concurrent map to represent a CallbackTable

#pragma once

#include <multimap>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class CallbackTable{
	public:

	private:
	std::multimap<std::string, Callback *> TopicCallbackPairings;
};
