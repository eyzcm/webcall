#ifndef UIAVATARWIDGET_H
#define UIAVATARWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
class UIStatusWidgetPanel;

class UIAvatarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UIAvatarWidget(QWidget *parent = nullptr);
    ~UIAvatarWidget();

    void SetAvatarImage(const QPixmap &pixmap);
    void SetLoginName(const QString &name);
    QString GetCurrentStatus() const;

private:
    QLabel *m_avatarImageLabel;
    QLabel *m_loginNameLabel;
    UIStatusWidgetPanel *m_statusComboBox;

    void LoadStylesheet();
    void InitializeStatusOptions();
};

#endif // UIAVATARWIDGET_H
