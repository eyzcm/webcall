#ifndef UIYL_TREEVIEWINTERFACE_H
#define UIYL_TREEVIEWINTERFACE_H

#include <QWidget>
#include "plat_ui_global.h"
class SwitchButton;

class PLAT_UI_EXPORT UIYL_TreeViewInterface : public QWidget
{
    Q_OBJECT
public:
    explicit UIYL_TreeViewInterface(QWidget *parent = nullptr);


    virtual QLineEdit *  SearchLine() = 0;
    virtual SwitchButton * SwithBtn() = 0;
    virtual QWidget * SearchResultWnd() = 0;
    virtual void SelectCurPanel(bool val) = 0;
    virtual void SearchText( const QString text) = 0;
    virtual QWidget * TreeWidget() = 0;



    virtual void OnFlashingTreeView(bool isFlashing ) = 0;
    virtual bool OnClickMainVisitorSearchItem(QString vid, QString type) = 0;


    //可不继承
    virtual void InitSwitchButton( bool isCheck );

    virtual QWidget* HeadWidget() { return nullptr; };


protected:


    QString m_buttonName;

signals:
    void S_SwitchBtnClicked();

};

#endif // UIYL_TREEVIEWINTERFACE_H
