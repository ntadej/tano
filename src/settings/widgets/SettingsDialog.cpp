/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "application/LocaleManager.h"
#include "settings/Settings.h"
#include "settings/SettingsShortcutsDesktop.h"
#include "settings/widgets/SettingsKeyDelegate.h"
#include "settings/widgets/SettingsNotEditableDelegate.h"

#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#if defined(Q_OS_MAC) && MAC_NATIVE_TOOLBAR
    #include <Cocoa/Cocoa.h>
#endif

SettingsDialog::SettingsDialog(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::SettingsDialog),
      _shortcuts(new SettingsShortcutsDesktop())
{
    ui->setupUi(this);
    ui->toolBar->setProperty("custom_style_disabled", true);

    _group = new QActionGroup(this);
    _group->addAction(ui->actionGeneral);
    _group->addAction(ui->actionTv);
    _group->addAction(ui->actionPlayback);
    _group->addAction(ui->actionInterface);
    _group->addAction(ui->actionShortcuts);

    setWindowTitle(ui->actionGeneral->text());
    setWindowFlags(Qt::Window
                 | Qt::WindowTitleHint
                 | Qt::WindowCloseButtonHint
                 | Qt::CustomizeWindowHint);

#if defined(Q_OS_MAC) && MAC_NATIVE_TOOLBAR
    removeToolBar(ui->toolBar);

    _macToolbar = new QMacToolBar(this);

    _macItemGeneral = _macToolbar->addItem(QIcon::fromTheme("preferences-system"), tr("General"));
    _macItemGeneral->setSelectable(true);
    connect(_macItemGeneral, &QMacToolBarItem::activated, this, &SettingsDialog::actionToggled);

    _macItemTv = _macToolbar->addItem(QIcon::fromTheme("video-television"), tr("TV"));
    _macItemTv->setSelectable(true);
    connect(_macItemTv, &QMacToolBarItem::activated, this, &SettingsDialog::actionToggled);

    _macItemPlayback = _macToolbar->addItem(QIcon::fromTheme("preferences-desktop-multimedia"), tr("Playback"));
    _macItemPlayback->setSelectable(true);
    connect(_macItemPlayback, &QMacToolBarItem::activated, this, &SettingsDialog::actionToggled);

    _macItemInterface = _macToolbar->addItem(QIcon::fromTheme("preferences-system-windows-actions"), tr("Interface"));
    _macItemInterface->setSelectable(true);
    connect(_macItemInterface, &QMacToolBarItem::activated, this, &SettingsDialog::actionToggled);

    _macItemShortcuts = _macToolbar->addItem(QIcon::fromTheme("preferences-desktop-keyboard"), tr("Shortcuts"));
    _macItemShortcuts->setSelectable(true);
    connect(_macItemShortcuts, &QMacToolBarItem::activated, this, &SettingsDialog::actionToggled);

    window()->winId();
    _macToolbar->attachToWindow(this->window()->windowHandle());

    NSToolbar *toolbar = _macToolbar->nativeToolbar();
    NSToolbarItem *toolbarItem = _macItemGeneral->nativeToolBarItem();
    NSString *toolbarItemId = [toolbarItem itemIdentifier];
    [toolbar setSelectedItemIdentifier:toolbarItemId];

    NSView *view = (NSView *)window()->winId();
    NSWindow *window = [view window];
    NSRect frame = [window frame];
    int extraHeight = frame.size.height - NSHeight([[window contentView] frame]);
    frame.origin.y += frame.size.height;
    frame.origin.y -= ui->pageGeneral->sizeHint().height() + extraHeight;
    frame.size.height = ui->pageGeneral->sizeHint().height() + extraHeight;
    [window setFrame:frame display:YES];
#else
    connect(ui->actionGeneral, &QAction::toggled, this, &SettingsDialog::actionToggled);
    connect(ui->actionTv, &QAction::toggled, this, &SettingsDialog::actionToggled);
    connect(ui->actionPlayback, &QAction::toggled, this, &SettingsDialog::actionToggled);
    connect(ui->actionInterface, &QAction::toggled, this, &SettingsDialog::actionToggled);
    connect(ui->actionShortcuts, &QAction::toggled, this, &SettingsDialog::actionToggled);
#endif

    loadLocales();

    QScopedPointer<Settings> settings(new Settings(this));
    ui->checkAutoPlay->setChecked(settings->autoplayLast());
    ui->checkMuteMinimize->setChecked(settings->muteOnMinimize());
    ui->browseRecordings->setValue(settings->recordingsDirectory());
    ui->browseSnapshots->setValue(settings->snapshotsDirectory());

    connect(ui->checkAutoPlay, &QCheckBox::toggled, this, &SettingsDialog::generalSettingsChanged);
    connect(ui->checkMuteMinimize, &QCheckBox::toggled, this, &SettingsDialog::generalSettingsChanged);
    connect(ui->browseRecordings, &BrowseWidget::valueChanged, this, &SettingsDialog::generalSettingsChanged);
    connect(ui->browseSnapshots, &BrowseWidget::valueChanged, this, &SettingsDialog::generalSettingsChanged);

#ifndef Q_OS_MAC
    ui->checkTray->setChecked(settings->trayEnabled());
    ui->checkTrayHide->setChecked(settings->hideToTray());
    connect(ui->checkTray, &QCheckBox::toggled, this, &SettingsDialog::guiSettingsChanged);
    connect(ui->checkTrayHide, &QCheckBox::toggled, this, &SettingsDialog::guiSettingsChanged);
#else
    ui->labelTray->hide();
    ui->checkTray->hide();
    ui->checkTrayHide->hide();
    ui->interfaceLayout->removeWidget(ui->labelTray);
    ui->interfaceLayout->removeWidget(ui->checkTray);
    ui->interfaceLayout->removeWidget(ui->checkTrayHide);
#endif

    if (settings->mouseWheel() == "volume") {
        ui->radioWheelVolume->setChecked(true);
    } else {
        ui->radioWheelChannel->setChecked(true);
    }
    connect(ui->radioWheelVolume, &QRadioButton::toggled, this, &SettingsDialog::guiSettingsChanged);

    connect(ui->pageTv, &SettingsTv::channelsChanged, this, &SettingsDialog::channelsChanged);
    connect(ui->pageTv, &SettingsTv::udpxyChanged, this, &SettingsDialog::udpxyChanged);
    connect(ui->pageTv, &SettingsTv::xmltvChanged, this, &SettingsDialog::xmltvChanged);
    connect(ui->pagePlayback, &SettingsPlayback::playbackDefaultsChanged, this, &SettingsDialog::playbackDefaultsChanged);
    connect(ui->pagePlayback, &SettingsPlayback::playbackMiscChanged, this, &SettingsDialog::playbackMiscChanged);
    connect(ui->pagePlayback, &SettingsPlayback::playbackRememberChanged, this, &SettingsDialog::playbackRememberChanged);

    // Shortcuts
    ui->shortcutsView->setItemDelegateForColumn(0, new SettingsNotEditableDelegate);
    ui->shortcutsView->setItemDelegateForColumn(1, new SettingsKeyDelegate);
    ui->shortcutsView->setModel(_shortcuts->model());
    ui->shortcutsView->header()->setStretchLastSection(false);
    ui->shortcutsView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->shortcutsView->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->shortcutsView->header()->hideSection(2);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;

    delete _group;
}

