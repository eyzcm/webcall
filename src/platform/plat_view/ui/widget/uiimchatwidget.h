#ifndef UIIMCHATWIDGET_H
#define UIIMCHATWIDGET_H

#include <QWidget>
#include <QSplitter>
#include "ui/widget/uisidebartreewidget.h"
#include "ui/widget/UICentralAreaWidget.h"
#include "ui/widget/rightside/uisidetabwidget.h"

#include "interface/uiyl_centercontentinterface.h"
#include "interface/uiyl_chattreeinterface.h"

class UIImChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UIImChatWidget(QWidget *parent = nullptr);
    ~UIImChatWidget() override;

private:
    QSplitter *m_splitter;
    UIYL_ChatTreeInterface *m_sidebarTreeWidget;
    UIYL_CenterContentInterface *m_centralAreaWidget;
    UISideTabWidget *m_sideTabWidget;
};

#endif // UICHATWIDGET_H
