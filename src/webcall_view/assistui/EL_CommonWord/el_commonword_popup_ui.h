#ifndef EL_COMMONWORD_POPUP_UI_H
#define EL_COMMONWORD_POPUP_UI_H

#include "utils/EnumDefine.h"
#include "data\YLData\YL_TreeViewItem_CommonWord.h"
#include "el_commonword_jshelper.h"
#include "data/UICommonWordInfo.h"
#include <MyWidgetBase.h>
#include <QWidget>

namespace Ui
{
    class EL_CommonWord_Popup_UI;
}
class UI_TreeViewItemModel_CommonWord;
class EL_CommonWord_Popup_UI : public MyWidgetBase
{
    Q_OBJECT

public:
    explicit EL_CommonWord_Popup_UI(QWidget *parent = nullptr);
    ~EL_CommonWord_Popup_UI();
    bool Is_HotkeyAlreadyExists(QString key);
    bool eventFilter(QObject *obj, QEvent *e);
    void setTitle(QString title);
    void Init(YL_TreeViewItem_CommonWord *item, EYLUI_CtrlName type, UI_TreeViewItemModel_CommonWord *gItemModel, bool haveGropu = false);
    void setGroupList(UI_TreeViewItemModel_CommonWord *model, QString selecID = "");
    void reSetSize(EYLUI_CtrlName t);
    void setGrouphide(bool ishide);

signals:
    void S_AddCommonWord(YL_TreeViewItem_CommonWord *parentItem, UICommonWordInfo *info);
    void S_ModifyCommonWord(YL_TreeViewItem_CommonWord *parentItem, UICommonWordInfo *info);
    void S_AddCommonWordDir(YL_TreeViewItem_CommonWord *parentItem, UICommonWordInfo *info);
    void S_ModifyCommonWordDir(YL_TreeViewItem_CommonWord *parentItem);

public slots:
    void R_addCommonWordFinished(QString code, QString reason, QString body);

private slots:
    void on_m_closeBtn_clicked();
    void on_saveCommonWord_clicked();
    void showToastMsg(QString msg);
    void R_GetInputValue(QString str, int i);
    void R_AddCommonWordcomplete();
    void on_addGroup_clicked();
    void R_currentIndexChanged(int index);

    void on_groupContent_textChanged(const QString &arg1);

    void on_title_textChanged(const QString &arg1);

private:
    Ui::EL_CommonWord_Popup_UI      *ui;
    UI_WebView_ShieldBackSpace      *m_webView;
    EL_CommonWord_JsHelper          *m_jshelper;
    YL_TreeViewItem_CommonWord      *m_item;
    UICommonWordInfo                *m_info;
    EYLUI_CtrlName                   m_type;
    UI_TreeViewItemModel_CommonWord *m_gItemModel;
};

#endif // EL_COMMONWORD_POPUP_UI_H
