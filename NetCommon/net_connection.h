#pragma once

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"

// class connection : public std::enable_shared_from_this<connection>
// to create a shared pointer from within this object
class connection
{
public:
	connection()
	{}

	virtual ~connection()
	{}

public:
	bool ConnectToServer();
	bool Disconnect();
	// const makes a compiler error for this function 
	// to change a member variable of a class
	bool IsConnected() const;
	bool Send(const message& msg);

protected:
	// Each connection has a unique socket
	asio::ip::tcp::socket socket;

	// A server can have multiple connections, but 
	// I don't want to have multiple asio contexts.
	// I want asio context to behave in tandem.
	// So the connection will be provided with an asio context
	// by the client or the server interface
	asio::io_context& asioContext;

	// This queue holds all messages to be sent to the remote side
	// of this connection
	tsqueue<message> queueMessagesOut;

	// TODO: understand
	// This queue holds all messages that have been received from
	// the remote side of this connection. It is a reference as
	// the "owner" of this connection is exprected to provide a queue
	tsqueue<owned_message>& queueMessagesIn;
};