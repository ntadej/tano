/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
*
* Copyright (C) 2016 The Qt Company Ltd.
* Contact: https://www.qt.io/licensing/
*
* This file is based on file from Qt Creator.
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

#ifndef TANO_THEME_H_
#define TANO_THEME_H_

#include <QBrush> // QGradientStops
#include <QObject>

class QSettings;
class QPalette;

class ThemePrivate;

class Theme : public QObject
{
    Q_OBJECT

    Q_ENUMS(Color)
    Q_ENUMS(ImageFile)
    Q_ENUMS(Gradient)
    Q_ENUMS(Flag)
    Q_ENUMS(WidgetStyle)

public:
    Theme(const QString &id, QObject *parent = 0);
    ~Theme();

    enum Color {
        BackgroundColorAlternate,
        BackgroundColorDark,
        BackgroundColorHover,
        BackgroundColorNormal,
        BackgroundColorSelected,
        BackgroundColorDisabled,
        BadgeLabelBackgroundColorChecked,
        BadgeLabelBackgroundColorUnchecked,
        BadgeLabelTextColorChecked,
        BadgeLabelTextColorUnchecked,
        CanceledSearchTextColor,
        ComboBoxArrowColor,
        ComboBoxArrowColorDisabled,
        ComboBoxTextColor,
        DetailsButtonBackgroundColorHover,
        DetailsWidgetBackgroundColor,
        DockWidgetResizeHandleColor,
        DoubleTabWidget1stEmptyAreaBackgroundColor,
        DoubleTabWidget1stSeparatorColor,
        DoubleTabWidget1stTabActiveTextColor,
        DoubleTabWidget1stTabBackgroundColor,
        DoubleTabWidget1stTabInactiveTextColor,
        DoubleTabWidget2ndSeparatorColor,
        DoubleTabWidget2ndTabActiveTextColor,
        DoubleTabWidget2ndTabBackgroundColor,
        DoubleTabWidget2ndTabInactiveTextColor,
        EditorPlaceholderColor,
        FancyTabBarBackgroundColor,
        FancyTabWidgetDisabledSelectedTextColor,
        FancyTabWidgetDisabledUnselectedTextColor,
        FancyTabWidgetEnabledSelectedTextColor,
        FancyTabWidgetEnabledUnselectedTextColor,
        FancyToolButtonHoverColor,
        FancyToolButtonSelectedColor,
        FutureProgressBackgroundColor,
        InfoBarBackground,
        InfoBarText,
        MenuBarEmptyAreaBackgroundColor,
        MenuBarItemBackgroundColor,
        MenuBarItemTextColorDisabled,
        MenuBarItemTextColorNormal,
        MenuItemTextColorDisabled,
        MenuItemTextColorNormal,
        MiniProjectTargetSelectorBackgroundColor,
        MiniProjectTargetSelectorBorderColor,
        MiniProjectTargetSelectorSummaryBackgroundColor,
        MiniProjectTargetSelectorTextColor,
        OutputPaneButtonFlashColor,
        OutputPaneToggleButtonTextColorChecked,
        OutputPaneToggleButtonTextColorUnchecked,
        PanelButtonToolBackgroundColorHover,
        PanelButtonToolBackgroundColorHoverLight,
        PanelStatusBarBackgroundColor,
        PanelsWidgetSeparatorLineColor,
        PanelTextColorDark,
        PanelTextColorMid,
        PanelTextColorLight,
        ProgressBarColorError,
        ProgressBarColorFinished,
        ProgressBarColorNormal,
        ProgressBarTitleColor,
        SplitterColor,
        TextColorDisabled,
        TextColorError,
        TextColorHighlight,
        TextColorLink,
        TextColorLinkVisited,
        TextColorNormal,
        TodoItemTextColor,
        ToggleButtonBackgroundColor,
        ToolBarBackgroundColor,
        TreeViewArrowColorNormal,
        TreeViewArrowColorSelected,

        /* Icons */

        IconsBaseColor,
        IconsDisabledColor,
        IconsInfoColor,
        IconsInfoToolBarColor,
        IconsWarningColor,
        IconsWarningToolBarColor,
        IconsErrorColor,
        IconsErrorToolBarColor,
        IconsRunColor,
        IconsStopColor,
        IconsInterruptColor,
        IconsDebugColor,
        IconsNavigationArrowsColor,
        IconsBuildHammerHandleColor,
        IconsBuildHammerHeadColor,
        IconsModeWelcomeActiveColor,
        IconsModeEditActiveColor,
        IconsModeDesignActiveColor,
        IconsModeDebugActiveColor,
        IconsModeProjetcsActiveColor,
        IconsModeAnalyzeActiveColor,
        IconsModeHelpActiveColor,

