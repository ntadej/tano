/********************************************************************************
** Form generated from reading ui file 'Siol.ui'
**
** Created: Sat Jan 17 10:15:55 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SIOL_H
#define UI_SIOL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <video/Video.h>

QT_BEGIN_NAMESPACE

class Ui_Siol
{
public:
    QAction *actionAbout;
    QAction *actionAbout_Qt;
    QAction *actionClose;
    QAction *actionSettings;
    QAction *actionFullscreen;
    QAction *actionOpen;
    QAction *actionOpenFile;
    QAction *actionBrowser;
    QAction *actionEditPlaylist;
    QAction *actionHelp;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *videoBox;
    QVBoxLayout *_2;
    Video *videoWidget;
    QFrame *frame;
    QHBoxLayout *videoControls;
    QPushButton *buttonPlay;
    QPushButton *buttonStop;
    QPushButton *buttonFull;
    QPushButton *buttonVlc;
    QSpacerItem *horizontalSpacer;
    QGroupBox *epgBox;
    QVBoxLayout *_3;
    QLabel *labelLanguage;
    QHBoxLayout *layoutNow;
    QLabel *labelNow;
    QPushButton *buttonRefresh;
    QPushButton *buttonRefreshToday;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QLabel *labelTodayEpg;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuAbout;
    QMenu *menuOptions;
    QStatusBar *statusbar;
    QToolBar *toolBar;
    QDockWidget *playlistWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_4;
    QTreeWidget *playlistTree;

    void setupUi(QMainWindow *Siol)
    {
    if (Siol->objectName().isEmpty())
        Siol->setObjectName(QString::fromUtf8("Siol"));
    Siol->resize(918, 756);
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/siol.png")), QIcon::Normal, QIcon::Off);
    Siol->setWindowIcon(icon);
    actionAbout = new QAction(Siol);
    actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
    actionAbout->setIcon(icon);
    actionAbout_Qt = new QAction(Siol);
    actionAbout_Qt->setObjectName(QString::fromUtf8("actionAbout_Qt"));
    QIcon icon1;
    icon1.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/qt.png")), QIcon::Normal, QIcon::Off);
    actionAbout_Qt->setIcon(icon1);
    actionClose = new QAction(Siol);
    actionClose->setObjectName(QString::fromUtf8("actionClose"));
    QIcon icon2;
    icon2.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/exit.png")), QIcon::Normal, QIcon::Off);
    actionClose->setIcon(icon2);
    actionSettings = new QAction(Siol);
    actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
    QIcon icon3;
    icon3.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/settings.png")), QIcon::Normal, QIcon::Off);
    actionSettings->setIcon(icon3);
    actionFullscreen = new QAction(Siol);
    actionFullscreen->setObjectName(QString::fromUtf8("actionFullscreen"));
    QIcon icon4;
    icon4.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/player_fullscreen.png")), QIcon::Normal, QIcon::Off);
    actionFullscreen->setIcon(icon4);
    actionFullscreen->setShortcutContext(Qt::ApplicationShortcut);
    actionOpen = new QAction(Siol);
    actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
    QIcon icon5;
    icon5.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/fileopen.png")), QIcon::Normal, QIcon::Off);
    actionOpen->setIcon(icon5);
    actionOpenFile = new QAction(Siol);
    actionOpenFile->setObjectName(QString::fromUtf8("actionOpenFile"));
    actionOpenFile->setIcon(icon5);
    actionBrowser = new QAction(Siol);
    actionBrowser->setObjectName(QString::fromUtf8("actionBrowser"));
    QIcon icon6;
    icon6.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/browser.png")), QIcon::Normal, QIcon::Off);
    actionBrowser->setIcon(icon6);
    actionEditPlaylist = new QAction(Siol);
    actionEditPlaylist->setObjectName(QString::fromUtf8("actionEditPlaylist"));
    QIcon icon7;
    icon7.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/edit.png")), QIcon::Normal, QIcon::Off);
    actionEditPlaylist->setIcon(icon7);
    actionHelp = new QAction(Siol);
    actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
    QIcon icon8;
    icon8.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/help.png")), QIcon::Normal, QIcon::Off);
    actionHelp->setIcon(icon8);
    actionHelp->setShortcutContext(Qt::ApplicationShortcut);
    centralwidget = new QWidget(Siol);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    verticalLayout = new QVBoxLayout(centralwidget);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    videoBox = new QGroupBox(centralwidget);
    videoBox->setObjectName(QString::fromUtf8("videoBox"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(videoBox->sizePolicy().hasHeightForWidth());
    videoBox->setSizePolicy(sizePolicy);
    _2 = new QVBoxLayout(videoBox);
    _2->setObjectName(QString::fromUtf8("_2"));
    videoWidget = new Video(videoBox);
    videoWidget->setObjectName(QString::fromUtf8("videoWidget"));

    _2->addWidget(videoWidget);

    frame = new QFrame(videoBox);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setMinimumSize(QSize(0, 64));
    frame->setMaximumSize(QSize(16777215, 64));
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    videoControls = new QHBoxLayout(frame);
    videoControls->setObjectName(QString::fromUtf8("videoControls"));
    videoControls->setContentsMargins(-1, 2, -1, 2);
    buttonPlay = new QPushButton(frame);
    buttonPlay->setObjectName(QString::fromUtf8("buttonPlay"));
    buttonPlay->setMinimumSize(QSize(46, 46));
    buttonPlay->setMaximumSize(QSize(46, 46));
    QIcon icon9;
    icon9.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/player_play.png")), QIcon::Normal, QIcon::Off);
    buttonPlay->setIcon(icon9);
    buttonPlay->setIconSize(QSize(32, 32));

    videoControls->addWidget(buttonPlay);

    buttonStop = new QPushButton(frame);
    buttonStop->setObjectName(QString::fromUtf8("buttonStop"));
    buttonStop->setMinimumSize(QSize(46, 46));
    buttonStop->setMaximumSize(QSize(46, 46));
    QIcon icon10;
    icon10.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/player_stop.png")), QIcon::Normal, QIcon::Off);
    buttonStop->setIcon(icon10);
    buttonStop->setIconSize(QSize(32, 32));

    videoControls->addWidget(buttonStop);

    buttonFull = new QPushButton(frame);
    buttonFull->setObjectName(QString::fromUtf8("buttonFull"));
    buttonFull->setMinimumSize(QSize(46, 46));
    buttonFull->setMaximumSize(QSize(46, 46));
    buttonFull->setIcon(icon4);
    buttonFull->setIconSize(QSize(32, 32));

    videoControls->addWidget(buttonFull);

    buttonVlc = new QPushButton(frame);
    buttonVlc->setObjectName(QString::fromUtf8("buttonVlc"));
    buttonVlc->setMinimumSize(QSize(46, 46));
    buttonVlc->setMaximumSize(QSize(46, 46));
    QIcon icon11;
    icon11.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/vlc.png")), QIcon::Normal, QIcon::Off);
    buttonVlc->setIcon(icon11);
    buttonVlc->setIconSize(QSize(32, 32));

    videoControls->addWidget(buttonVlc);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    videoControls->addItem(horizontalSpacer);


    _2->addWidget(frame);


    verticalLayout->addWidget(videoBox);

    epgBox = new QGroupBox(centralwidget);
    epgBox->setObjectName(QString::fromUtf8("epgBox"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(epgBox->sizePolicy().hasHeightForWidth());
    epgBox->setSizePolicy(sizePolicy1);
    _3 = new QVBoxLayout(epgBox);
    _3->setObjectName(QString::fromUtf8("_3"));
    labelLanguage = new QLabel(epgBox);
    labelLanguage->setObjectName(QString::fromUtf8("labelLanguage"));

    _3->addWidget(labelLanguage);

    layoutNow = new QHBoxLayout();
    layoutNow->setObjectName(QString::fromUtf8("layoutNow"));
    labelNow = new QLabel(epgBox);
    labelNow->setObjectName(QString::fromUtf8("labelNow"));

    layoutNow->addWidget(labelNow);

    buttonRefresh = new QPushButton(epgBox);
    buttonRefresh->setObjectName(QString::fromUtf8("buttonRefresh"));
    buttonRefresh->setMaximumSize(QSize(200, 24));

    layoutNow->addWidget(buttonRefresh);

    buttonRefreshToday = new QPushButton(epgBox);
    buttonRefreshToday->setObjectName(QString::fromUtf8("buttonRefreshToday"));
    buttonRefreshToday->setMaximumSize(QSize(200, 24));

    layoutNow->addWidget(buttonRefreshToday);


    _3->addLayout(layoutNow);

    scrollArea = new QScrollArea(epgBox);
    scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
    scrollArea->setMinimumSize(QSize(0, 50));
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidgetResizable(true);
    scrollAreaWidgetContents_2 = new QWidget();
    scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
    scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 625, 60));
    verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents_2);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    verticalLayout_2->setContentsMargins(-1, 0, -1, 0);
    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_2->addItem(verticalSpacer);

    labelTodayEpg = new QLabel(scrollAreaWidgetContents_2);
    labelTodayEpg->setObjectName(QString::fromUtf8("labelTodayEpg"));
    labelTodayEpg->setMinimumSize(QSize(0, 0));

    verticalLayout_2->addWidget(labelTodayEpg);

    scrollArea->setWidget(scrollAreaWidgetContents_2);

    _3->addWidget(scrollArea);


    verticalLayout->addWidget(epgBox);

    Siol->setCentralWidget(centralwidget);
    menubar = new QMenuBar(Siol);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 918, 25));
    menuFile = new QMenu(menubar);
    menuFile->setObjectName(QString::fromUtf8("menuFile"));
    menuAbout = new QMenu(menubar);
    menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
    menuOptions = new QMenu(menubar);
    menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
    Siol->setMenuBar(menubar);
    statusbar = new QStatusBar(Siol);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    Siol->setStatusBar(statusbar);
    toolBar = new QToolBar(Siol);
    toolBar->setObjectName(QString::fromUtf8("toolBar"));
    toolBar->setMovable(false);
    toolBar->setIconSize(QSize(32, 32));
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->setFloatable(false);
    Siol->addToolBar(Qt::TopToolBarArea, toolBar);
    playlistWidget = new QDockWidget(Siol);
    playlistWidget->setObjectName(QString::fromUtf8("playlistWidget"));
    playlistWidget->setMaximumSize(QSize(452, 524287));
    playlistWidget->setFloating(true);
    playlistWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dockWidgetContents = new QWidget();
    dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
    verticalLayout_4 = new QVBoxLayout(dockWidgetContents);
    verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
    playlistTree = new QTreeWidget(dockWidgetContents);
    playlistTree->setObjectName(QString::fromUtf8("playlistTree"));
    playlistTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    playlistTree->setAnimated(true);
    playlistTree->setColumnCount(1);

    verticalLayout_4->addWidget(playlistTree);

    playlistWidget->setWidget(dockWidgetContents);
    Siol->addDockWidget(static_cast<Qt::DockWidgetArea>(2), playlistWidget);

    menubar->addAction(menuFile->menuAction());
    menubar->addAction(menuOptions->menuAction());
    menubar->addAction(menuAbout->menuAction());
    menuFile->addAction(actionOpenFile);
    menuFile->addAction(actionOpen);
    menuFile->addSeparator();
    menuFile->addAction(actionBrowser);
    menuFile->addSeparator();
    menuFile->addAction(actionClose);
    menuAbout->addAction(actionHelp);
    menuAbout->addSeparator();
    menuAbout->addAction(actionAbout);
    menuAbout->addAction(actionAbout_Qt);
    menuOptions->addAction(actionFullscreen);
    menuOptions->addSeparator();
    menuOptions->addAction(actionEditPlaylist);
    menuOptions->addAction(actionSettings);
    toolBar->addAction(actionOpenFile);
    toolBar->addAction(actionOpen);
    toolBar->addSeparator();
    toolBar->addAction(actionBrowser);
    toolBar->addSeparator();
    toolBar->addAction(actionEditPlaylist);
    toolBar->addAction(actionSettings);

    retranslateUi(Siol);

    QMetaObject::connectSlotsByName(Siol);
    } // setupUi

    void retranslateUi(QMainWindow *Siol)
    {
    Siol->setWindowTitle(QApplication::translate("Siol", "SiOL TV", 0, QApplication::UnicodeUTF8));
    actionAbout->setText(QApplication::translate("Siol", "About Siol TV", 0, QApplication::UnicodeUTF8));
    actionAbout_Qt->setText(QApplication::translate("Siol", "About Qt", 0, QApplication::UnicodeUTF8));
    actionClose->setText(QApplication::translate("Siol", "&Exit", 0, QApplication::UnicodeUTF8));
    actionSettings->setText(QApplication::translate("Siol", "&Settings", 0, QApplication::UnicodeUTF8));
    actionFullscreen->setText(QApplication::translate("Siol", "Fullscreen", 0, QApplication::UnicodeUTF8));
    actionFullscreen->setShortcut(QApplication::translate("Siol", "Ctrl+F", 0, QApplication::UnicodeUTF8));
    actionOpen->setText(QApplication::translate("Siol", "&Open Channel list", 0, QApplication::UnicodeUTF8));
    actionOpenFile->setText(QApplication::translate("Siol", "Open", 0, QApplication::UnicodeUTF8));
    actionBrowser->setText(QApplication::translate("Siol", "EPG Browser", 0, QApplication::UnicodeUTF8));
    actionEditPlaylist->setText(QApplication::translate("Siol", "Edit Channel list", 0, QApplication::UnicodeUTF8));
    actionHelp->setText(QApplication::translate("Siol", "&Help", 0, QApplication::UnicodeUTF8));
    actionHelp->setShortcut(QApplication::translate("Siol", "F1", 0, QApplication::UnicodeUTF8));
    videoBox->setTitle(QApplication::translate("Siol", "Video", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    buttonPlay->setToolTip(QApplication::translate("Siol", "Play", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    buttonStop->setToolTip(QApplication::translate("Siol", "Stop", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    buttonFull->setToolTip(QApplication::translate("Siol", "Fullscreen", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    buttonFull->setShortcut(QApplication::translate("Siol", "Ctrl+F", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    buttonVlc->setToolTip(QApplication::translate("Siol", "Play with VLC", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    epgBox->setTitle(QString());
    labelLanguage->setText(QString());
    labelNow->setText(QString());
    buttonRefresh->setText(QApplication::translate("Siol", "Refresh Currently playing", 0, QApplication::UnicodeUTF8));
    buttonRefreshToday->setText(QApplication::translate("Siol", "Refresh Today", 0, QApplication::UnicodeUTF8));
    labelTodayEpg->setText(QString());
    menuFile->setTitle(QApplication::translate("Siol", "&File", 0, QApplication::UnicodeUTF8));
    menuAbout->setTitle(QApplication::translate("Siol", "&Help", 0, QApplication::UnicodeUTF8));
    menuOptions->setTitle(QApplication::translate("Siol", "&Tools", 0, QApplication::UnicodeUTF8));
    playlistWidget->setWindowTitle(QApplication::translate("Siol", "Channel list", 0, QApplication::UnicodeUTF8));
    playlistTree->headerItem()->setText(0, QApplication::translate("Siol", "Channel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Siol: public Ui_Siol {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIOL_H
