/// @author Cedric Boudinet
/// @license GNU GPL Version 3
///
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

float MmsValueWrapper::getValueAsFloat(IedConnection IedCon)
{
	update(IedCon);
	return MmsValue_toFloat(_mmsVal);
}

void MmsValueWrapper::setFloatValue(IedConnection IedCon, float newVal)
{
	IedClientError error; //TODO : handle read error
	IedConnection_writeFloatValue(IedCon, &error, _variableName.c_str(), _fc, newVal);
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

void MmsValueWrapper::setStringValue(IedConnection IedCon, const std::string & newVal)
{
	IedClientError error; //TODO : handle read error
	char * buffer = new char[newVal.size()+1];
	std::copy(newVal.begin(), newVal.end(), buffer);
	buffer[newVal.size()]='\0';
	IedConnection_writeVisibleStringValue(IedCon, &error, _variableName.c_str(), _fc, buffer);
	delete [] buffer;
}

void MmsValueWrapper::setIntegerValue(IedConnection IedCon, int newVal)
{
	IedClientError error; //TODO : handle read error
	IedConnection_writeInt32Value(IedCon, &error, _variableName.c_str(), _fc, newVal);
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
