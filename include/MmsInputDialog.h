/// @author Cedric Boudinet
/// @license GNU GPL Version 3
///
/// Distributed under the GNU GPL version 3 License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/)
///
#ifndef MMS_INPUT_DIALOG_H
#define MMS_INPUT_DIALOG_H
#include <QInputDialog>
#include <libiec61850/iec61850_client.h>
class MmsValueWrapper;

class MmsInputDialog : public QInputDialog
{
	public:
		MmsInputDialog(QWidget * parent=0) : QInputDialog(parent), _parent(parent) { }
		MmsValue * getMmsValue(const MmsValueWrapper &, bool & ok);
		bool getBool(QWidget * parent, bool oldBool, bool *ok);

	private :
		QWidget * _parent;
};

#endif
