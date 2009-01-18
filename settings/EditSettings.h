#ifndef EDITSETTINGS_H
#define EDITSETTINGS_H
#include <QDialog>
#include <QStringList>

#include "Settings.h"
#include "ui_EditSettings.h"

class EditSettings : public QDialog
{
	Q_OBJECT
	public:
		EditSettings(QWidget *parent = 0, QString file = "");

	private slots:
		void ok();
		void cancel();
		void custom();
		void browse();
		void reset();

	private:
		void createActions();
		void read();
		Ui::EditSettings ui;

		Settings *settings;
		QStringList settingsList;
		bool success;
};
#endif
