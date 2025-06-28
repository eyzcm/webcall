#include "ui/uimessagebox.h"
#include <ui_uimessagebox.h>
#include <qgraphicseffect.h>

UIMessageBox::UIMessageBox(QWidget* parent, int mode) : MyWidgetBase(parent)
{
    ui = new Ui::UIMessageBox;
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    reInt = 0;
    m_eventLoop = nullptr;

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::gray);
    shadow->setBlurRadius(10);
    ui->widget_bg->setGraphicsEffect(shadow);
    ui->m_tipContent->setEnabled(false);
    ui->cancelpushButton->setVisible(false);
    ui->stackedWidget_bg->setCurrentIndex(mode);
    QSize size = QSize(440, 220);
    if (mode == 0)
    {
        size = QSize(440, 220);
        connect(ui->okpushButton, SIGNAL(clicked()), this, SLOT(R_OKClicked()));
        connect(ui->cancelpushButton, SIGNAL(clicked()), this, SLOT(R_CancelClicked()));
        connect(ui->closetoolButton, SIGNAL(clicked()), this, SLOT(R_CancelClicked()));
    }
    else if (mode == 1)
    {
        size = QSize(480, 300);
        ui->gridLayout_4->setContentsMargins(15, 40, 9, 45);
        connect(ui->okpushButton_2, SIGNAL(clicked()), this, SLOT(R_OKClicked()));         // 微信扫码 1
        connect(ui->cancelpushButton_2, SIGNAL(clicked()), this, SLOT(R_CancelClicked())); // 验证码解冻 0
        connect(ui->closetoolButton_2, SIGNAL(clicked()), this, SLOT(R_Closed()));         // 关闭按钮 2
    }
    setMaximumSize(size);
    setMinimumSize(size);
    resize(size);
}

UIMessageBox::~UIMessageBox()
{
    QGraphicsDropShadowEffect* shadow = (QGraphicsDropShadowEffect*)graphicsEffect();
    if (shadow)
    {
        shadow->deleteLater();
        shadow = nullptr;
    }
    if (m_eventLoop)
    {
        m_eventLoop->deleteLater();
        m_eventLoop = nullptr;
    }
    /*
    if( ui )
        delete ui;
        */

}

int UIMessageBox::exec()
{
    ShowCenter_Model();
    m_eventLoop = new QEventLoop(this);
    m_eventLoop->exec();
    return reInt;
}

void UIMessageBox::done(int i)
{
    reInt = i;
}

void UIMessageBox::SetContentTip(QString tip)
{
    ui->m_tipContent->setText(tip);
}

void UIMessageBox::SetCancelBtnVisible(bool isShow)
{
    ui->cancelpushButton->setVisible(isShow);
}

void UIMessageBox::R_OKClicked()
{
    done(1);
    Q_EMIT S_ResultOK(true);
    close();
}

void UIMessageBox::R_CancelClicked()
{
    done(0);
    Q_EMIT S_ResultOK(false);
    close();
}

void UIMessageBox::R_Closed()
{
    done(2);
    Q_EMIT S_ResultOK(false);
    close();
}

void UIMessageBox::SetOkBtnText(QString text)
{
    ui->okpushButton->setText(text);
    if (text.count() > 3)
    {
        ui->okpushButton->setMinimumWidth(80);
        ui->okpushButton->setMaximumWidth(80);
    }

}

void UIMessageBox::SetCancelBtnText(QString text)
{
    ui->cancelpushButton->setText(text);
}

void UIMessageBox::closeEvent(QCloseEvent *event)
{
    // 关闭窗口时结束事件循环，在exec()方法中返回选择结果;
    if (m_eventLoop)
    {
        m_eventLoop->exit();
    }
    event->accept();
}

