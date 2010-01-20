#ifndef TANO_FIRSTRUN_H_
#define TANO_FIRSTRUN_H_

#include <QObject>
#include <QWizard>
#include <QSettings>
#include "License.h"

class QCheckBox;
class QLabel;
class QLineEdit;
class QRadioButton;

class FirstRun : public QWizard
{
Q_OBJECT

public:
    enum { Page_Intro, Page_Settings, Page_Conclusion };

    FirstRun(QWidget *parent = 0);
};


class IntroPage : public QWizardPage
{
Q_OBJECT

public:
    IntroPage(QWidget *parent = 0);

    int nextId() const;

private:
    QLabel *topLabel;
    QLabel *versionLabel;
};


class SettingsPage : public QWizardPage
{
Q_OBJECT

public:
    SettingsPage(QWidget *parent = 0);

    int nextId() const;

private slots:
	void setPlaylist();

private:
	QLabel *settingsLabel;
	QLabel *spacerLabel;
	QLabel *siol;
	QLabel *t2;
	QCheckBox *sessionBox;
	QCheckBox *vlcBox;
    QRadioButton *siolRadio2;
    QRadioButton *siolRadio4;
    QRadioButton *T2Radio;
    QRadioButton *T2RadioFull;
    QLineEdit *playlist;
    QLineEdit *type;
};


class ConclusionPage : public QWizardPage
{
Q_OBJECT

public:
    ConclusionPage(QWidget *parent = 0);

    int nextId() const;

private:
    QLabel *topLabel;
    QCheckBox *agreeCheckBox;
    License *license;
    QSettings *settings;
};

#endif // TANO_FIRSTRUN_H_