void SettingsDialog::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        loadLocales();
        break;
    default:
        break;
    }
}

void SettingsDialog::refreshAndShow()
{
    show();
    raise();
    activateWindow();
}

void SettingsDialog::actionToggled()
{
#if defined(Q_OS_MAC) && MAC_NATIVE_TOOLBAR
    NSView *view = (NSView *)window()->winId();
    NSWindow *window = [view window];
    NSRect frame = [window frame];

    int extraHeight = frame.size.height - NSHeight([[window contentView] frame]);
    int stackHeight = 0;
    QMacToolBarItem *item = qobject_cast<QMacToolBarItem *>(sender());
    ui->stackedWidget->hide();
    if (item == _macItemGeneral) {
        ui->stackedWidget->setCurrentWidget(ui->pageGeneral);
        stackHeight = ui->pageGeneral->sizeHint().height();
    } else if (item == _macItemTv) {
        ui->stackedWidget->setCurrentWidget(ui->pageTv);
        stackHeight = ui->pageTv->sizeHint().height();
    } else if (item == _macItemPlayback) {
        ui->stackedWidget->setCurrentWidget(ui->pagePlayback);
        stackHeight = ui->pagePlayback->sizeHint().height();
    } else if (item == _macItemInterface) {
        ui->stackedWidget->setCurrentWidget(ui->pageInterface);
        stackHeight = ui->pageInterface->sizeHint().height();
    } else if (item == _macItemShortcuts) {
        ui->stackedWidget->setCurrentWidget(ui->pageShortcuts);
        stackHeight = ui->pageShortcuts->sizeHint().height();
    }

    frame.origin.y += frame.size.height;
    frame.origin.y -= stackHeight + extraHeight;
    frame.size.height = stackHeight + extraHeight;

    [window setFrame:frame display:YES animate:YES];

    setWindowTitle(item->text());
    ui->stackedWidget->show();
#else
    QAction *action = qobject_cast<QAction *>(sender());

    setWindowTitle(action->text());
    if (action == ui->actionGeneral) {
        ui->stackedWidget->setCurrentWidget(ui->pageGeneral);
    } else if (action == ui->actionInterface) {
        ui->stackedWidget->setCurrentWidget(ui->pageInterface);
    } else if (action == ui->actionTv) {
        ui->stackedWidget->setCurrentWidget(ui->pageTv);
    } else if (action == ui->actionPlayback) {
        ui->stackedWidget->setCurrentWidget(ui->pagePlayback);
    } else if (action == ui->actionShortcuts) {
        ui->stackedWidget->setCurrentWidget(ui->pageShortcuts);
    }
#endif
}

