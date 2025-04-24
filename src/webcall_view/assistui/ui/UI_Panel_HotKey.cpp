#include "UI_Panel_HotKey.h"
#include "utils/UICommUtilsHelper.h"
#include "service/DbUiWebcallService.h"
#include "data/UIShowSettingInfo.h"

UI_Panel_HotKey::UI_Panel_HotKey(QWidget *parent) : QWidget(parent), m_HasHotKey(false)
{
    ui.setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    ui.m_hotkeyLineEdit->installEventFilter(this);

    connect(ui.m_delBtn, SIGNAL(clicked()), this, SLOT(R_OnDeleteBtnClick()));
    connect(ui.m_cancelBtn, SIGNAL(clicked()), this, SLOT(R_OnCancelBtnClick()));
}

UI_Panel_HotKey::~UI_Panel_HotKey()
{
}

void UI_Panel_HotKey::InitHotkey(QString key)
{
    ui.m_hotkeyLineEdit->setText(key);
    ui.m_hotkeyLineEdit->setFocus();
}

void UI_Panel_HotKey::R_OnCancelBtnClick()
{
    this->parentWidget()->close();
}

void UI_Panel_HotKey::R_OnDeleteBtnClick()
{
    ui.m_hotkeyLineEdit->setText("");
}

bool UI_Panel_HotKey::Is_HotkeyAlreadyExists(QString key)
{
    bool hasKey = false;

    UIShowSettingInfo *showInfo = gUIPersonalConfigYLService->ShowSettingInfo();
    if (showInfo)
    {
        key                         = key.remove(QRegExp("\\s")).toLower();
        QString strScreenShot       = showInfo->KeyScreenShot().remove(QRegExp("\\s")).toLower();
        QString strCommonWordSearch = showInfo->KeyCommonWordSearch().remove(QRegExp("\\s")).toLower();
        QString strOpenCard         = showInfo->KeyOpenCard().remove(QRegExp("\\s")).toLower();
        QString strActiveMainWnd    = showInfo->KeyActiveMainWnd().remove(QRegExp("\\s")).toLower();
        QString strChatChange       = showInfo->KeyChatChange().remove(QRegExp("\\s")).toLower();
        QString strSendImage        = "ctrl+shift+v";

        if ((key == strScreenShot) || (key == strCommonWordSearch) || (key == strOpenCard) || (key == strActiveMainWnd) || (key == strChatChange) || (key == strSendImage))
        {
            hasKey = true;
        }

        //清理指针
        delete showInfo;
        showInfo = nullptr;
    }

    return hasKey;
}

bool UI_Panel_HotKey::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent *event = static_cast<QKeyEvent *>(e);
        if (obj == ui.m_hotkeyLineEdit)
        {
            if ((event->modifiers() & Qt::ControlModifier) || (event->modifiers() & Qt::ShiftModifier) || (event->modifiers() & Qt::AltModifier))
            {
                if (((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_A)) || ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_S)) || ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_X))
                    || ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_C)) || ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_V)) || ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_Z))
                    || ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_B)))
                {
                    UICommUtilsHelper::ShowCommonBox(tr("提示"), tr("您设置的快捷键已被占用，请重新设置！"));
                    return true;
                }

                QString ret = "";
                if (event->modifiers() & Qt::ControlModifier)
                {
                    ret = "Ctrl";
                    if (event->modifiers() & Qt::AltModifier)
                    {
                        ret = ret + " + Alt";
                        if (event->modifiers() & Qt::ShiftModifier)
                        {
                            ret = ret + " + Shift";
                        }
                    }
                    else
                    {
                        if (event->modifiers() & Qt::ShiftModifier)
                        {
                            ret = ret + " + Shift";
                        }
                    }
                }
                else
                {
                    if (event->modifiers() & Qt::AltModifier)
                    {
                        ret = "Alt";
                        if (event->modifiers() & Qt::ShiftModifier)
                        {
                            ret = ret + " + Shift";
                        }
                    }
                    else
                    {
                        ret = "Shift";
                    }
                }

                int nVal = event->key();
                if ((nVal >= 0x30 && nVal <= 0x39) || (nVal >= 0x40 && nVal <= 0x5a) || (nVal >= 0x60 && nVal <= 0x69) || (nVal >= 0x01000030 && nVal <= 0x0100003b))
                {
                    ret = ret + " + " + char(nVal);

                    if (gUICommonWordManager->FindHotkey(ret))
                    {
                        m_HasHotKey = true;
                        UICommUtilsHelper::ShowCommonBox(tr("提示"), tr("热键已存在，请重新输入!"));
                    }
                    else if (Is_HotkeyAlreadyExists(ret))
                    {
                        m_HasHotKey = true;
                        UICommUtilsHelper::ShowCommonBox(tr("提示"), tr("热键已存在，请重新输入!"));
                    }
                    else
                    {
                        m_HasHotKey = false;
                        Q_EMIT S_AddHotKey(ret);
                        this->parentWidget()->close();
                    }
                }
            }
        }
    }

    if ((e->type() == QEvent::FocusIn) && m_HasHotKey)
    {
        ui.m_hotkeyLineEdit->setText("");
    }

    return QWidget::eventFilter(obj, e);
}
