#ifndef UISIDETABWIDGET_H
#define UISIDETABWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>

class UISideTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UISideTabWidget(QWidget *parent = nullptr);
    void AddTab(QWidget *widget, const QString &label);

private:
    QTabWidget *m_tabWidget;
    QVBoxLayout *m_mainLayout;
};

#endif // UISIDETABWIDGET_H
