#pragma once

#include <stdint.h>
#include "globals.h"


enum class MessageType : uint8_t
{
	GET_SYMMETRIC_KEY = 1,
	SEND_SYMMETRIC_KEY = 2,
	ENCRYPTED_BY_SYMMETRIC_KEY = 3
};



#pragma pack(1)
struct Message 
{
	MessageType type;
	uint32_t content_size;
	uint8_t* content;
};