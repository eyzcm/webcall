#ifndef UIMAINWIDGET_H
#define UIMAINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>

class UIHeaderWidget;
class UISidebarWidget;
class UIBottomWidget;

class UiMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UiMainWidget(QWidget *parent = nullptr);
    ~UiMainWidget();

private:
    UIHeaderWidget *m_headerWidget;
    UISidebarWidget *m_sidebarWidget;
    QStackedWidget *m_stackedWidget;
    UIBottomWidget *m_bottomWidget;

    void LoadStylesheet();
};


#endif // UIMAINWIDGET_H
