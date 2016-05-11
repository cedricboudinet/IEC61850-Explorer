#ifndef iec61850EXP_FUN_H
#define iec61850EXP_FUN_H
#include <string>
#include <vector>
#include <libiec61850/iec61850_client.h>
#include <map>

std::vector<std::string> getLDList(IedConnection con);
std::vector<std::string> getLNList(IedConnection con, const std::string & parentDevice);
std::vector<std::string> getLNVars(IedConnection con, const std::string & parentLogicalNode);
void dispLNVar(IedConnection con, const std::string & LNVarName, const std::string & parentDevice, const std::string & parentNode);
std::map<std::string, FunctionalConstraint> getVariableList(IedConnection con);
bool getVariableName(IedConnection con, const std::string & LNVarName, const std::string & parentDevice, const std::string & parentNode, std::string &varName, FunctionalConstraint &fc);
void setIedPasswd(IedConnection, std::string password);
void SetStdinEcho(bool enable = true);

#endif
