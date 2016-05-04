#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <libiec61850/iec61850_client.h>
#include "iec61850Exp_version.h"
#define buffLen 30

void print_help()
{
	std::cout
	<< "Usage :"<<std::endl
	<< "  --help    show this help message and exit"<<std::endl
	<< "  --version show version info and exit"<<std::endl
	<< "  -P ARG    port number"<<std::endl
	<< "  -H ARG    hostname"<<std::endl
	<< "  -V ARG    set varname to read"<<std::endl
	<< "  -F ARG    set variable functionnal constraint (int)"<<std::endl
	<< "  -S ARG    set new value"<<std::endl;
	std::cout<<	"Examples :"<<std::endl<<
		"  -V testDevice/LLN0.varMV.mag.f -F MX"<<std::endl<<
		"  -V testDevice/LLN0.varSAV.instMag.f -F MX"<<std::endl<<
		"  -V testDevice/LLN0.varASG.setMag.f -F SP"<<std::endl;
}

void print_version()
{
	std::cout<< "iec61850_varCmd version "<<IECEXP_VERSION<<std::endl;
}

void parseCmdLine(int argc, char **argv, std::string & hostname, int & iecPort, std::string & varName, FunctionalConstraint & fc, bool & writeIfTrue, float & newVal)
{
	int i;
	writeIfTrue = false;
	for (i = 1; i < argc; i++)
	{
		if(strcmp(argv[i], "--help")==0)
		{
			print_help(); exit(1);
		}
		else if(strcmp(argv[i], "--version")==0)
		{
			print_version(); exit(1);
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
			fc=FunctionalConstraint_fromString(argv[i+1]);
			i++;
		}
		else if(strcmp(argv[i], "-S")==0)
		{
			writeIfTrue=true;
			newVal = atof(argv[i+1]);
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
	bool writeIfTrue;
	float newVal;
	parseCmdLine(argc, argv, hostname, tcpPort, varName, varType, writeIfTrue, newVal);
	if(writeIfTrue)
		std::cout<<"Writing "<<newVal<<" in";
	else
		std::cout<<"Reading";
	std::cout<<" variable '"<<varName<<"' of type "<<FunctionalConstraint_toString(varType)<<" on "<<hostname<<":"<<tcpPort<<std::endl;

	IedClientError error;
	IedConnection con = IedConnection_create();
	IedConnection_connect(con, &error, hostname.c_str(), tcpPort);
	if (error == IED_ERROR_OK)
	{
		IedClientError error;
		MmsValue* my_mms;
		if(writeIfTrue)
		{
			my_mms=MmsValue_newFloat(newVal);
			IedConnection_writeObject(con, &error, varName.c_str(), varType, my_mms);
			if(error!=IED_ERROR_OK)
				std::cout<<"Failed with error "<<error<<std::endl;
			else
				std::cout<<"Done"<<std::endl;
		}
		else
		{
			my_mms = IedConnection_readObject(con, &error, varName.c_str(), varType);
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
		}
		MmsValue_delete(my_mms);
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
