#ifndef UIWEBTABWIDGET_H
#define UIWEBTABWIDGET_H

#include <QTabWidget>
#include <QMenu>

class UIWebTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit UIWebTabWidget(QWidget *parent = nullptr);

    // 添加Tab
    void AddTab(const QString &title, QWidget *content);

    // 设置右键菜单
    void SetCustomContextMenu(QMenu *menu);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QMenu *m_customMenu;
};

#endif // UIWEBTABWIDGET_H

