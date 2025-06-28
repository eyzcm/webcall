#ifndef UIYL_INPUTTEXTPANELINTERFACE_H
#define UIYL_INPUTTEXTPANELINTERFACE_H

#include <QWidget>
#include "plat_ui_global.h"

#ifndef Rich_Length
#define Rich_Length 2000
#endif

class PLAT_UI_EXPORT UIYL_InputTextPanelInterface : public QWidget
{
    Q_OBJECT
public:
    explicit UIYL_InputTextPanelInterface(QWidget *parent = nullptr);

        virtual void GetInputValue(int i) = NULL;                 //得到输入框内容
        virtual void SetInputValue(QString text)= NULL;          //设置输入框内容， 清空参数text为""
        virtual void AddReferredMsg(QString jsontext)= NULL; //设置引用消息
        virtual void SetSearchEngineName(QString searchName)= NULL;
        virtual void setFocus()= NULL;                           //设置焦点
        virtual void HiddenTools(int hideTools)= NULL;
        virtual void Js_AddSearchCommonWordList(QString ison)= NULL;
        virtual void InsertHtmlValue(QString htmlval)= NULL;

        virtual void SetShortCutKeyFiter(QString keyvals)= NULL; //设置快捷键过滤, 多个用,隔开

        virtual void OnShortCutKey(QString key)= NULL;
        virtual void RemoveFormat() = NULL;
        virtual void appendPlainText(QString text) = NULL;
        virtual void SetFontSize(QString font) = NULL;            //设置编辑框中字符临时变化
        virtual void SetBackColor(QString color, int opacity) = NULL;
        virtual void Init(int maxlen, int hideTools, int font = 0)= NULL;






signals:
    void S_OnRichTextChanged(QString txt); //文本改动后发送信号
    void S_SetInputType(int inputType);
    void S_OnShortCutKey(QString keyval); //快捷键信号
    void S_LoadFinished(bool isl);        //页面加载完信号
    void S_RightBtnClicked(QString selecetText, QString pos);
    void S_GetInputValue(QString str, int i);
    void S_GetTextLength(int le);

};

#endif // UIYL_INPUTTEXTPANELINTERFACE_H
