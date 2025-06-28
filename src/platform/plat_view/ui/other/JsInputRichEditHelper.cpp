#include "stdafx.h"
#include "JsInputRichEditHelper.h"
#include "QClipboard"
#include "QFileDialog"
#include "include/json/value.h"
#include "utils/jsonhelper.h"
#include "ui/uimessagebox.h"
#include "utils/utilshelper.h"
#include "service/util_service.h"
#include "serviceInterface/dbuiplatservice.h"
#include "service/PlatCommonSignalService.h"



JsInputRichEditHelper::JsInputRichEditHelper()
{
    m_logoutUserId  = "";
    m_defaultHeight = 0;
    gUISystemConfigManager->FreeObj(nullptr);
    connect(PlatCommonSignalService::GetInstance(), SIGNAL(S_ueUploadFileEnd(QString, EUSERTYPE, QString, QString, EMESSAGETYPE)), this, SLOT(R_ueUploadFileEnd(QString, EUSERTYPE, QString, QString, EMESSAGETYPE)));
}

JsInputRichEditHelper::~JsInputRichEditHelper()
{
}

IWebView * JsInputRichEditHelper::GetCurrentView()
{
    return m_webview;
}

void JsInputRichEditHelper::SetUrl(const QString &url)
{
    if (GetCurrentView())
    {
        GetCurrentView()->LoadUrl(url);
    }
}

void JsInputRichEditHelper::GetInputValue(int i)
{
    if (GetCurrentView())
    {
        GetCurrentView()->runJavaScript("GetInputValue()", [=](const QVariant &text) { Q_EMIT S_GetInputValue(text.toString(), i); });
    }
}

void JsInputRichEditHelper::setFocus()
{
    if (GetCurrentView())
    {
        GetCurrentView()->runJavaScript("setFocus()");
    }
}

void JsInputRichEditHelper::SetInputValue(QString text)
{
    if (GetCurrentView())
    {
        Json::Value val;
        val["text"]     = text.toStdString();
        QString jsonStr = QString::fromStdString(val.toStyledString());
        QString jsVal   = QString("JsSetInputValue(%1)").arg(jsonStr);
        GetCurrentView()->runJavaScript(jsVal);
    }
}

void JsInputRichEditHelper::InsertFilename(QString fileName)
{
    if (GetCurrentView())
    {
        static int init = 1000;
        init++;
        QString jscall = QString("jsclientuploadfile('%1','loading_id_%2')").arg(fileName).arg(init);
        GSCRIPTLOG(jscall);
        GetCurrentView()->runJavaScript(jscall);
        //todo 9999
        //ConversionController::GetInstance()->UploadFile(fileName, QString("%1").arg(init), E_MSGTYPE_FILE, E_UEDITOR, false);
    }
}

void JsInputRichEditHelper::R_ueUploadFileEnd(QString fileType, EUSERTYPE type, QString fileName, QString msgTo, EMESSAGETYPE msgType)
{
    if (GetCurrentView() )
    {
        QString jscall = QString("jsclientuploadfileEnd('%1','loading_id_%2')").arg(fileName).arg(msgTo);
        GSCRIPTLOG(jscall);
        GetCurrentView()->runJavaScript(jscall);
    }
}

void JsInputRichEditHelper::R_UpdateFile(QString filename)
{
    InsertFilename(filename);
}

void JsInputRichEditHelper::R_InsertPastImage()
{
    QClipboard *board = QApplication::clipboard();
    if (board)
    {
        QImage image = board->image();
        if (!image.isNull())
        {
            QString filepath = UtilsHelper::GetDownLoadFileDir();
            filepath += QString("%1.png").arg(QTime::currentTime().msec() % 10000);
            image.save(filepath, "png", 1);
            InsertFilename(filepath);
        }
    }
}

void JsInputRichEditHelper::JsInsertPlainText(QString srt)
{
    if (GetCurrentView())
    {
        QString     jscall;
        Json::Value val;
        val["text"]     = srt.toStdString();
        QString jsonStr = QString::fromStdString(val.toStyledString());
        jscall          = QString("JsInsertPlainText(%1)").arg(jsonStr);
        GSCRIPTLOG(jscall);
        GetCurrentView()->runJavaScript(jscall);
    }
}

void JsInputRichEditHelper::appendPlainText(QString text)
{
    if (GetCurrentView())
    {
        QString     jscall;
        Json::Value val;
        val["text"]     = text.toStdString();
        QString jsonStr = QString::fromStdString(val.toStyledString());
        jscall          = QString("JsAppendPlainText(%1)").arg(jsonStr);
        GSCRIPTLOG(jscall);
        GetCurrentView()->runJavaScript(jscall);
    }
}

