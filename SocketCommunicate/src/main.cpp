#include"Socket\ClientSocket\ClientSocket.h"
#include"Socket\ServerSocket\ServerSocket.h"

#include"Server\Server.h"

#include<stdio.h>



int main()
{
	WSADATA wsaData;
	Server server;

	if (!server.Init(49152))
	{
		fprintf(stderr, "Failed in initialize.\n");
		return 0;
	}

	server.Update();
	server.Release();

}