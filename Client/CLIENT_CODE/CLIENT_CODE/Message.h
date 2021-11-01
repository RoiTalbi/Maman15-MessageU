#pragma once

#include <stdint.h>
#include "globals.h"


enum class MessageType : uint8_t
{
	GET_SYMMETRIC_KEY = 1,
	SYMMETRIC_KEY_SENT = 2,
	REGULAR_MESSAGE = 3
};



#pragma pack(1)
struct Message 
{

public:

	uint8_t sender_client_id[CLIENT_ID_SIZE_BYTES];
	uint32_t message_id;
	MessageType type;
	uint32_t content_size;
	uint8_t* content;


	Message(MessageType type,
			uint32_t message_id,
			uint8_t sender_client_id[CLIENT_ID_SIZE_BYTES],
			uint32_t content_size = 0, 
			uint8_t* content = NULL)
	{
		this->type = type;
		this->message_id = message_id;
		memcpy(this->sender_client_id, sender_client_id, CLIENT_ID_SIZE_BYTES);
		this->content_size = content_size;
		this->content = content;
	}

	Message(Message* other_message)
	{
		this->type = other_message->type;
		this->message_id = other_message->message_id;
		memcpy(this->sender_client_id, other_message->sender_client_id, CLIENT_ID_SIZE_BYTES);
		this->content_size = other_message->content_size;
		this->content = other_message->content;
	}
	
	~Message()
	{
		if (content)
		{
			delete content;
		}
	}

};

#pragma pack(pop)