#ifndef UIMenuUserStateButton_H
#define UIMenuUserStateButton_H
#include <qobject.h>


QT_BEGIN_NAMESPACE
namespace Ui { class UIMenuUserStateButton; }
QT_END_NAMESPACE

#include <QMenu>
#include <QWidget>

class UIMenuUserStateButton : public QWidget
{
    Q_OBJECT

public:
    UIMenuUserStateButton(QWidget* parent = 0);
    ~UIMenuUserStateButton();
    void SetMenuButtonStyle(const QString& style);
    void SetMainBtn(const QString& iconUri, const QString& text, Qt::ToolButtonStyle toolButtonStyle = Qt::ToolButtonIconOnly);
    void SetArrawBtnIcon(const QString& iconUri);
    void SetArrawBtnVisible(bool isVisible);
    void SetMainBtnStyle(const QString& style);
    void SetArrawBtnStyle(const QString& style);
    void SetContentStyle(const QString& style);
    void SetMainBtnIconSize(int width, int height);
    void SetMenu(QMenu* menu);
    void ShowMenu();

signals:
    void S_MainBtnClicked();
    void S_ArrawBtnClicked();
    void S_MoveOnButton(bool isOn);

private:
    Ui::UIMenuUserStateButton * ui;
    QMenu*                    m_menu;
};

#endif // UIMenuUserStateButton_H
