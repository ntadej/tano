/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#include "Branding.h"
#include "ui_MainWindow.h"
#include "ui_SettingsDialog.h"

void Tano::Branding::processMenus(Ui::MainWindow *ui,
                                  QMenu *right)
{
    ui->actionOpen->setDisabled(true);
    ui->actionOpenFile->setDisabled(true);
    ui->actionOpenToolbar->setDisabled(true);
    ui->actionOpenUrl->setDisabled(true);

    ui->menuFile->removeAction(ui->actionOpen);
    ui->menuFile->removeAction(ui->actionOpenFile);
    ui->menuFile->removeAction(ui->actionOpenUrl);

    ui->actionInfoPanel->setDisabled(true);
    ui->actionControls->setDisabled(true);
    ui->actionPreview->setDisabled(true);

    ui->menuView->removeAction(ui->actionInfoPanel);
    ui->menuView->removeAction(ui->actionControls);
    ui->menuMedia->removeAction(ui->actionPreview);

    ui->actionEditPlaylist->setDisabled(true);
    ui->actionSettingsShortcuts->setDisabled(true);
    ui->menubar->removeAction(ui->menuOptions->menuAction());

    ui->menuFile->insertAction(ui->actionExit, ui->actionSettings);
    ui->menuAbout->removeAction(ui->actionDonate);

    ui->toolBar->removeAction(ui->actionOpenToolbar);
    ui->toolBar->removeAction(ui->actionEditPlaylist);
    ui->toolBar->hide();

    right->removeAction(ui->actionInfoPanel);
    right->removeAction(ui->actionControls);
    right->removeAction(ui->menuAudio->menuAction());
    right->removeAction(ui->menuVideo->menuAction());

    ui->buttonPlaylistClose->setDisabled(true);
    ui->buttonScheduleClose->setDisabled(true);
    ui->buttonPlaylistClose->hide();
    ui->buttonScheduleClose->hide();
}

void Tano::Branding::processSettings(Ui::SettingsDialog *ui)
{
    ui->buttonPlaylist->setDisabled(true);
    ui->buttonRecorder->setDisabled(true);
    ui->buttonSchedule->setDisabled(true);
    ui->buttonPlaylist->hide();
    ui->buttonRecorder->hide();
    ui->buttonSchedule->hide();
}
