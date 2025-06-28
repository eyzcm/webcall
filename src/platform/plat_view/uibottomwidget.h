#ifndef UIBOTTOMWIDGET_H
#define UIBOTTOMWIDGET_H

#include <QWidget>

class UIBottomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UIBottomWidget(QWidget *parent = nullptr);

private:
    QWidget *m_leftWidget;
    QWidget *m_centerWidget;
    QWidget *m_rightWidget;
};

#endif // UIBOTTOMWIDGET_H
