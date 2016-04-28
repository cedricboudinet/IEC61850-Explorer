#ifndef iec61850EXP_FUN_H
#define iec61850EXP_FUN_H
#include <string>
#include <vector>
#include <libiec61850/iec61850_client.h>
std::vector<std::string> getLDList(IedConnection con);
std::vector<std::string> getLNList(IedConnection con, const std::string & parentDevice);
std::vector<std::string> getLNVars(IedConnection con, const std::string & parentLogicalNode);
void dispLNVar(IedConnection con, const std::string & LNVarName, const std::string & parentDevice, const std::string & parentNode);

#endif