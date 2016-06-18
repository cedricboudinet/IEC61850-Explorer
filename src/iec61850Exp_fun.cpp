/// @author Cedric Boudinet
/// @copyright GNU GPL Version 3
/// @file iec61850Exp_fun.cpp
/// Various functions using iec61850 protocol
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///

#include "iec61850Exp_fun.h"
#include "MmsValueWrapper.h"
#include <iostream>
#include <algorithm>
#ifdef WIN32
	#include <windows.h>
#else
	#include <termios.h>
	#include <unistd.h>
#endif

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

bool getVariableName(IedConnection con, const std::string & LNVarName, const std::string & parentDevice, const std::string & parentNode, std::string &varName, FunctionalConstraint &fc)
{
	varName = LNVarName;
	std::size_t found =varName.find("$");
	if(found!=std::string::npos)
	{
		std::string fcstr;
		fcstr=varName.substr(0,found);
		varName=varName.substr(found+1);
		std::replace(varName.begin(), varName.end(), '$', '.');
		varName = parentDevice+'/'+parentNode+'.'+varName;
		fc = FunctionalConstraint_fromString(fcstr.c_str());
		return true;
	}
	return false;
}

void dispLNVar(IedConnection con, const std::string & LNVarName, const std::string & parentDevice, const std::string & parentNode)
{
	//std::cout<<"  + "<<LNVarName<<std::endl;
	std::string varName; FunctionalConstraint fc;
	if(getVariableName(con, LNVarName, parentDevice, parentNode, varName, fc))
	{
		IedClientError error;
		char buffer[100];
		MmsValue* my_mms = IedConnection_readObject(con, &error, varName.c_str(), fc);
		std::cout<<"  + "<<varName<<" ["<<FunctionalConstraint_toString(fc)<<"]:";
		if(error!=0)
			std::cout<<"Error "<<error;
		std::cout<<"(type "<<MmsValue_getTypeString(my_mms)<<")= "<<MmsValue_printToBuffer(my_mms, buffer, 100);
		std::cout<<std::endl;
	}
}

std::vector<MmsValueWrapper> getVariableList(IedConnection con)
{
	std::vector<MmsValueWrapper> listVar;
	std::string varName;
	FunctionalConstraint fc;
	MmsType varType;
	MmsValue * mmsVal;
	IedClientError ier;
	std::vector<std::string> devices = getLDList(con);
	for(std::vector<std::string>::iterator itLD=devices.begin();itLD<devices.end();itLD++)
	{
		std::vector<std::string> nodes = getLNList(con, (*itLD));
		for(std::vector<std::string>::iterator itLN=nodes.begin();itLN<nodes.end();itLN++)
		{
			std::vector<std::string> dataObjects=getLNVars(con, (*itLD)+"/"+(*itLN));
			for(std::vector<std::string>::iterator itDO=dataObjects.begin();itDO<dataObjects.end();itDO++)
			{
				if(getVariableName(con,*itDO, *itLD, *itLN, varName, fc))
				{
					mmsVal = IedConnection_readObject(con, &ier, varName.c_str(), fc);
					varType = MmsValue_getType(mmsVal);
					if( (varType!=MMS_STRUCTURE))
					{   // Do not add structured types
						MmsValueWrapper newVal(varName, fc, varType);
						listVar.push_back(newVal);
					}
				}
			}
		}
	}
	return listVar;
}

void setIedPasswd(IedConnection IedCon, std::string password)
{
	MmsConnection mmsConnection = IedConnection_getMmsConnection(IedCon);
	IsoConnectionParameters parameters = MmsConnection_getIsoConnectionParameters(mmsConnection);
	AcseAuthenticationParameter auth = (AcseAuthenticationParameter) calloc(1, sizeof(struct sAcseAuthenticationParameter));
	char * passwd=strdup(password.c_str());
	AcseAuthenticationParameter_setPassword(auth, passwd);
	AcseAuthenticationParameter_setAuthMechanism(auth, ACSE_AUTH_PASSWORD);
	IsoConnectionParameters_setAcseAuthenticationParameter(parameters, auth);
	free(passwd);
}

void SetStdinEcho(bool enable)
{
#ifdef WIN32
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);

	if( !enable )
		mode &= ~ENABLE_ECHO_INPUT;
	else
		mode |= ENABLE_ECHO_INPUT;

	SetConsoleMode(hStdin, mode );

#else
	struct termios tty;
	tcgetattr(STDIN_FILENO, &tty);
	if( !enable )
		tty.c_lflag &= ~ECHO;
	else
		tty.c_lflag |= ECHO;

	(void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

int display_server_structure(IedConnection con)
{
	std::cout<<"Showing variables on server"<<std::endl;
	if (IedConnection_getState(con)==IED_STATE_CONNECTED)
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
		std::cout<<"Not connected"<<std::endl;
		return -1;
	}
	return 1;
}

int unpackSVToString(const SVClientASDU asdu, const char * format, std::string & result)
{
	int status = 0;
	int dataSize = SVClientASDU_getDataSize(asdu);
	int nbVals = strlen(format);
	std::stringstream oss;
	int idx = 0;
	for(int i=0;i<nbVals;i++)
	{
		switch(format[i])
		{
			case 'f':
				oss << SVClientASDU_getFLOAT32(asdu, idx)<<" ";
				idx+=4;
				break;
			case 'd':
				oss << SVClientASDU_getFLOAT64(asdu, idx)<<" ";
				idx+=8;
				break;
			case 'b':
				oss << SVClientASDU_getINT8(asdu, idx)<<" ";
				idx+=1;
				break;
			case 'h':
				oss << SVClientASDU_getINT16(asdu, idx)<<" ";
				idx+=2;
				break;
			case 'i':
				oss << SVClientASDU_getINT32(asdu, idx)<<" ";
				idx+=4;
				break;
			case 'B':
				oss << SVClientASDU_getINT8U(asdu, idx)<<" ";
				idx+=1;
				break;
			case 'H':
				oss << SVClientASDU_getINT16U(asdu, idx)<<" ";
				idx+=2;
				break;
			case 'I':
				oss << SVClientASDU_getINT32U(asdu, idx)<<" ";
				idx+=4;
				break;
		}
	}
	if(idx!=dataSize) status = -1;
	else result = oss.str();

	return status;
}
