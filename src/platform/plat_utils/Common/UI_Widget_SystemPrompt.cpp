#include "UI_Widget_SystemPrompt.h"
#include "UI_UI_Widget_SystemPrompt.h"

#include <utils/UtilsHelper.h>

UI_Widget_SystemPrompt::UI_Widget_SystemPrompt(QWidget *parent) :
    QWidget(parent)
    , ui(new Ui::UI_Widget_SystemPrompt)
    , m_nShowTime(2000)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_DeleteOnClose);
    SetFont(9, "#ffffff", tr("微软雅黑"));
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(R_Timeout()));
}

UI_Widget_SystemPrompt::~UI_Widget_SystemPrompt()
{
}

void UI_Widget_SystemPrompt::SetShowTime(int sec)
{
    m_nShowTime = sec * 1000;
}

//设置显示位置（在Toast弹窗的父窗口中固定位置显示、在Toast弹窗的父窗口中目标位置附近显示）
void UI_Widget_SystemPrompt::SetShowPos(QWidget *parent, EYLUI_ToastShowPos showPos, QPoint tarPos)
{
    if (parent)
    {
        //在父窗口中显示
        if (tarPos == QPoint())
        {
            switch (showPos)
            {
                case Enum_YL_Toast_Parent_LeftTop:
                {
                    this->move(ParentShowToast_Padding, ParentShowToast_Padding);
                    break;
                }
                case Enum_YL_Toast_Parent_LeftCenter:
                {
                    this->move(ParentShowToast_Padding, parent->height() / 2 - this->height() / 2);
                    break;
                }
                case Enum_YL_Toast_Parent_LeftBottom:
                {
                    this->move(ParentShowToast_Padding, parent->height() - this->height() - ParentShowToast_Padding);
                    break;
                }
                case Enum_YL_Toast_Parent_CenterTop:
                {
                    this->move(parent->width() / 2 - this->width() / 2, ParentShowToast_Padding);
                    break;
                }
                case Enum_YL_Toast_Parent_CenterCenter:
                {
                    this->move(parent->width() / 2 - this->width() / 2, parent->height() / 2 - this->height() / 2);
                    break;
                }
                case Enum_YL_Toast_Parent_CenterBottom:
                {
                    this->move(parent->width() / 2 - this->width() / 2, parent->height() - this->height() - ParentShowToast_Padding);
                    break;
                }
                case Enum_YL_Toast_Parent_RightTop:
                {
                    this->move(parent->width() - this->width() - ParentShowToast_Padding, ParentShowToast_Padding);
                    break;
                }
                case Enum_YL_Toast_Parent_RightCenter:
                {
                    this->move(parent->width() - this->width() - ParentShowToast_Padding, parent->height() / 2 - this->height() / 2);
                    break;
                }
                case Enum_YL_Toast_Parent_RightBottom:
                {
                    this->move(parent->width() - this->width() - ParentShowToast_Padding, parent->height() - this->height() - ParentShowToast_Padding);
                    break;
                }
                default:
                {
                    this->move(parent->width() / 2 - this->width() / 2, parent->height() / 2 - this->height() / 2);
                    break;
                }
            }
        }

        //在兄弟窗口附近显示
        else
        {
            switch (showPos)
            {
                case Enum_YL_Toast_Target_LeftTop_Left:
                {
                    //目标坐标 左上 顶点的左侧
                    this->move(tarPos.x() - this->width() - ParentShowToast_Spacing, this->y());
                    break;
                }
                case Enum_YL_Toast_Target_LeftBottom_Left:
                {
                    //目标坐标 左下 顶点的左侧
                    this->move(tarPos.x() - this->width() - ParentShowToast_Spacing, this->y() - this->width());
                    break;
                }
                case Enum_YL_Toast_Target_LeftTop_Up:
                {
                    //目标坐标 左上 顶点的上方
                    this->move(tarPos.x(), tarPos.y() - this->height() - ParentShowToast_Spacing);
                    break;
                }
                case Enum_YL_Toast_Target_RightTop_Up:
                {
                    //目标坐标 右上 顶点的上方
                    this->move(tarPos.x() - this->width(), tarPos.y() - this->height() - ParentShowToast_Spacing);
                    break;
                }
                case Enum_YL_Toast_Target_RightTop_Right:
                {
                    //目标坐标 右上 顶点的右侧
                    this->move(tarPos.x() + ParentShowToast_Spacing, tarPos.y());
                    break;
                }
                case Enum_YL_Toast_Target_RightBottom_Right:
                {
                    //目标坐标 右下 顶点的右侧
                    this->move(tarPos.x() + ParentShowToast_Spacing, tarPos.y() - this->width());
                    break;
                }
                case Enum_YL_Toast_Target_RightBottom_Bottom:
                {
                    //目标坐标 右下 顶点的下方
                    this->move(tarPos.x() - this->width(), tarPos.y() + ParentShowToast_Spacing);
                    break;
                }
                case Enum_YL_Toast_Target_LeftBottom_Bottom:
                {
                    //目标坐标 左下 顶点的下方
                    this->move(tarPos.x(), tarPos.y() + ParentShowToast_Spacing);
                    break;
                }

                    // Toast 窗口中点与目标坐标一致(与目标窗口水平、垂直居中)
                case Enum_YL_Toast_Target_Top_HCenter:
                {
                    this->move(tarPos.x() - this->width() / 2, tarPos.y() - ParentShowToast_Spacing - this->height());
                    break;
                }
                case Enum_YL_Toast_Target_Bottom_HCenter:
                {
                    this->move(tarPos.x() - this->width() / 2, tarPos.y() + ParentShowToast_Spacing);
                    break;
                }
                case Enum_YL_Toast_Target_Left_VCenter:
                {
                    this->move(tarPos.x() - ParentShowToast_Spacing - this->width(), tarPos.y() - this->height() / 2);
                    break;
                }
                case Enum_YL_Toast_Target_Right_VCenter:
                {
                    this->move(tarPos.x() + ParentShowToast_Spacing, tarPos.y() - this->height() / 2);
                    break;
                }
                default:
                {
                    this->move(parent->width() / 2 - this->width() / 2, parent->height() / 2 - this->height() / 2);
                    break;
                }
            }
        }
    }
}

