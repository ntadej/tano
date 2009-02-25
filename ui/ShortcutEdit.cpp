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

	if(event != nkey && event != skey) {
		if(event->key() == Qt::Key_Shift) {
			super = true;
			skey = event;
			e = "Shift";
			return;
		} else if(event->key() == Qt::Key_Control) {
			super = true;
			skey = event;
			e = "Ctrl";;
			return;
		} else if(event->key() == Qt::Key_Meta){
			super = true;
			skey = event;
			e = "Meta";
			return;
		} else if(event->key() == Qt::Key_Alt) {
			super = true;
			skey = event;
			e = "Alt";
			return;
		} else {
			nkey = event;
		}
	}
}

void ShortcutEdit::keyReleaseEvent(QKeyEvent *event)
{
	event->ignore();

	if(super == true && event != skey) {
		super = false;
		if(event->text() != "")
			key = new QKeySequence(e+"+"+event->text());
		else
			key = new QKeySequence(e+"+"+QKeySequence(event->key()).toString());
	} else if(event != skey) {
		key = new QKeySequence(event->key());
	}

	skey = 0;
	nkey = 0;

	this->setText(key->toString(QKeySequence::NativeText));
}