void SettingsDialog::localeSettingsChanged()
{
    QScopedPointer<Settings> settings(new Settings(this));
    if (ui->comboLocale->currentIndex() == 0) {
        settings->setLocale("");
    } else {
        settings->setLocale(_locales[ui->comboLocale->currentIndex() - 1]);
    }
    settings->writeSettings();

    emit localeChanged();
}

void SettingsDialog::generalSettingsChanged()
{
    QScopedPointer<Settings> settings(new Settings(this));
    settings->setAutoplayLast(ui->checkAutoPlay->isChecked());
    settings->setMuteOnMinimize(ui->checkMuteMinimize->isChecked());
    settings->setSnapshotsDirectory(ui->browseSnapshots->value());
    settings->setRecordingsDirectory(ui->browseRecordings->value());
    settings->writeSettings();

    emit generalChanged();
}

void SettingsDialog::guiSettingsChanged()
{
    QScopedPointer<Settings> settings(new Settings(this));
#ifndef Q_OS_MAC
    settings->setTrayEnabled(ui->checkTray->isChecked());
    settings->setHideToTray(ui->checkTrayHide->isChecked());
#endif
    if (ui->radioWheelVolume->isChecked()) {
        settings->setMouseWheel("volume");
    } else {
        settings->setMouseWheel("channel");
    }
    settings->writeSettings();

    emit guiChanged();
}

void SettingsDialog::loadLocales()
{
    QScopedPointer<Settings> settings(new Settings(this));

    disconnect(ui->comboLocale, &QComboBox::currentTextChanged, this, &SettingsDialog::localeSettingsChanged);

    _locales = LocaleManager::loadLocales();

    ui->comboLocale->clear();
    ui->comboLocale->addItem(tr("System default"));
    for (const QString locale : _locales) {
        ui->comboLocale->addItem(QLocale(locale).nativeLanguageName());
        if (settings->locale() == locale) {
            ui->comboLocale->setCurrentIndex(ui->comboLocale->count() - 1);
        }
    }

    connect(ui->comboLocale, &QComboBox::currentTextChanged, this, &SettingsDialog::localeSettingsChanged);
}
