/// @author Cedric Boudinet
/// @copyright GNU GPL Version 3
/// @file MmsValueWrapper.cpp
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
#include "MmsValueWrapper.h"

MmsValueWrapper::MmsValueWrapper(const std::string & varName, FunctionalConstraint fc, MmsType varType) :
	_variableName(varName), _fc(fc), _mmstype(varType), _mmsVal(NULL) { }

std::string MmsValueWrapper::getDispName()
{
	return (_variableName+ " ["+FunctionalConstraint_toString(_fc)+"]").c_str();
}

std::string MmsValueWrapper::getValueAsString(IedConnection IedCon)
{
	update(IedCon);
	char buffer[100];
	std::string ret = MmsValue_printToBuffer(_mmsVal, buffer, 100);
	return ret;
}

MmsType MmsValueWrapper::getType() const
{
	return _mmstype;
}

void MmsValueWrapper::update(IedConnection IedCon)
{
	IedClientError error; //TODO : handle read error
	if(_mmsVal)
		MmsValue_delete(_mmsVal);
	_mmsVal = IedConnection_readObject(IedCon, &error, _variableName.c_str(), _fc);
}

void MmsValueWrapper::setMmsValue(IedConnection IedCon, MmsValue* newVal)
{
	IedClientError error; //TODO : handle read error
	return IedConnection_writeObject(IedCon, &error, _variableName.c_str(), _fc, newVal);
}

MmsValue * MmsValueWrapper::getMmsValue() const
{
	return _mmsVal;
}
