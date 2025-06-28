#ifndef UICONTENTBOXWIDGET_H
#define UICONTENTBOXWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>

class UIContentBoxWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UIContentBoxWidget(QWidget *parent = nullptr);

    // 添加新的widget到QStackedWidget中
    void AddWidget(QWidget *widget);

    // 切换到指定索引的widget
    void SetCurrentIndex(int index);

private:
    QStackedWidget *m_stackedWidget;
    QVBoxLayout *m_mainLayout;
};

#endif // UICONTENTBOXWIDGET_H
