#include "uiyl_centerview.h"
#include "ui_uiyl_centerview.h"
#include "server/platuiservice.h"

UIYL_CenterView::UIYL_CenterView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UIYL_CenterView)
{
    m_IsUnfold = true;
	m_centerContent = nullptr;
    ui->setupUi(this);

    connect(PlatUiService::GetInstance(), SIGNAL(S_LoadUi(int)), this, SLOT(R_LoadUi(int)));

    InitShowOrHideEditPanel();
}

UIYL_CenterView::~UIYL_CenterView()
{
    delete ui;
}


void UIYL_CenterView::R_LoadUi(int type)
{
    m_centerContent = PlatUiService::GetInstance()->GetCenterContent();
	ui->centerContent_Layout->addWidget(m_centerContent);
}

void UIYL_CenterView::InitShowOrHideEditPanel()
{
    m_IsUnfold = true;
    //代码中设置输入功能区窗口最大、最小高度
    ui->inputContent->setMinimumHeight(150);
    ui->inputContent->setMaximumHeight(390);
    //伸缩输入功能区
    connect(ui->inputContent, SIGNAL(S_ShowEditFunWidget(bool)), this, SLOT(R_ShowEditFunWidget(bool)));
}


void UIYL_CenterView::R_ShowEditFunWidget(bool show)
{
    m_IsUnfold = show;
    if (show)
    {
        ui->inputContent->setMinimumHeight(150);
        ui->inputContent->setMaximumHeight(390);
        ui->splitter->setSizes(m_splitterSizes);
    }
    else
    {
        m_splitterSizes = ui->splitter->sizes();
        ui->inputContent->setMinimumHeight(34);
        ui->inputContent->setMaximumHeight(34);
        QList<int> listHeight;
        listHeight.append(m_splitterSizes.at(0) + m_splitterSizes.at(1) - 34);
        listHeight.append(34);
        ui->splitter->setSizes(listHeight);
    }
}
