#include "UIYL_CommonWordTreeSearchResultWidget.h"
#include "model\YL_Model\UIYL_CommonWordSearchModel.h"

UIYL_CommonWordTreeSearchResultWidget::UIYL_CommonWordTreeSearchResultWidget(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    Init();
}

UIYL_CommonWordTreeSearchResultWidget::~UIYL_CommonWordTreeSearchResultWidget()
{
}

void UIYL_CommonWordTreeSearchResultWidget::Init()
{
    UIYL_CommonWordSearchModel::GetInstance()->RegisterObserver(ui.m_commonWordListView);
    connect(ui.m_commonWordListView, SIGNAL(S_SetSearchResultNull(bool)), this, SLOT(R_SetSearchResultNull(bool)));
}

void UIYL_CommonWordTreeSearchResultWidget::SearchText(QString text)
{
    UIYL_CommonWordSearchModel::GetInstance()->SearchCustomerByKey(text);
}

void UIYL_CommonWordTreeSearchResultWidget::R_SetSearchResultNull(bool result)
{
    if (!result)
    {
        if (ui.widget_bg->styleSheet().indexOf("image:url") != -1)
        {
            QString strStyle = "QWidget#widget_bg{border:1px solid #dddddd;background-color:rgb(255,255,255);}";
            ui.widget_bg->setStyleSheet(strStyle);
        }
    }
    else
    {
        if (ui.widget_bg->styleSheet().indexOf("image:url") == -1)
        {
            QString strStyle = "QWidget#widget_bg{border:1px solid #dddddd;background-color:rgb(255,255,255);";
            strStyle += "image:url(:/YL/Resources/YL/MainTree/icon_searchResultNull.png);}";
            ui.widget_bg->setStyleSheet(strStyle);
        }
    }
}
