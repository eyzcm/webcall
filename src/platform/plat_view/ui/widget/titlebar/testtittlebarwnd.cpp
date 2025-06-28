#include "testtittlebarwnd.h"

TestTittleBarWnd::TestTittleBarWnd(QWidget *parent) : QMainWindow(parent)
{
          setWindowFlags(Qt::FramelessWindowHint);

          UICustomTitleBarWidget *titleBar = new UICustomTitleBarWidget(this);
          connect(titleBar, &UICustomTitleBarWidget::SignalMinimize, this, &TestTittleBarWnd::showMinimized);
          connect(titleBar, &UICustomTitleBarWidget::SignalMaximizeRestore, this, &TestTittleBarWnd::OnMaximizeRestore);
          connect(titleBar, &UICustomTitleBarWidget::SignalClose, this, &TestTittleBarWnd::close);

          QWidget *centralWidget = new QWidget(this);
          QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
          mainLayout->setContentsMargins(0, 0, 0, 0);
          mainLayout->setSpacing(0);

          mainLayout->addWidget(titleBar);
          QWidget *yourCentralWidget = new QWidget(this);
          yourCentralWidget->setStyleSheet("background-color: #ff0000; ");
          // Add your central content widget here
          mainLayout->addWidget(yourCentralWidget);

          setCentralWidget(centralWidget);
}
