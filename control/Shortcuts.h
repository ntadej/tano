#ifndef TANO_SHORTCUTS_H_
#define TANO_SHORTCUTS_H_

#include <QObject>
#include <QAction>
#include <QList>
#include <QStringList>

class Shortcuts : public QObject {
	Q_OBJECT
public:
	Shortcuts(QList<QAction*> list);
	virtual ~Shortcuts();

	void apply();
	QStringList defaultKeys();
	QStringList actionsNames();

private:
	QList<QAction*> actions;
	QStringList defaultList;
	QStringList actionsName;
	QStringList keys;
};

#endif // TANO_SHORTCUTS_H_
