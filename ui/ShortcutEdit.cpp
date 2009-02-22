#include "ShortcutEdit.h"

ShortcutEdit::ShortcutEdit(QWidget *parent)
	:QLineEdit(parent)
{
	this->setReadOnly(true);
	super = false;
}

ShortcutEdit::~ShortcutEdit()
{

}

void ShortcutEdit::keyPressEvent(QKeyEvent *event)
{
	event->ignore();

	if(super != true) {
		if(event->key() == Qt::Key_Shift) {
			super = true;
			e = "Shift";
			return;
		} else if(event->key() == Qt::Key_Control) {
			super = true;
			e = "Ctrl";;
			return;
		} else if(event->key() == Qt::Key_Meta){
			super = true;
			e = "Meta";
			return;
		} else if(event->key() == Qt::Key_Alt) {
			super = true;
			e = "Alt";
			return;
		}
	}

	if(super == true) {
		super = false;
		key = new QKeySequence(e+"+"+event->text());
	} else {
		key = new QKeySequence(event->key());
	}

	this->setText(key->toString(QKeySequence::NativeText));
}
