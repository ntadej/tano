#ifndef SHORTCUTEDIT_H_
#define SHORTCUTEDIT_H_

#include <QLineEdit>
#include <QKeyEvent>
#include <QKeySequence>

class ShortcutEdit : public QLineEdit {
Q_OBJECT
public:
	ShortcutEdit(QWidget *parent);
	virtual ~ShortcutEdit();

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	QKeySequence *key;
	QString e;
	bool super;
};

#endif /* SHORTCUTEDIT_H_ */
