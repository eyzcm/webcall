#include "UI_Widget_User_Panel.h"
#include "utils/EL_Util_Time.h"

//#include "control/CommonSignalService.h"
#include "session/el_sessionservice.h"
#include <QClipboard>

UI_Widget_User_Panel::UI_Widget_User_Panel(QWidget *parent) : widget_user_interface(parent), m_strUserName(""), m_strAccountNumber("")
{
    //过渡页显示进度


    gLoginTransitionsPage->SendSchedule("20", tr("初始化用户信息区..."));


    ui.setupUi(this);


    QString headUri = ":/Resources/SVG/user_head_portrait.svg";
    SetHeadPortrait(headUri);

    connect(ui.label_account_number, SIGNAL(clicked()), this, SLOT(On_Label_account_number_Clicked()));
}

UI_Widget_User_Panel::~UI_Widget_User_Panel()
{
}

void UI_Widget_User_Panel::SetUserName(QString strName)
{
    m_strUserName = strName;

    QFontMetrics fontWidth(ui.label_user_name->font());
    QString      elideNote = fontWidth.elidedText(strName, Qt::ElideRight, 200);
    ui.label_user_name->setText(elideNote);
    ui.label_user_name->setToolTip(strName);
}

QString UI_Widget_User_Panel::GetUserName()
{
    return m_strUserName;
}

void UI_Widget_User_Panel::SetAccountNumber(QString strNum)
{
    m_strAccountNumber = strNum;

    QFontMetrics fontWidth(ui.label_account_number->font());
    QString      elideNote = fontWidth.elidedText(strNum, Qt::ElideRight, 200);
    ui.label_account_number->setText(elideNote);
    ui.label_account_number->setToolTip(strNum);
}

QString UI_Widget_User_Panel::GetAccountNUmber()
{
    return m_strAccountNumber;
}

void UI_Widget_User_Panel::SetHeadPortrait(QString strImagePath)
{
    QString str = QString("QWidget#widget_user_head_portrait_rect{image: url(%1);border: 2px solid white;border-radius: 27px;};").arg(strImagePath);
    ui.widget_user_head_portrait_rect->setStyleSheet(str);
}

void UI_Widget_User_Panel::On_Label_account_number_Clicked()
{
//todo 9999
    //CommonSignalService::GetInstance()->DoShowToastMsg(tr("复制成功"), 2, Enum_YL_Toast_Parent_CenterCenter, Enum_YL_Toast_Type_Common, QPoint());
    QClipboard *board = QApplication::clipboard();
    board->setText(ui.label_account_number->text());
}
