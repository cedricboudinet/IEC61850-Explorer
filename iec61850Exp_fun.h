/// @author Cedric Boudinet
/// @license GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///
#ifndef iec61850EXP_FUN_H
#define iec61850EXP_FUN_H
#include <string>
#include <vector>
#include <libiec61850/iec61850_client.h>
#include <map>
class MmsValueWrapper;

std::vector<std::string> getLDList(IedConnection con);
std::vector<std::string> getLNList(IedConnection con, const std::string & parentDevice);
std::vector<std::string> getLNVars(IedConnection con, const std::string & parentLogicalNode);
void dispLNVar(IedConnection con, const std::string & LNVarName, const std::string & parentDevice, const std::string & parentNode);
std::vector<MmsValueWrapper> getVariableList(IedConnection con);
bool getVariableName(IedConnection con, const std::string & LNVarName, const std::string & parentDevice, const std::string & parentNode, std::string &varName, FunctionalConstraint &fc);
void setIedPasswd(IedConnection, std::string password);
void SetStdinEcho(bool enable = true);
int display_server_structure(IedConnection);

#endif