void UI_Widget_SystemPrompt::SetMsg(QString msg, EYLUI_ToastType type)
{
    //设置消息类型图标
    QString style = "QLabel#label_icon{border-image: url(";
    switch (type)
    {
        case Enum_YL_Toast_Type_Error:
        {
            style += ":/YL/Resources/YL/Toast/icon_toast_error.png";
            break;
        }
        case Enum_YL_Toast_Type_Urgent:
        {
            style += ":/YL/Resources/YL/Toast/icon_toast_urgent.png";
            break;
        }
        case Enum_YL_Toast_Type_Warning:
        {
            style += ":/YL/Resources/YL/Toast/icon_toast_warning.png";
            break;
        }
        case Enum_YL_Toast_Type_Common:
        default:
        {
            style += ":/YL/Resources/YL/Toast/icon_toast_success.png";
            break;
        }
    }
    style += ");}";
    ui->label_icon->setStyleSheet(style);

    //设置消息
    int nStrLen = msg.size();
    if (nStrLen > 12)
    {
        int nOffset = nStrLen - 12;

        QFontMetrics fontWidth(m_font);
        int          nCharWidth = fontWidth.boundingRect(msg).width() / nStrLen;

        nOffset = nOffset * nCharWidth;
        this->setFixedWidth(220 + nOffset);
    }

    ui->label_msg->setText(msg);
    ui->label_msg->setToolTip(msg);
}

void UI_Widget_SystemPrompt::SetStyle(int borderWidth, QColor borderColor, int borderRadius, QColor bgColor)
{
    QString strBorderColor = QString::number(borderColor.red()) + ", " + QString::number(borderColor.green()) + ", " + QString::number(borderColor.blue());
    QString strBgColor     = QString::number(bgColor.red()) + ", " + QString::number(bgColor.green()) + ", " + QString::number(bgColor.blue());

    QString strStyle = "QWidget#widget_bg{";
    strStyle += QString("border:%1px solid rgb(%2);").arg(QString::number(borderWidth)).arg(strBorderColor);
    strStyle += QString("border-radius:%1px;").arg(QString::number(borderRadius));
    strStyle += QString("background-color: rgb(%1);").arg(strBgColor);
    strStyle += "}";

    ui->widget_bg->setStyleSheet(strStyle);
}

void UI_Widget_SystemPrompt::SetStyle(QString bgImage)
{
    QString strStyle = QString("QWidget#widget_bg{border-image: url(%1);}").arg(bgImage);
    ui->widget_bg->setStyleSheet(strStyle);
}

void UI_Widget_SystemPrompt::SetFont(int fontSize, QString fontColor, QString fontFamily)
{
    m_font.setPointSize(UtilsHelper::ptconvertpx(fontSize));
    m_font.setFamily(fontFamily);

    QString strStyle = "QLabel{";
    strStyle += QString("font: %1pt \"%2\";").arg(UtilsHelper::ptconvertpx(fontSize)).arg(fontFamily);
    strStyle += QString("color:%1;").arg(fontColor);
    strStyle += "}";

    ui->label_msg->setStyleSheet(strStyle);
}

void UI_Widget_SystemPrompt::SetIcon(QString path)
{
    if (!path.isEmpty())
    {
        QString strStyle = QString("border-image: url(%1);").arg(path);

        ui->label_icon->setFixedSize(20, 20);
        ui->label_icon->setStyleSheet(strStyle);
    }
}

void UI_Widget_SystemPrompt::show(bool hi)
{
    m_tohide = hi;
    if (m_nShowTime > 0)
    {
        m_pTimer->stop();
        m_pTimer->start(m_nShowTime);
    }
    QWidget::show();
}

void UI_Widget_SystemPrompt::hide()
{
    setFixedWidth(220);
    m_pTimer->stop();
    QWidget::hide();
}

void UI_Widget_SystemPrompt::setVisible(bool visible)
{
    m_pTimer->stop();
    if (visible && m_nShowTime > 0)
    {
        m_pTimer->start(m_nShowTime);
    }
    QWidget::setVisible(visible);
}

void UI_Widget_SystemPrompt::R_Timeout()
{
    if (m_tohide)
    {
        hide();
    }
    else
    {
        m_pTimer->stop();
        QWidget::close();
    }
}
