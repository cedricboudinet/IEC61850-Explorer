#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <libiec61850/iec61850_client.h>
#define buffLen 30
int main(int argc, char **argv)
{
	std::string hostname, varName;
	FunctionalConstraint varType;
	int tcpPort = 102;
	char buffer[buffLen];
	
	if (argc > 1)
		varName = argv[1];
	else
	{
		std::cout<<"Please specify a variable name and a type"<<std::endl;
		std::cout<<	"Examples :"<<std::endl<<
					"  testDevice/LLN0.varMV0.mag.f 2"<<std::endl<<
					"  testDevice/LLN0.varSAV1.instMag.f 1"<<std::endl<<
					"  testDevice/LLN0.varASG1.setMag.f 2"<<std::endl;
		return 0;
	}
	if(argc > 2)
		varType=(FunctionalConstraint)atoi(argv[2]);
	if (argc > 3)
		hostname = argv[3];
	else
		hostname = "localhost";

	if (argc > 4)
		tcpPort = atoi(argv[4]);
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
