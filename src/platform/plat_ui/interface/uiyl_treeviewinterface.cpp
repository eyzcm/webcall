#include "uiyl_treeviewinterface.h"
#include "ui/switchbutton.h"

UIYL_TreeViewInterface::UIYL_TreeViewInterface(QWidget *parent) : QWidget(parent)
{

}



void UIYL_TreeViewInterface::InitSwitchButton( bool isCheck)
{
    SwitchButton *btn = SwithBtn();
    QString checkStyle   = "";
    QString unCheckStyle = "";
    QString strUncheck   = "QToolButton"
                           "{"
                           "background-color:rgba(255,255,255,204);"
                           "border:0px;"
                           "padding: 5px;"
                           "image: url(:/Resources/SVG/%1.svg);"
                           "}"
                           "QToolButton:hover,QToolButton:pressed"
                           "{"
                           "image: url(:/Resources/SVG/%2.svg);"
                           "}";

    QString strCheck = "QToolButton,QToolButton:hover,QToolButton:pressed"
                       "{"
                       "background-color: rgba(255, 255, 255, 230);"
                       "border:0 px;"
                       "padding: 5px;"
                       "image: url(:/Resources/SVG/%1.svg);"
                       "}";
        unCheckStyle = strUncheck.arg(m_buttonName+"_normal",m_buttonName+"_over");
        checkStyle   = strCheck.arg(m_buttonName+"_over");


    btn->SetCheckStyle(checkStyle);
    btn->SetUnCheckStyle(unCheckStyle);
    btn->IsBtnChecked(isCheck);
    btn->Init();
}

