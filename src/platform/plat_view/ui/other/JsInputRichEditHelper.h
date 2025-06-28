#ifndef JsInputRichEditHelper_H
#define JsInputRichEditHelper_H

//#include "ui/common/BaseJsCommHelper.h"
#include "utils/Plat_EnumDefine.h"
#include "ui/brower/uiwebview.h"

class JsInputRichEditHelper : public QObject
{
    Q_OBJECT

public:
    JsInputRichEditHelper();
    ~JsInputRichEditHelper();

    virtual void LoadFinished(bool isl);

    void GetInputValue(int i); // 获取富文本内容
    void InsertFilename(QString filename);
    void AddReferredMsg(QString jsontext);
    void SetSearchEngineName(QString engineName);
    void SetFont(QString font);
    void SetFontSize(QString font);
    void SetInputValue(QString text);
    void JsInsertPlainText(QString srt); //插入文本
    void appendPlainText(QString text);
    void SetInitData(int maxlen, int hideTools, QString font = "");
    void SetWindowDefaultHeight(int height);
    void HiddenTools(int hideTools);
    void setFocus();
    void SetUrl(const QString &url);
    void SetShortCutKeyFiter(QString keyvals);
    void SetBackColor(QString color, int opacity);
    void Js_AddSearchCommonWordList(QString json); //通知Js 添加匹配成功的常用语列表
    IWebView * GetCurrentView();

signals:
    void S_SetInputType(int inputType);    // 1.引用方式，发送消息带上引用消息 ， 0 ,普通方式
    void S_OnRichTextChanged(QString txt); // 录入框中文本改变 txt 纯文本
    void S_OnShortCutKey(QString keyval);
    void S_LoadFinished(bool isl);
    void S_RightBtnClicked(QString selecetText, QString pos);
    void S_GetInputValue(QString str, int i);
    void S_GetTextLength(int le);

public slots:
    void R_ueUploadFileEnd(QString fileType, EUSERTYPE type, QString fileName, QString msgTo, EMESSAGETYPE msgType);
    void R_UpdateFile(QString filename);
    void R_InsertPastImage();
    void R_InsertFile();
    void R_OnRichTextChanged(QString txt);
    void R_OnShortCutKey(QString keyval);
    void R_RightBtnClicked(QString selecetText, QString pos);
    void R_SetRefferMsgType(int type);

private:
    QString m_logoutUserId;
    int     m_defaultHeight;
    QString m_prefont;

    IWebView * m_webview;
};

#endif // JsInputRichEditHelper_H
