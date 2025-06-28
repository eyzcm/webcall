#ifndef UIHEADERWIDGET_H
#define UIHEADERWIDGET_H

#include <QWidget>
#include "UIAvatarWidget.h"
#include "UIHeadToolbarWidget.h"
#include "UIHeadCustomFunctionWidget.h"
#include "uiyl/common/UIStatusWidgetPanel.h"
#include "uiyl/utils/UI_Widget_User_Panel.h"

class UIHeaderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UIHeaderWidget(QWidget *parent = nullptr);
    ~UIHeaderWidget();

private:
    UI_Widget_User_Panel *m_avatarWidget;

    UIHeadToolbarWidget *m_toolbarWidget;
    UIHeadCustomFunctionWidget *m_customFunctionWidget;

    void LoadStylesheet();
};

#endif // UIHEADERWIDGET_H
