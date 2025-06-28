#include "drawtools.h"
#include "utils/utilshelper.h"

DrawTools::DrawTools()
{

}


// 绘制按钮
void DrawTools::paintButton(QPainter *painter, const QStyleOptionViewItem &option, YL_MainTreeLeafNodeItemData *leafData) const
{
    QStringList buttonList = GetButtonListByData(leafData);
    // 计算按钮显示区域
    int nCount = buttonList.count();
    int nTop   = option.rect.top() + option.rect.height() - 5 - m_nHeight;

    for (int i = 0; i < nCount; ++i)
    {
        if (buttonList.at(i).indexOf("transferChat", 0, Qt::CaseInsensitive) != -1)
        {
            bool hasPermission_colleagueList = gSessionService->HasPermissionByKey("colleague");
            if ((leafData->SearchingType() == "WeChatPublic") || !hasPermission_colleagueList)
            {
                continue;
            }
        }

        // 绘制按钮
        QStyleOptionButton button;
        button.rect = QRect(option.rect.left() + option.rect.width() - m_nWidth * (i + 1) - m_nSpacing * i - 10, nTop, m_nWidth, m_nHeight);
        button.state |= QStyle::State_Enabled;
        button.iconSize = QSize(14, 14);

        if (button.rect.contains(m_mousePoint))
        {
            if (m_nType == 0)
            {
                button.state |= QStyle::State_MouseOver;
            }
            else if (m_nType == 1)
            {
                button.state |= QStyle::State_Sunken;
            }
        }

        QString key = buttonList.at(i);
        if (key.indexOf("transferChat", 0, Qt::CaseInsensitive) != -1)
        {
            button.icon = QIcon(QString(":/YL/Resources/YL/MainTree/chatbutton/transferChat.png"));
        }
        else if (key.indexOf("joinRobotChat", 0, Qt::CaseInsensitive) != -1)
        {
            button.icon = QIcon(QString(":/YL/Resources/YL/MainTree/chatbutton/joinRobotChat.png"));
        }
        else if (key.indexOf("returnRobotChat", 0, Qt::CaseInsensitive) != -1)
        {
            button.icon = QIcon(QString(":/YL/Resources/YL/MainTree/chatbutton/joinRobotChat.png"));
        }
        else if (key.indexOf("grabRobotChat", 0, Qt::CaseInsensitive) != -1)
        {
            button.icon = QIcon(QString(":/YL/Resources/YL/MainTree/chatbutton/grabRobotChat.png"));
        }
        else if (key.indexOf("summaryTagChat", 0, Qt::CaseInsensitive) != -1)
        {
            button.icon = QIcon(QString(":/YL/Resources/YL/MainTree/chatbutton/summaryTagChat.png"));
        }
        else if (key.indexOf("acceptChat", 0, Qt::CaseInsensitive) != -1)
        {
            button.icon = QIcon(QString(":/YL/Resources/YL/MainTree/chatbutton/acceptChat.png"));
        }
        else if (key.indexOf("assignChat", 0, Qt::CaseInsensitive) != -1)
        {
            button.icon = QIcon(QString(":/YL/Resources/YL/MainTree/chatbutton/assignChat.png"));
        }
        else if (key.indexOf("directChat", 0, Qt::CaseInsensitive) != -1)
        {
            button.icon = QIcon(QString(":/YL/Resources/YL/MainTree/chatbutton/directChat.png"));
        }
        else if (key.indexOf("inviteChat", 0, Qt::CaseInsensitive) != -1)
        {
            button.icon = QIcon(QString(":/YL/Resources/YL/MainTree/chatbutton/inviteChat.png"));
        }else if (key.indexOf("douyinChat", 0, Qt::CaseInsensitive) != -1)
        {
            button.icon = QIcon(QString(":/YL/Resources/YL/MainTree/chatbutton/directChat.png"));
        }
        else
        {
            Q_ASSERT(false);
        }

        QWidget *pWidget = m_pButton.data();
        pWidget->style()->drawControl(QStyle::CE_PushButton, &button, painter, pWidget);
    }
}


void DrawTools::DoDrawPixmap(QPainter *painter, const QStyleOptionViewItem &option, QRect rect, QPixmap &pixmap, QString pixname)
{
    QStyleOptionViewItem tempOption = option;
    tempOption.decorationAlignment  = Qt::AlignCenter | Qt::AlignVCenter;
    tempOption.decorationSize       = QSize(rect.width(), rect.height());
    pixmap                          = UtilsHelper::ChangeQPixmapDPI(pixmap, pixname, tempOption.decorationSize);
    QApplication::style()->drawItemPixmap(painter, rect, tempOption.decorationAlignment, pixmap);
}
