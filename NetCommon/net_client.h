#pragma once

#include "net_common.h"
#include "net_message.h"
#include "net_tsqueue.h"
#include "net_connection.h"

// For setting up asio and setting up the connection.
// Also acts as an access point for the app to talk
// to a server.

class client_interface
{
	client_interface() : socket(context)
	{

	}

	virtual ~client_interface()
	{
		Disconnect();
	}


public:
	// Connect to the server with hostname/ip-address and port
	bool Connect(const std::string& host, const uint16_t port)
	{
		try
		{
			// Create connection
			m_connection = std::make_unique<connection>();

			// Resolve hostname/ip-address into tangiable physical address 
			// (can fail)
			asio::ip::tcp::resolver resolver(context);
			auto endpoint = resolver.resolve(host, std::to_string(port));

			m_connection->ConnectToServer(endpoint);

			// Start context thread
			thrContext = std::thread([this]() { context.run(); });
		}
		catch (std::exception& e)
		{
			std::cerr << "Client Exception: " << e.what() << "\n";
			return false;
		}

		return true;
	}

	void Disconnect()
	{
		if (IsConnected())
		{
			m_connection->Disconnect();
		}

		// Either way, we're also done with the asio context...
		context.stop();
		// ...and its thread
		if (thrContext.joinable())
			thrContext.join();

		// Destroy the connection object
		m_connection.release();
	}

	bool IsConnected()
	{
		if (m_connection)
			return m_connection->IsConnected();
		else
			return false;
	}

	// Get access to the queue of messages from the server
	tsqueue<owned_message>& Incoming()
	{
		return queueMessagesIn;
	}

protected:
	// The client inteface owns the asio context
	asio::io_context context;
	// Context needs a thread of its own to execute its work commands
	std::thread thrContext;
	// This is the hardware socket that is connected to the server
	asio::ip::tcp::socket socket;
	// The client has a single instance of a "connection" object, which handles data transfer
	std::unique_ptr<connection> m_connection;

private:
	tsqueue<owned_message> queueMessagesIn;

};