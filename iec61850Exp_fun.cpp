#include "iec61850Exp_fun.h"
#include <iostream>
#include <algorithm>
std::vector<std::string> getLDList(IedConnection con)
{
	IedClientError error;
	std::vector<std::string> devices;
	LinkedList deviceList = IedConnection_getLogicalDeviceList(con, &error);
	LinkedList device=LinkedList_getNext(deviceList);
	while(device!=NULL)
	{
		devices.push_back((char*)device->data);
		device=LinkedList_getNext(device);
	}
	LinkedList_destroy(deviceList);
	return devices;
}

std::vector<std::string> getLNList(IedConnection con, const std::string & parentDevice)
{
	IedClientError error;
	std::vector<std::string> nodes;
	LinkedList logicalNodes = IedConnection_getLogicalDeviceDirectory(con, &error, parentDevice.c_str());

	LinkedList logicalNode = LinkedList_getNext(logicalNodes);
	while(logicalNode!=NULL)
	{
		nodes.push_back((char*)logicalNode->data);
		logicalNode = LinkedList_getNext(logicalNode);
	}
	LinkedList_destroy(logicalNodes);
	return nodes;
}

std::vector<std::string> getLNVars(IedConnection con, const std::string & parentLogicalNode)
{
	IedClientError error;
	std::vector<std::string> objects;
	LinkedList LNobjects = IedConnection_getLogicalNodeVariables(con, &error, parentLogicalNode.c_str());
	LinkedList LNobject = LinkedList_getNext(LNobjects);
	while(LNobject!=NULL)
	{
		objects.push_back((char*)LNobject->data);
		LNobject = LinkedList_getNext(LNobject);
	}
	LinkedList_destroy(LNobjects);
	return objects;
}

void dispLNVar(IedConnection con, const std::string & LNVarName, const std::string & parentDevice, const std::string & parentNode)
{
	//std::cout<<"  + "<<LNVarName<<std::endl;
	std::string varName(LNVarName);
	std::size_t found =varName.find("$");
	char buffer[100];
	if(found!=std::string::npos)
	{
		std::string fc;
		fc=varName.substr(0,found);
		varName=varName.substr(found+1);
		std::replace(varName.begin(), varName.end(), '$', '.');
		varName = parentDevice+'/'+parentNode+'.'+varName;
		std::cout<<"  + "<<varName<<":";
		IedClientError error;
		MmsValue* my_mms = IedConnection_readObject(con, &error, varName.c_str(), FunctionalConstraint_fromString(fc.c_str()));
		if(error!=0)
			std::cout<<"Error "<<error;
		std::cout<<"(type "<<MmsValue_getTypeString(my_mms)<<")= "<<MmsValue_printToBuffer(my_mms, buffer, 100);
		std::cout<<std::endl;
	}
}
