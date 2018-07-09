#include"Socket\ClientSocket\ClientSocket.h"
#include"Socket\ServerSocket\ServerSocket.h"

#include"Server\Server.h"

#include<stdio.h>



int main()
{
	Server server;

	if (!server.Init(50000))
	{
		fprintf(stderr, "Failed in initialize.\n");
		return 0;
	}

	server.Update();
	server.Release();
}