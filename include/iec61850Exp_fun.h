/// @author Cedric Boudinet
/// @copyright GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///
#ifndef iec61850EXP_FUN_H
#define iec61850EXP_FUN_H
#include <string>
#include <vector>
#include <sstream>
#include <libiec61850/sv_subscriber.h>
#include <libiec61850/iec61850_client.h>
#include <map>
class MmsValueWrapper;

/** returns a list of Logical Devices in a IED */
std::vector<std::string> getLDList(IedConnection con);

/** returns a list of Logical Nodes attached to parentDevice LD */
std::vector<std::string> getLNList(IedConnection con, const std::string & parentDevice);

/** returns a list of variables attached to parentLogicalNode LN */
std::vector<std::string> getLNVars(IedConnection con, const std::string & parentLogicalNode);

/** brief displays a variable \n
	prints its name, functional constraint, type, value */
void dispLNVar(IedConnection con, const std::string & LNVarName, const std::string & parentDevice, const std::string & parentNode);

/** returns a list or MmsValues in a IED */
std::vector<MmsValueWrapper> getVariableList(IedConnection con);

/** gets an iec61850 variable name from MMS style name*/
bool getVariableName(IedConnection con, const std::string & LNVarName, const std::string & parentDevice, const std::string & parentNode, std::string &varName, FunctionalConstraint &fc);

/** sets an IedConnection's password */
void setIedPasswd(IedConnection, std::string password);

/** turns on/off stdin display*/
void SetStdinEcho(bool enable = true);

/** displays complete IED tree */
int display_server_structure(IedConnection);

/** unpacks a Sample Value data into a std::string using a given format */
int unpackSVToString(const SVClientASDU asdu, const char * format, std::string & result);

const char * IedClientErrorToCharP(IedClientError error);
#endif
