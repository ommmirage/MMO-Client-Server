#pragma once
#include "net_comon.h"

enum class MsgTypes
{
	MovePlayer
};

struct message_header
{
	// Enum class ensures that the messages are valid at compile time
	MsgTypes type;
	uint32_t size = 0;
};

struct message
{
	message_header header;
	std::vector<uint8_t> body;

	size_t size() const
	{
		return sizeof(message_header) + body.size();
	}

	friend std::ostream& operator << (std::ostream& os, const message& msg)
	{
		os << "ID:" << int(msg.header.type) << " Size:" << msg.header.size;
		return os;
	}

	// Pushes any POD (Plain Old Data) into the message buffer
	template<typename DataType>
	friend message& operator << (message& msg, const DataType& data)
	{
		// Check that the type of the data being pushed is trivially copyable
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");
		
		size_t i = msg.body.size();

		msg.body.resize(msg.body.size() + sizeof(DataType));

		std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

		msg.header.size = msg.size();

		return msg;
	}

	// Pulls any POD-like data form the message buffer
	template<typename DataType>
	friend message& operator >> (message& msg, DataType& data)
	{
		// Check that the type of the data being pushed is trivially copyable
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

		// Cache the location towards the end of the vector where the pulled data starts
		size_t i = msg.body.size() - sizeof(DataType);

		// Physically copy the data from the vector into the user variable
		std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

		// Shrink the vector to remove read bytes, and reset end position
		msg.body.resize(i);

		// Recalculate the message size
		msg.header.size = msg.size();

		// Return the target message so it can be "chained"
		return msg;
	}
};