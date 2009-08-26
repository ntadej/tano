#include "Locker.h"
#include "../Common.h"

#include <QDebug>

Locker::Locker(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(action(QAbstractButton*)));

	settings = Common::settings();

	if(settings->value("password"," ÉÀp»éiÊÖD").toString() == " ÉÀp»éiÊÖD") // Default '0000' password
		ui.labelWarning->setText("<center>"+tr("<b>Warning:</b>")+"<br>"+tr("You are using default password '0000'!")+"</center>");
	else
		ui.labelWarning->hide();
}

Locker::~Locker()
{

}

void Locker::action(QAbstractButton *button)
{
	switch(ui.buttonBox->standardButton(button)) {
	case 0x00000400:
		checkPassword();
		break;
	case 0x00400000:
		close();
		break;
	default:
		break;
	}
}

void Locker::checkPassword()
{

	QByteArray step1 = QCryptographicHash::hash(ui.editPassword->text().toAscii(), QCryptographicHash::Md5);
	QByteArray step2 = QCryptographicHash::hash(step1, QCryptographicHash::Md5);

	qDebug() << ui.editPassword->text() << QString::fromAscii(step2);

	if(settings->value("password", QCryptographicHash::hash(QCryptographicHash::hash("0000", QCryptographicHash::Md5), QCryptographicHash::Md5)).toByteArray() == step2) {
		isLocked = false;
		close();
	} else {
		ui.labelWarning->setText("<b>"+tr("Wrong password!")+"</b>");
	}
}
