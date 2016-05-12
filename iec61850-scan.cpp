/// @author Cedric Boudinet
/// @license GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <libiec61850/iec61850_client.h>
#include "iec61850Exp_fun.h"

int main(int argc, char **argv)
{
	std::string hostname;
	int tcpPort = 102;

	if (argc > 1)
		hostname = argv[1];
	else
		hostname = "localhost";

	if (argc > 2)
		tcpPort = atoi(argv[2]);
	std::cout<<"Scanning server "<<hostname<<":"<<tcpPort<<std::endl;
	IedClientError error;
	IedConnection con = IedConnection_create();
	IedConnection_connect(con, &error, hostname.c_str(), tcpPort);
	if (error == IED_ERROR_OK)
	{
		display_server_structure(con);
	}
	else
	{
		printf("Connection failed with error %d!\n", error);
		return -1;
	}

	IedConnection_close(con);
	IedConnection_destroy(con);
	return 0;
}

