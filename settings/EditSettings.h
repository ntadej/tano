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
		void ok();
		void cancel();
		void custom();
		void browse();
		void reset();
		void restoreS();
		void set();

		void action(QAbstractButton *button);
		void editShortcut(QTableWidgetItem *titem);

	private:
		void createActions();
		void read();
		void readS();
		Ui::EditSettings ui;

		Shortcuts *keys;
		SettingsShortcuts *sshortcuts;
		SettingsMain *settings;

		QTableWidgetItem *item;

		QStringList settingsList;
		QStringList keysList;
		bool success;
};
#endif
