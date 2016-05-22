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
#include "iec61850-shell.h"

void print_shell_help()
{
	std::cout	<<"iec61850-shell commands :"
				<<" ? : this help"<<std::endl
//				<<" h : change hostname"<<std::endl
//				<<" p : change port"<<std::endl
				<<" c : connect"<<std::endl
				<<" d : disconnect"<<std::endl
//				<<" s : set password"<<std::endl
//				<<" l : list variables on server"<<std::endl
//				<<" a : add variable to watch list"<<std::endl
//				<<" w : show watch list"<<std::endl
//				<<" r : remove variable from watch list"<<std::endl
				<<" t : display all the variables and their values"<<std::endl
				;
}

int main(int argc, char **argv)
{
	std::string hostname("localhost");
	int tcpPort = 102;

	if (argc > 1)
		hostname = argv[1];

	if (argc > 2)
		tcpPort = atoi(argv[2]);
	std::string input;

	IedClientError error;
	IedConnection con = IedConnection_create();

	while((std::cout<<"iec61850-shell>") && (std::cin>>input))
	{
		if(input[0]=='q') break;
		switch(input[0])
		{
			case '?':
				print_shell_help();
				break;
			case 'c':
				IedConnection_connect(con, &error, hostname.c_str(), tcpPort);
				if(error!=IED_ERROR_OK)
					std::cout<<"Connection failed"<<std::endl;
				break;
			case 'd':
				IedConnection_close(con);
				break;
			case 't':
				display_server_structure(con);
		}
	}
	IedConnection_close(con);
	IedConnection_destroy(con);
	return 0;
}

