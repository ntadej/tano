/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_SETTINGSBACKEND_H_
#define TANO_SETTINGSBACKEND_H_

#if defined(Qt5)
    #include <QtWidgets/QWidget>
#elif defined(Qt4)
    #include <QtGui/QWidget>
#endif

namespace Ui
{
    class SettingsBackend;
}

class SettingsBackend : public QWidget
{
Q_OBJECT
public:
    SettingsBackend(QWidget *parent = 0);
    ~SettingsBackend();

    int vout() const;
    void setVout(const int &id);
    int aout() const;
    void setAout(const int &id);
#if defined(Q_OS_WIN32)
    bool yuvToRgb() const;
    void setYuvToRgb(const bool &enabled);
#endif
    bool spdif() const;
    void setSpdif(const bool &enabled);
    int interface() const;
    void setInterface(const int &index);

    bool rememberChannelSettings() const;
    void setRememberChannelSettings(const bool &remember);
    bool rememberPerChannel() const;
    void setRememberPerChannel(const bool &remember);
    int aspectRatio() const;
    void setAspectRatio(const int &id);
    int cropRatio() const;
    void setCropRatio(const int &id);
    int deinterlacing() const;
    void setDeinterlacing(const int &id);
    QString audio() const;
    void setAudio(const QString &audio);
    QString sub() const;
    void setSub(const QString &sub);

    bool mute() const;
    void setMute(const bool &enabled);
    bool teletext() const;
    void setTeletext(const bool &enabled);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SettingsBackend *ui;
};

#endif // TANO_SETTINGSBACKEND_H_
