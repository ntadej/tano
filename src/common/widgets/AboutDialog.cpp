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

#include <QtGui/QIcon>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

#include "application/Common.h"
#include "common/Backend.h"

#include "AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setMinimumSize(300, 415);
    setMaximumSize(300, 415);

    setWindowTitle(tr("About %1").arg(Tano::name()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(10 - layout->spacing());
    setLayout(layout);

    QLabel *logo = new QLabel(this);
    logo->setAlignment(Qt::AlignHCenter);
    logo->setPixmap(QIcon(":/logo/128x128/logo.png").pixmap(128));
    layout->addWidget(logo);
    layout->addSpacing(8);

    QLabel *name = new QLabel(this);
    name->setText(Tano::name());
    name->setAlignment(Qt::AlignHCenter);
    QFont title = name->font();
    title.setBold(true);
    title.setPixelSize(18);
    name->setFont(title);
    layout->addWidget(name);

    QLabel *version = new QLabel(this);
    version->setText(Tano::version() + " " + Tano::build());
    version->setAlignment(Qt::AlignHCenter);
    layout->addWidget(version);

    QLabel *copyright = new QLabel(this);
    copyright->setText(tr("Copyright %1 %2 %3<br>All rights reserved.").arg("©", QString::number(QDate::currentDate().year()), "Tadej Novak"));
    copyright->setAlignment(Qt::AlignHCenter);
    layout->addWidget(copyright);

    layout->addSpacing(10 - layout->spacing());

    QStringList backend =
            QStringList() << "<a href=\"https://vlc-qt.tano.si\">VLC-Qt</a> "
                          << Tano::Backend::versionLibrary()
                          << "<br>" << tr("Using VLC %1").arg(Tano::Backend::versionCore()) << "<br>"
                          << "© 2016 Tadej Novak "
                          << "(<a href=\"http://www.gnu.org/licenses/lgpl-3.0.html\">LGPL v3</a>)";

    QStringList icons =
            QStringList() << "<a href=\"http://snwh.org/paper/\">Paper</a>"
                          << " - © 2016 Sam Hewitt "
                          << "(<a href=\"http://creativecommons.org/licenses/by-sa/4.0/\">CC BY-SA 4.0</a>)";

    QStringList libraries =
            QStringList() << "<a href=\"http://www.qt.io\">Qt</a>"
                          << " - © 2016 The Qt Company "
                          << "(<a href=\"http://www.gnu.org/licenses/lgpl-3.0.html\">LGPL v3</a>)"
                          << "<br>"
                          << "<a href=\"https://sparkle-project.org\">Sparkle</a>"
                          << " - © 2015 Sparkle Project "
                          << "(<a href=\"https://github.com/sparkle-project/Sparkle/blob/master/LICENSE\">MIT</a>)"
                          << "<br>"
                          << "<a href=\"http://winsparkle.org\">WinSparkle</a>"
                          << " - © 2016 Vaclav Slavik "
                          << "(<a href=\"https://github.com/vslavik/winsparkle/blob/master/COPYING\">MIT</a>)";

    QStringList detailsContent =
            QStringList() << "<center>"
                          << "<p>"
                          << "<strong>" << tr("Backend") << "</strong><br>"
                          << backend.join("")
                          << "</p>"
                          << "<p>"
                          << "<strong>" << tr("Icons") << "</strong><br>"
                          << icons.join("")
                          << "</p>"
                          << "<p>"
                          << "<strong>" << tr("Libraries") << "</strong><br>"
                          << libraries.join("")
                          << "</p>"
                          << "</center>";

    QTextEdit *details = new QTextEdit(this);
    QFont content = details->font();
    content.setPixelSize(12);
    details->setReadOnly(true);
    details->setAcceptRichText(true);
    details->setFont(content);
    details->setHtml(detailsContent.join(""));
    layout->addWidget(details);

    layout->addStretch();
}
