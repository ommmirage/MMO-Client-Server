#include "net.h"

class CustomClient : public client_interface
{
	void Move(float x, float y)
	{
		message msg;
		msg.header.type = MsgTypes::MovePlayer;
		msg << x << y;
		Send(msg);
	}
};

int main()
{
	CustomClient c;

}