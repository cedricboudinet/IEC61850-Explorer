/// @author Cedric Boudinet
/// @license GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///
#include "MmsInputDialog.h"
#include "MmsValueWrapper.h"
#include <QLocale>
#include <iostream>

MmsValue * MmsInputDialog::getMmsValue(const MmsValueWrapper & wrappedValue, bool & ok)
{
	MmsValue * newValue = MmsValue_clone(wrappedValue.getMmsValue());
	switch(wrappedValue.getType())
	{
		case MMS_FLOAT :
			{
				double oldValFloat = MmsValue_toFloat(newValue);
				double result = getDouble(_parent, "Change value", "New value:", oldValFloat, -1e9, 1e9, 6, &ok);
				if(ok)
					MmsValue_setFloat(newValue, result);
			}
			break;
		case MMS_VISIBLE_STRING:
			{
				QString result = getText(_parent, "Change value", "New value:", QLineEdit::Normal, MmsValue_toString(newValue), &ok);
				if(ok)
					MmsValue_setVisibleString(newValue, result.toStdString().c_str());
			}
			break;
		case MMS_INTEGER:
			{
				int oldVal = MmsValue_toInt32(newValue);
				int result = getInt(_parent, "Change value", "New value:", oldVal, -1e9, 1e9, 1, &ok);
				if(ok)
					MmsValue_setInt32(newValue, result);
			}
			break;
		case MMS_BOOLEAN:
			{
				bool oldVal = MmsValue_getBoolean(newValue);
				bool result = getBool(_parent, oldVal, &ok);
				if(ok)
					MmsValue_setBoolean(newValue, result);
			}
			break;
		default://TODO
			std::cout << "Data type modification not handled :"<<wrappedValue.getType()<<std::endl;
			ok = false;
			break;
			
	}
	return newValue;
}

bool MmsInputDialog::getBool(QWidget * parent, bool oldBool, bool *ok)
{
	return (bool) getInt(parent, "Change value", "New value:", (int)oldBool, 0, 1, 1, ok);
}
