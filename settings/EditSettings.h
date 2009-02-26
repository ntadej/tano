#ifndef EDITSETTINGS_H
#define EDITSETTINGS_H
#include <QDialog>
#include <QStringList>

#include "SettingsMain.h"
#include "SettingsShortcuts.h"
#include "../control/Shortcuts.h"
#include "ui_EditSettings.h"

class EditSettings : public QDialog
{
	Q_OBJECT
	public:
		EditSettings(QWidget *parent = 0, Shortcuts *shortcuts = 0);

	private slots:
		void action(QAbstractButton *button);
		void ok();
		void cancel();

		void toggleCustom();

		void playlistBrowse();
		void playlistReset();

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

		Shortcuts *keys;
		SettingsShortcuts *sshortcuts;
		SettingsMain *settings;

		QTableWidgetItem *item;

		QKeySequence sequence;

		QStringList settingsList;
		QStringList keysList;
		bool success;
};
#endif
