#include "net.h"

int main()
{
	message msg;
	msg.header.type = MsgTypes::MovePlayer;

	msg << 2;

	int a;
	msg >> a;

	return 0;
}