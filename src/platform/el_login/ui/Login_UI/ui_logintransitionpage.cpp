#include "ui_logintransitionpage.h"
#include "UI_LoginTransitionsPage.h"
#include "control/LoginController.h"
#include <QIcon>

UI_LoginTransitionPage::UI_LoginTransitionPage(QWidget *parent) : QSplashScreen(parent)
{
    ui.setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowIcon(QIcon(":/Resources/Image/easy.ico"));
    this->setWindowTitle(tr("资源加载"));

    //切换静态图片
    m_nCurIndex            = 0;
    m_pTimer_ChangePicture = new QTimer(this);
    m_pTimer_ChangePicture->start(1000);
    connect(m_pTimer_ChangePicture, SIGNAL(timeout()), this, SLOT(R_Timeout_ChangePicture()));

    //进度条设置
    ui.m_progressBar->setRange(0, 100);
    ui.m_progressBar->setValue(0);
    connect(ui.m_progressBar, SIGNAL(valueChanged(int)), this, SLOT(R_ValueChanged(int)));

    //进度条动画
    m_pAnimation = new QPropertyAnimation(this);
}

UI_LoginTransitionPage::~UI_LoginTransitionPage()
{
}

void UI_LoginTransitionPage::R_Timeout_ChangePicture()
{
    if (m_nCurIndex == 0)
    {
        ui.m_label_name->setText(tr("关于易聊"));
        ui.m_label_content->setText(tr("北京易聊科技有限公司是一家为客户提供智能客服综合服务解决方案的专业服务机构，以AI人工智能及智能在线客服系统相关产业的研发、运营及销售为核心业务。"));
        ui.widget_picture->setStyleSheet("QWidget#widget_picture{border-image: url(:/login/Resources/Image/TransitionPage/icon_firstPicture.png);}");
    }
    else if (m_nCurIndex == 1)
    {
        ui.m_label_name->setText("Each Bot");
        ui.m_label_content->setText(tr("“Each Bot”AI人工智能机器人可代替人工在线客服，完成售前、售后的接待工作。产品广泛服务于教育、留学、医疗、电商、通信、金融、装饰等各行业的智能客服系统。"));
        ui.widget_picture->setStyleSheet("QWidget#widget_picture{border-image: url(:/login/Resources/Image/TransitionPage/icon_secondPicture.png);}");
    }
    else
    {
        ui.m_label_name->setText("Easy Liao");
        ui.m_label_content->setText(tr("“Easy Liao”人工在线客服系统，11年资深工程团队根据千余家客户需求为导向开发的多版本、多功能，可以满足各行业客服人员使用的在线客服软件。"));
        ui.widget_picture->setStyleSheet("QWidget#widget_picture{border-image: url(:/login/Resources/Image/TransitionPage/icon_threePicture.png);}");
    }

    m_nCurIndex++;
    if (m_nCurIndex > 2)
    {
        m_nCurIndex = 0;
    }
    QCoreApplication::processEvents();
}

void UI_LoginTransitionPage::R_ValueChanged(int value)
{
    ui.m_label_value->setText(QString::number(value));
}

void UI_LoginTransitionPage::UpdateProgressBarValue(int nValue, QString msg)
{
	
    if ((nValue > 0) && (nValue < 100))
    {
        m_pAnimation->stop();
        int nCurValue = ui.m_progressBar->value();

        m_pAnimation->setTargetObject(ui.m_progressBar);
        m_pAnimation->setPropertyName("value");
        m_pAnimation->setDuration(300);
        m_pAnimation->setStartValue(nCurValue);
        m_pAnimation->setEndValue(nValue);
        m_pAnimation->start();
        //        showMessage(msg, Qt::AlignHCenter | Qt::AlignBottom, Qt::white); //设置消息
        QCoreApplication::processEvents();
    }
    else if (nValue == 100)
    {
        ui.m_progressBar->setValue(100);
        m_pAnimation->stop();
        m_pTimer_ChangePicture->stop();
        
        finish((QWidget *)LoginController::GetInstance()->getBaseMainWnd());
    }
}
