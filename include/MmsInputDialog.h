/// @author Cedric Boudinet
/// @copyright GNU GPL Version 3
/// @file MmsInputDialog.h
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///
#ifndef MMS_INPUT_DIALOG_H
#define MMS_INPUT_DIALOG_H
#include <QInputDialog>
#include <libiec61850/iec61850_client.h>
class MmsValueWrapper;

/*! class for MmsInputDialog window */
class MmsInputDialog : public QInputDialog
{
	public:
		/** MmsInputDialog constructor */
		MmsInputDialog(QWidget * parent=0) : QInputDialog(parent), _parent(parent) { }
		/** General function to get an Mms value.
		\param wrappedValue will be used to determine the prompt type and initial value.
		\param ok will be set to true is user pressed OK.
		*/
		MmsValue * getMmsValue(const MmsValueWrapper &, bool & ok);
		/** Convenience function to get a boolean value from the user.
		\param parent widget's parent
		\param oldBool initial value to propose
		\param ok will be set to true is user pressed OK.
		*/
		bool getBool(QWidget * parent, bool oldBool, bool *ok);

	private :
		QWidget * _parent;
};

#endif
