#ifndef EDITSETTINGS_H
#define EDITSETTINGS_H
#include <QDialog>
#include <QStringList>

#include "SettingsMain.h"
#include "ui_EditSettings.h"

class EditSettings : public QDialog
{
	Q_OBJECT
	public:
		EditSettings(QWidget *parent = 0, QString file = "", QStringList dl = QStringList(""));

	private slots:
		void ok();
		void cancel();
		void apply();
		void custom();
		void browse();
		void reset();

		void action(QAbstractButton *button);

	private:
		void createActions();
		void read();
		Ui::EditSettings ui;

		SettingsMain *settings;
		QStringList settingsList;
		bool success;
};
#endif
