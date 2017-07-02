/// @author Cedric Boudinet
/// @copyright GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///
#ifndef MMSVALUEWRAPPER_H
#define MMSVALUEWRAPPER_H
#include <libiec61850/iec61850_client.h>
#include <string>
/** Convenience class for handling Mms variables */
class MmsValueWrapper
{
	public:
		/** Class constructor.*/
		MmsValueWrapper(const std::string & variableName, FunctionalConstraint, MmsType);
		/** Returns an IEC61850 styled name.*/
		std::string getDispName();
		/** Returns the Function Constraint of the variable.*/
		FunctionalConstraint getFC() { return _fc; }
		/** Returns the variable name.*/
		std::string getName() { return _variableName; }
		/** Returns the variable value as a string.*/
		std::string getValueAsString(IedConnection);
		/** Sets a new value.*/
		IedClientError setMmsValue(IedConnection, MmsValue*);
		/** Returns the value type.*/
		MmsType getType() const;
		/** Returns a pointer to the current Mms value.*/
		MmsValue * getMmsValue() const;
		/** Updates the Mms value.*/
		void update(IedConnection);
	private:
		std::string _variableName;
		FunctionalConstraint _fc;
		MmsType _mmstype;
		MmsValue * _mmsVal;
};
#endif
