/// @author Cedric Boudinet
/// @license GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
#include "MmsValueWrapper.h"

MmsValueWrapper::MmsValueWrapper(const std::string & varName, FunctionalConstraint fc, MmsType varType) :
	_variableName(varName), _fc(fc), _mmstype(varType) { }

std::string MmsValueWrapper::getDispName()
{
	return (_variableName+ " ["+FunctionalConstraint_toString(_fc)+"]").c_str();
}

