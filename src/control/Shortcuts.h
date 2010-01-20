#ifndef TANO_SHORTCUTS_H_
#define TANO_SHORTCUTS_H_

#include <QObject>
#include <QAction>
#include <QList>
#include <QStringList>
#include <QSettings>

class Shortcuts : public QObject {
	Q_OBJECT
public:
	Shortcuts(QList<QAction*> list);
	virtual ~Shortcuts();

	void apply();
	QStringList defaultKeys() const {return defaultList;};
	QStringList actionsNames() const {return actionsName;};

private:
	QList<QAction*> actions;
	QStringList defaultList;
	QStringList actionsName;
	QStringList keys;

	QSettings *settings;
};

#endif // TANO_SHORTCUTS_H_