        /* Output panes */

        OutputPanes_DebugTextColor,
        OutputPanes_ErrorMessageTextColor,
        OutputPanes_MessageOutput,
        OutputPanes_NormalMessageTextColor,
        OutputPanes_StdErrTextColor,
        OutputPanes_StdOutTextColor,
        OutputPanes_WarningMessageTextColor,

        /* Debugger Log Window */

        Debugger_LogWindow_LogInput,
        Debugger_LogWindow_LogStatus,
        Debugger_LogWindow_LogTime,

        /* Debugger Watch Item */

        Debugger_WatchItem_ValueNormal,
        Debugger_WatchItem_ValueInvalid,
        Debugger_WatchItem_ValueChanged,

        /* Welcome Plugin */

        Welcome_TextColorNormal,
        Welcome_TextColorHeading,  // #535353 // Sessions, Recent Projects
        Welcome_BackgroundColorNormal,   // #ffffff
        Welcome_DividerColor,      // #737373
        Welcome_Button_BorderColorNormal,
        Welcome_Button_BorderColorPressed,
        Welcome_Button_TextColorNormal,
        Welcome_Button_TextColorPressed,
        Welcome_Link_TextColorNormal,
        Welcome_Link_TextColorActive,
        Welcome_Link_BackgroundColor,
        Welcome_Caption_TextColorNormal,
        Welcome_SideBar_BackgroundColor,

        Welcome_ProjectItem_TextColorFilepath,
        Welcome_ProjectItem_BackgroundColorHover,

        Welcome_SessionItem_BackgroundColorNormal,
        Welcome_SessionItem_BackgroundColorHover,
        Welcome_SessionItemExpanded_BackgroundColorNormal,
        Welcome_SessionItemExpanded_BackgroundColorHover,

        /* VcsBase Plugin */
        VcsBase_FileStatusUnknown_TextColor,
        VcsBase_FileAdded_TextColor,
        VcsBase_FileModified_TextColor,
        VcsBase_FileDeleted_TextColor,
        VcsBase_FileRenamed_TextColor,

        /* Bookmarks Plugin */
        Bookmarks_TextMarkColor,

        /* TextEditor Plugin */
        TextEditor_SearchResult_ScrollBarColor,
        TextEditor_CurrentLine_ScrollBarColor,

        /* Debugger Plugin */
        Debugger_Breakpoint_TextMarkColor,

        /* ProjectExplorer Plugin */
        ProjectExplorer_TaskError_TextMarkColor,
        ProjectExplorer_TaskWarn_TextMarkColor,

        /* ClangCodeModel Plugin */
        ClangCodeModel_Error_TextMarkColor,
        ClangCodeModel_Warning_TextMarkColor
    };

    enum Gradient {
        DetailsWidgetHeaderGradient,
        Welcome_Button_GradientNormal,
        Welcome_Button_GradientPressed
    };

    enum ImageFile {
        ProjectExplorerHeader,
        ProjectExplorerSource,
        ProjectExplorerForm,
        ProjectExplorerResource,
        ProjectExplorerQML,
        ProjectExplorerOtherFiles,
        ProjectFileIcon,
        IconOverlayCSource,
        IconOverlayCppHeader,
        IconOverlayCppSource,
        IconOverlayPri,
        IconOverlayPrf,
        IconOverlayPro,
        StandardPixmapFileIcon,
        StandardPixmapDirIcon,
        BuildStepDisable,
        BuildStepRemove,
        BuildStepMoveDown,
        BuildStepMoveUp
    };

    enum Flag {
        DrawTargetSelectorBottom,
        DrawSearchResultWidgetFrame,
        DrawProgressBarSunken,
        DrawIndicatorBranch,
        ComboBoxDrawTextShadow,
        DerivePaletteFromTheme,
        ApplyThemePaletteGlobally,
        FlatSideBarIcons
    };

    enum WidgetStyle {
        StyleDefault,
        StyleFlat
    };

    WidgetStyle widgetStyle() const;
    bool flag(Flag f) const;
    QColor color(Color role) const;
    QString imageFile(ImageFile imageFile, const QString &fallBack) const;
    QGradientStops gradient(Gradient role) const;
    QPalette palette() const;
    QStringList preferredStyles() const;
    QString defaultTextEditorColorScheme() const;

    QString id() const;
    QString filePath() const;
    QString displayName() const;
    void setDisplayName(const QString &displayName);

    QVariantHash values() const;

    void readSettings(QSettings &settings);

    static QPalette initialPalette();

    ThemePrivate *d;

private:
    QPair<QColor, QString> readNamedColor(const QString &color) const;
};

namespace Tano {
Theme *applicationTheme();
}

#endif // TANO_THEME_H_
