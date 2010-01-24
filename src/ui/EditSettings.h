#ifndef TANO_EDITSETTINGS_H_
#define TANO_EDITSETTINGS_H_

#include <QDialog>
#include <QStringList>
#include <QSettings>

#include "../control/Shortcuts.h"

#include <ui_EditSettings.h>

class EditSettings : public QDialog
{
Q_OBJECT
public:
	EditSettings(QWidget *parent = 0, Shortcuts *s = 0);
	~EditSettings();
signals:
	void apply();

private slots:
	void action(QAbstractButton *button);
	void ok();
	void cancel();

	void toggleCustom();
	void toggleNetwork();
	void togglePlaylist();

	void playlistBrowse();
	void playlistReset();
	void dirBrowse();
	void dirReset();

	void shortcutRestore();
	void shortcutSequence(const QKeySequence &s);
	void shortcutSet();
	void shortcutClear();
	void shortcutEdit(QTableWidgetItem *titem);

private:
	void createActions();
	void read();
	void shortcutRead();

	Ui::EditSettings ui;
	QSettings *settings;
	Shortcuts *shortcuts;
	QTableWidgetItem *item;
	QKeySequence sequence;
	QStringList actionsList;
	QStringList keysList;
	bool success;
};

#endif // TANO_EDITSETTINGS_H_
