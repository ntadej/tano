#ifndef EDITSETTINGS_H
#define EDITSETTINGS_H
#include <QDialog>
#include <QStringList>
#include <QSettings>

#include "../control/Shortcuts.h"
#include "ui_EditSettings.h"

class EditSettings : public QDialog
{
	Q_OBJECT
	public:
		EditSettings(QWidget *parent = 0, Shortcuts *s = 0);

	signals:
		void apply();

	private slots:
		void action(QAbstractButton *button);
		void ok();
		void cancel();

		void toggleCustom();
		void togglePlaylist();

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

		QSettings *settings;

		Shortcuts *shortcuts;

		QTableWidgetItem *item;

		QKeySequence sequence;

		QStringList actionsList;
		QStringList keysList;
		bool success;
};
#endif
