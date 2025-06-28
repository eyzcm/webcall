#include "QUrl"
#include <QString>

#include "QClipboard"
#include "QDesktopServices"
#include "QFutureWatcher"
#include "QMessageBox"
#include "control/CommonSignalService.h"
#include "service/DbUiWebcallService.h"
#include "ui_sendimage.h"
#include "data/UIOpertorSettinginfo.h"
#include "utils/utilshelper.h"
#include <QDesktopWidget>
#include <qevent.h>
#include "ui/common/UtilsViewHelper.h"

UI_SendImage::UI_SendImage(QWidget *parent) : QWidget(parent), m_image(QImage()), m_imgfilename(""), m_chatItem(nullptr)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

    ui.m_image->installEventFilter(this);

    connect(ui.m_sendbtn, SIGNAL(clicked()), this, SLOT(R_OnSendBtnBtnClick()));
    connect(ui.m_canclebtn, SIGNAL(clicked()), this, SLOT(R_OnCancleBtnBtnClick()));
    connect(ui.m_image, SIGNAL(clicked()), this, SLOT(R_OnOpenImage()));
    connect(this, SIGNAL(S_OnSendFile(MChatItemData *, QString, EMESSAGETYPE)), CommonSignalService::GetInstance(), SIGNAL(S_OnSendFile(MChatItemData *, QString, EMESSAGETYPE)));
    connect(CommonSignalService::GetInstance(), SIGNAL(S_OpertorSettingChanged()), this, SLOT(R_OpertorSettingChanged()));
}

UI_SendImage::~UI_SendImage()
{
}

int UI_SendImage::LoadBoardData()
{
    m_imgfilename = "";

    UIOpertorSettingInfo *operInfo = gUIPersonalConfigYLService->OpertorSettingInfo();
    if (operInfo)
    {
        if (operInfo->SendImagePreview() == 1)
        {
            ui.m_immsendimagecheck->setChecked(false);
        }
        else
        {
            ui.m_immsendimagecheck->setChecked(true);
        }
		delete operInfo;
		operInfo = NULL;
    }

    QClipboard *board = QApplication::clipboard();
    if (board)
    {
        m_image = board->image();
        if (!m_image.isNull())
        {
            QPixmap         pixmap        = QPixmap::fromImage(m_image);
            QDesktopWidget *desktopWidget = QApplication::desktop();

            //图片宽高
            int pixW = pixmap.width();
            int pixH = pixmap.height();

            //窗口显示图片的区域宽高
            int showW = 500;
            int showH = 400;

            //屏幕宽高
            int screenW = desktopWidget->screenGeometry().width();
            int screenH = desktopWidget->screenGeometry().height();

            //窗口操作区面板高度
            int opeH = ui.widget_operator->height();

            if ((pixW < showW) && (pixH < showH))
            {
                //保持原始大小
                // ui.m_image->setFixedSize(pixW, pixH);
                ui.widget_image->setMinimumSize(showW, showH);
                ui.m_image->setPixmap(pixmap);
            }
            else if ((pixW < screenW - 20) && (pixH < screenH - opeH - 50))
            {
                //窗口适应图片大小
                this->setMinimumSize(pixW, pixH + opeH);
                ui.widget_image->setMinimumSize(pixW, pixH);
                ui.m_image->setPixmap(pixmap);
            }
            else
            {
                //图片进行自适应
                QPixmap fitpixmap = pixmap.scaled(showW, showH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                ui.widget_image->setMinimumSize(showW, showH);
                ui.m_image->setPixmap(fitpixmap);
            }

            return 1;
        }
    }

    return 0;
}

void UI_SendImage::SetImage(const QImage &image)
{
    m_image = image;
    ui.m_image->setPixmap(QPixmap::fromImage(m_image));
}

void UI_SendImage::SetChatItem(MChatItemData *chatItem)
{
    m_chatItem = chatItem;
}

void UI_SendImage::R_OnSendBtnBtnClick()
{
    //更新个人设置配置项
    UIOpertorSettingInfo *operInfo = gUIPersonalConfigYLService->OpertorSettingInfo();
    if (operInfo)
    {
        if (ui.m_immsendimagecheck->checkState() == Qt::Checked)
        {
            operInfo->SendImagePreview(0);
        }
        else
        {
            operInfo->SendImagePreview(1);
        }

        gUIPersonalConfigYLService->OpertorSettingInfo(operInfo);
        delete operInfo;
        operInfo = nullptr;
    }

    //图片发送
    if (!m_image.isNull())
    {
        if (m_chatItem)
        {
            if (m_imgfilename.isEmpty())
            {
                QString filepath = UtilsViewHelper::GetDownLoadFileDir();
                filepath += QString("%1.png").arg(QTime::currentTime().msec() % 10000);
                m_image.save(filepath, "png", 1);
                m_imgfilename = filepath;
            }
            Q_EMIT S_OnSendFile(m_chatItem, m_imgfilename, E_MSGTYPE_SCREENSHOTS);
            Q_EMIT S_Closed();
        }
    }
}

void UI_SendImage::R_OnCancleBtnBtnClick()
{
    Q_EMIT S_Closed();
}

void UI_SendImage::R_OnOpenImage()
{
    if (m_imgfilename.isEmpty())
    {
        QString filepath = UtilsViewHelper::GetDownLoadFileDir();
        filepath += QString("%1.png").arg(QTime::currentTime().msec() % 10000);
        m_image.save(filepath, "png", 1);
        m_imgfilename = filepath;
    }

    QDesktopServices::openUrl(QUrl::fromLocalFile(m_imgfilename));
}

void UI_SendImage::R_OpertorSettingChanged()
{
    UIOpertorSettingInfo *info = gUIPersonalConfigYLService->OpertorSettingInfo();
    if (info)
    {
        if (info->SendImagePreview() == 1)
        {
            ui.m_immsendimagecheck->setChecked(false);
        }
        else
        {
            ui.m_immsendimagecheck->setChecked(true);
        }
    }
}

bool UI_SendImage::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui.m_image)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = (QMouseEvent *)(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                R_OnOpenImage();
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        // pass the event on to the parent class
        return QWidget::eventFilter(obj, event);
    }
}
