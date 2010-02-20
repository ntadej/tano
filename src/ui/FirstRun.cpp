/****************************************************************************
* FirstRun.cpp: First run wizard
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
*****************************************************************************/

#include <QtGui/QLayout>

#include "FirstRun.h"
#include "../Common.h"
#include "../Ver.h"

FirstRun::FirstRun(QWidget *parent)
	: QWizard(parent)
{
	setPage(Page_Intro, new IntroPage);
	setPage(Page_Settings, new SettingsPage);
	setPage(Page_Conclusion, new ConclusionPage);

	setStartId(Page_Intro);

#ifndef Q_WS_MAC
	setWizardStyle(ModernStyle);
#endif

	setPixmap(QWizard::LogoPixmap, QPixmap(":/icons/images/tano-small.png"));
	setWindowTitle(tr("First Run Wizard"));
}

FirstRun::~FirstRun()
{

}

IntroPage::IntroPage(QWidget *parent)
	: QWizardPage(parent)
{
	setTitle(tr("Welcome"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/icons/images/wizard.bmp"));

	_topLabel = new QLabel(tr("This wizard will help you set basic settings for your copy of <i>Tano</i>."));
	_topLabel->setWordWrap(true);

	_settings = Common::settings();

	if(_settings->value("version", Version::Tano()).toString() != Version::Tano()) {
		_versionLabel = new QLabel(tr("You previously used version %1 of <i>Tano</i>. Please re-set your settings.").arg(_settings->value("version", Version::Tano()).toString()));
		_versionLabel->setWordWrap(true);
	} else
		_versionLabel = new QLabel("");

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(_topLabel);
	layout->addWidget(_versionLabel);
	setLayout(layout);
}

IntroPage::~IntroPage()
{
	delete _topLabel;
	delete _versionLabel;
	delete _settings;
}

int IntroPage::nextId() const
{
	return FirstRun::Page_Settings;
}

SettingsPage::SettingsPage(QWidget *parent)
	: QWizardPage(parent)
{
	setTitle(tr("Set <i>Tano</i>"));
	setSubTitle(tr("Please select your IP TV provider with playlist and other settings."));

	_playlist = new QLineEdit;
	_type = new QLineEdit;

	_spacerLabel = new QLabel("");
	_settingsLabel = new QLabel(tr("Quick settings:"));
	_siol = new QLabel("SiOL");
	_t2 = new QLabel("T-2");

	_siolRadio2 = new QRadioButton(tr("SiOL MPEG-2"));
	_siolRadio4 = new QRadioButton(tr("SiOL MPEG-4"));
	_T2Radio = new QRadioButton(tr("T-2 Categorised"));
	_T2RadioFull = new QRadioButton(tr("T-2 Full"));
	_sessionBox = new QCheckBox(tr("Enable Session store"));
	_vlcBox = new QCheckBox(tr("Use global VLC configuration"));

	connect(_siolRadio2, SIGNAL(clicked()), this, SLOT(setPlaylist()));
	connect(_siolRadio4, SIGNAL(clicked()), this, SLOT(setPlaylist()));
	connect(_T2Radio, SIGNAL(clicked()), this, SLOT(setPlaylist()));
	connect(_T2RadioFull, SIGNAL(clicked()), this, SLOT(setPlaylist()));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(_siol);
	layout->addWidget(_siolRadio2);
	layout->addWidget(_siolRadio4);
	layout->addWidget(_t2);
	layout->addWidget(_T2Radio);
	layout->addWidget(_T2RadioFull);
	layout->insertSpacerItem(6,new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Expanding));
	layout->addWidget(_spacerLabel);
	layout->addWidget(_settingsLabel);
	layout->addWidget(_sessionBox);
	layout->addWidget(_vlcBox);
	setLayout(layout);

	registerField("playlist*", _playlist);
	registerField("type*", _type);
	registerField("session", _sessionBox);
	registerField("vlc", _vlcBox);
}

SettingsPage::~SettingsPage()
{
	delete _settingsLabel;
	delete _spacerLabel;
	delete _siol;
	delete _t2;
	delete _sessionBox;
	delete _vlcBox;
	delete _siolRadio2;
	delete _siolRadio4;
	delete _T2Radio;
	delete _T2RadioFull;
	delete _playlist;
	delete _type;
}

int SettingsPage::nextId() const
{
	return FirstRun::Page_Conclusion;
}

void SettingsPage::setPlaylist()
{
	if(_siolRadio2->isChecked()) {
		_playlist->setText("playlists/siol-mpeg2.m3u");
		_type->setText(_siolRadio2->text());
	} else if(_siolRadio4->isChecked()) {
		_playlist->setText("playlists/siol-mpeg4.m3u");
		_type->setText(_siolRadio4->text());
	} else if(_T2Radio->isChecked()) {
		_playlist->setText("playlists/t-2-cat.m3u");
		_type->setText(_T2Radio->text());
	} else if(_T2RadioFull->isChecked()) {
		_playlist->setText("playlists/t-2-full.m3u");
		_type->setText(_T2RadioFull->text());
	}
}

ConclusionPage::ConclusionPage(QWidget *parent)
	: QWizardPage(parent)
{
	setTitle(tr("Complete Wizard"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/icons/images/wizard.bmp"));

	_settings = Common::settings();

	_topLabel = new QLabel(tr("Thank you for using <i>Tano</i>."));
	_topLabel->setWordWrap(true);

	_license = new License(this);

	_agreeCheckBox = new QCheckBox(tr("I agree to the terms of the GNU General Public License"));

	registerField("agree*", _agreeCheckBox);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(_topLabel);
	layout->insertSpacerItem(1,new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Expanding));
	layout->addWidget(_license);
	layout->addWidget(_agreeCheckBox);
	setLayout(layout);
}

ConclusionPage::~ConclusionPage()
{
	delete _topLabel;
	delete _agreeCheckBox;
	delete _license;
	delete _settings;
}

int ConclusionPage::nextId() const
{
	_settings->setValue("version",Version::Tano());
	_settings->setValue("registered",true);
	_settings->setValue("playlist",field("playlist").toString());
	_settings->setValue("session",field("session").toBool());

	_settings->beginGroup("VLC");
	_settings->setValue("ignore-config",!field("vlc").toBool());
	_settings->endGroup();

	return -1;
}

