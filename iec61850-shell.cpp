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
	std::cout<<"Showing variables on server "<<hostname<<":"<<tcpPort<<std::endl;
	IedClientError error;
	IedConnection con = IedConnection_create();
	IedConnection_connect(con, &error, hostname.c_str(), tcpPort);
	if (error == IED_ERROR_OK)
	{
		std::vector<std::string> devices = getLDList(con);
		for(std::vector<std::string>::iterator itLD=devices.begin();itLD<devices.end();itLD++)
		{
			std::cout<<"LD : "<<(*itLD)<<std::endl;
			std::vector<std::string> nodes = getLNList(con, (*itLD));
			for(std::vector<std::string>::iterator itLN=nodes.begin();itLN<nodes.end();itLN++)
			{
				std::cout<<" LN : "<<(*itLN)<<std::endl;
				std::vector<std::string> dataObjects=getLNVars(con, (*itLD)+"/"+(*itLN));
				for(std::vector<std::string>::iterator itDO=dataObjects.begin();itDO<dataObjects.end();itDO++)
				{
					//std::cout<<"  MMS : "<<(*itDO)<<std::endl;
					dispLNVar(con, *itDO, *itLD, *itLN);
				}
			}
		}
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

