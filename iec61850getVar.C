#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <libiec61850/iec61850_client.h>
#define buffLen 30

void print_help()
{
	std::cout
	<< "--help  show this help message and exit"<<std::endl
	<< "-P ARG  port number"<<std::endl
	<< "-H ARG  hostname"<<std::endl
	<< "-V ARG  set varname to read"<<std::endl
	<< "-F ARG  set variable functionnal constraint (int)"<<std::endl;
	std::cout<<	"Examples :"<<std::endl<<
		"  -V testDevice/LLN0.varMV.mag.f -F 1"<<std::endl<<
		"  -V testDevice/LLN0.varSAV.instMag.f -F 1"<<std::endl<<
		"  -V testDevice/LLN0.varASG.setMag.f -F 2"<<std::endl;
}

int parseCmdLine(int argc, char **argv, std::string & hostname, int & iecPort, std::string & varName, FunctionalConstraint & fc)
{
	int i;
	int number;
	for (i = 1; i < argc; i++)
	{
		if(strcmp(argv[i], "--help")==0)
		{
			print_help(); exit(1);
		}
		else if(strcmp(argv[i], "-P")==0)
		{
			iecPort = atoi(argv[i+1]);
			if((iecPort<=0)||(iecPort>=0xFFFF))
			{
				fprintf(stderr, "Please specify a valid port number\n");
				exit(-1);
			}
			i++;
		}
		else if(strcmp(argv[i], "-H")==0)
		{
			hostname = argv[i+1];
			i++;
		}
		else if(strcmp(argv[i], "-V")==0)
		{
			varName = argv[i+1];
			i++;
		}
		else if(strcmp(argv[i], "-F")==0)
		{
			fc=(FunctionalConstraint)atoi(argv[i+1]);
			i++;
		}
	}
}


int main(int argc, char **argv)
{
	std::string hostname, varName;
	FunctionalConstraint varType;
	int tcpPort = 102;
	hostname="localhost";
	char buffer[buffLen];
	parseCmdLine(argc, argv, hostname, tcpPort, varName, varType);
	
	std::cout<<"Reading variable '"<<varName<<"' of type "<<FunctionalConstraint_toString(varType)<<" on "<<hostname<<":"<<tcpPort<<std::endl;

	IedClientError error;
	IedConnection con = IedConnection_create();
	IedConnection_connect(con, &error, hostname.c_str(), tcpPort);
	if (error == IED_ERROR_OK)
	{
		IedClientError error;
		MmsValue* my_mms = IedConnection_readObject(con, &error, varName.c_str(), varType);
		if(error!=IED_ERROR_OK)
		{
			std::cout<<"Failed with error "<<error<<std::endl;
		}
		else
		{
			std::cout<<"Type is "<<MmsValue_getTypeString(my_mms)<<std::endl;
			MmsValue_printToBuffer(my_mms, buffer, buffLen);
			std::cout<<"Value is "<<buffer<<std::endl;
		}
		IedConnection_close(con);
	}
	else
	{
		printf("Connection failed with error %d!\n", error);
		return -1;
	}
	IedConnection_destroy(con);
	return 0;

}