void JsInputRichEditHelper::R_InsertFile()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(nullptr, tr("选择图片"), QDir::currentPath(), "Images (*.png *.jpg *.bmp *.jpeg *.mpeg *.gif)");
    if (fileName.isNull())
    {
        return;
    }
    InsertFilename(fileName);
}

void JsInputRichEditHelper::R_OnRichTextChanged(QString txt)
{
    Q_EMIT S_OnRichTextChanged(txt);
}

void JsInputRichEditHelper::R_OnShortCutKey(QString keyval)
{
    Q_EMIT S_OnShortCutKey(keyval);
}

void JsInputRichEditHelper::SetShortCutKeyFiter(QString keyvals)
{
    if (GetCurrentView())
    {
        QString jscall;
        jscall = QString("JsSetShortCutKeyFiter('%1')").arg(keyvals);
        GSCRIPTLOG(jscall);
        GetCurrentView()->runJavaScript(jscall);
    }
}

void JsInputRichEditHelper::SetSearchEngineName(QString searchName)
{
    if (GetCurrentView())
    {
        QString jscall;
        jscall = QString("JsSetSearchEngineName('%1')").arg(searchName);
        GSCRIPTLOG(jscall);
        GetCurrentView()->runJavaScript(jscall);
    }
}

void JsInputRichEditHelper::SetInitData(int maxlen, int hideTools, QString fonttext)
{
    if (GetCurrentView())
    {
        QString language = "0";
        QLocale locale;
        if (locale.language() == QLocale::Chinese && gUISystemConfigManager->GetSystemConfigByKey("language", "0") == "0")
        {
            language = "0";
        }
        else
        {
            language = "1";
        }

        Json::Value val;
        val["language"]  = language.toStdString();
        val["maxlen"]    = maxlen;
        val["showTools"] = hideTools;
        if (!fonttext.isEmpty())
            val["fontsize"] = fonttext.toStdString();

        if (m_defaultHeight > 0)
        {
            val["defaultHeight"] = m_defaultHeight;
        }

        QString jsonStr = QString::fromStdString(val.toStyledString());
        QString jscall  = QString("InitData(%1)").arg(jsonStr);
        GSCRIPTLOG(jscall);
        GetCurrentView()->runJavaScript(jscall);
    }
}

void JsInputRichEditHelper::SetWindowDefaultHeight(int height)
{
    m_defaultHeight = height;
}

void JsInputRichEditHelper::HiddenTools(int hideTools)
{
    if (GetCurrentView())
    {
        QString jscall = QString("JsHiddenTools(%1)").arg(hideTools);
        GSCRIPTLOG(jscall);
        GetCurrentView()->runJavaScript(jscall);
    }
}

void JsInputRichEditHelper::LoadFinished(bool isl)
{
    Q_EMIT S_LoadFinished(isl);
}

void JsInputRichEditHelper::R_RightBtnClicked(QString selecetText, QString pos)
{
    Q_EMIT S_RightBtnClicked(selecetText, pos);
}

void JsInputRichEditHelper::SetFont(QString font)
{
    m_prefont;
}

void JsInputRichEditHelper::SetFontSize(QString font)
{
    if (GetCurrentView())
    {
        QString jscall = QString("JsSetFontSize(%1)").arg(font);
        GSCRIPTLOG(jscall);
        GetCurrentView()->runJavaScript(jscall);
    }
}

void JsInputRichEditHelper::SetBackColor(QString color, int opacity)
{
    if (GetCurrentView())
    {
        QString jscall = QString("JsSetBackColor(\"%1\", %2)").arg(color).arg(opacity);
        GSCRIPTLOG(jscall);
        GetCurrentView()->runJavaScript(jscall);
    }
}

void JsInputRichEditHelper::Js_AddSearchCommonWordList(QString json)
{
    if (GetCurrentView())
    {
        QString jscall = QString("Js_AddSearchCommonWordList(%1)").arg(json);
        GSCRIPTLOG(jscall);
        qDebug() << "JsInputRichEditHelper call " << jscall;
        GetCurrentView()->runJavaScript(jscall);
    }
}

void JsInputRichEditHelper::AddReferredMsg(QString jsontext)
{
    if (GetCurrentView())
    {
        QString jscall = QString("JsAddReferredMsg(%1)").arg(jsontext);
        GSCRIPTLOG(jscall);
        GetCurrentView()->runJavaScript(jscall);
    }
}

void JsInputRichEditHelper::R_SetRefferMsgType(int type)
{
    Q_EMIT S_SetInputType(type);
}
