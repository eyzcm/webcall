#ifndef TESTTITTLEBARWND_H
#define TESTTITTLEBARWND_H

#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include "UICustomTitleBarWidget.h"

class TestTittleBarWnd : public QMainWindow
{
    Q_OBJECT

public:
    TestTittleBarWnd(QWidget *parent = nullptr);

private slots:
    void OnMaximizeRestore()
    {
        if (isMaximized()) {
            showNormal();
        } else {
            showMaximized();
        }
    }


};

#endif // TESTTITTLEBARWND_H
