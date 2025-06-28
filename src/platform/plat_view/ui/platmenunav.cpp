#include "platmenunav.h"
#include "ui_platmenunav.h"
#include "data/UIMenuDataInfo.h"

PlatMenuNav::PlatMenuNav(QWidget *parent) :
    El_MenuNavInterface(),
    ui(new Ui::PlatMenuNav)
{
    ui->setupUi(this);
    m_currentBtn = nullptr;
	connect(ui->m_exitBtn, SIGNAL(clicked(bool)), this, SLOT(R_CheckChange(bool)));

}

PlatMenuNav::~PlatMenuNav()
{
    delete ui;
}


void PlatMenuNav::SetMenuData(QString key , UIMenuDataInfo * val )
{
    m_menu.insert(key,val);
    /*

        m_analyzeBtn = new QToolButton(m_navWidget);
        m_analyzeBtn->setObjectName(QString::fromUtf8("m_analyzeBtn"));
        m_analyzeBtn->setMinimumSize(QSize(60, 60));
        m_analyzeBtn->setStyleSheet(QString::fromUtf8("QToolButton{\n"
"	border-image: url(:/Resources/Image/a_normal1.png);	\n"
"	padding-top: 5px;\n"
"	color: #ffffff;	\n"
"}\n"
"QToolButton:hover{\n"
"	border-image: url(:/Resources/Image/a_press.png);\n"
"	padding-top: 5px;\n"
"	color: #ffffff;	\n"
"}\n"
"QToolButton:pressed{\n"
"	border-image: url(:/Resources/Image/a_press.png);\n"
"	padding-top: 5px;\n"
"	color: #ffffff;	\n"
"}\n"
""));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/Resources/Image/a_analyze.png"), QSize(), QIcon::Normal, QIcon::Off);
        m_analyzeBtn->setIcon(icon4);
        m_analyzeBtn->setIconSize(QSize(25, 25));
        m_analyzeBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        verticalLayout->addWidget(m_analyzeBtn);
    */
    /*

        m_queueBtn = new SwitchButton(m_navWidget);
        m_queueBtn->setObjectName(QString::fromUtf8("m_queueBtn"));
        m_queueBtn->setMinimumSize(QSize(60, 60));
        m_queueBtn->setStyleSheet(QString::fromUtf8("QToolButton{\n"
"	border-image: url(:/Resources/Image/a_normal1.png);	\n"
"	padding-top: 5px;\n"
"	color: #ffffff;	\n"
"}\n"
"QToolButton:hover{\n"
"	border-image: url(:/Resources/Image/a_press.png);\n"
"	padding-top: 5px;\n"
"	color: #ffffff;	\n"
"}\n"
"QToolButton:pressed{\n"
"	border-image: url(:/Resources/Image/a_press.png);\n"
"	padding-top: 5px;\n"
"	color: #ffffff;	\n"
"}\n"
""));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Resources/Image/a_monitorqueue.png"), QSize(), QIcon::Normal, QIcon::Off);
        m_queueBtn->setProperty("icon", QVariant(icon2));
        m_queueBtn->setProperty("iconSize", QVariant(QSize(25, 25)));

        verticalLayout->addWidget(m_queueBtn);
    */

	QToolButton * switchBtn = new QToolButton(ui->m_navWidget);
	
	switchBtn->setMinimumSize(QSize(60, 60));
    if( (m_currentBtn == nullptr )&&(val->Name() == "webcall_online" ))
    {
        m_currentBtn = switchBtn;
        SetBtnPressStyle(switchBtn,1);
    }else
        SetBtnPressStyle(switchBtn,0);
	QIcon icon2;
	icon2.addFile(QString::fromUtf8(":/Resources/Image/a_monitorqueue.png"), QSize(), QIcon::Normal, QIcon::Off);
	switchBtn->setProperty("icon", QVariant(icon2));
	switchBtn->setProperty("iconSize", QVariant(QSize(25, 25)));
	switchBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	switchBtn->setText(val->Name() );
	
	
	ui->m_menunavlayout->addWidget(switchBtn);

	m_MenuBtnList.insert(key, switchBtn);
	connect(switchBtn, SIGNAL(clicked(bool)), this, SLOT(R_CheckChange(bool)));

}

QMap<QString, UIMenuDataInfo *> PlatMenuNav::GetMenuData( )
{
    return m_menu;
}


void PlatMenuNav::R_CheckChange(bool val)
{

	//if (val)
	{
		//
		if (ui->m_exitBtn == sender())
		{
			Q_EMIT s_navClick("exit");
			return;
		}
		
        if( m_currentBtn != sender())
        {
			if(m_currentBtn)
				SetBtnPressStyle(m_currentBtn,0);
            m_currentBtn = (QToolButton *)sender();
            SetBtnPressStyle(m_currentBtn,1);

        }
		for (auto iter = m_MenuBtnList.begin(); iter != m_MenuBtnList.end(); iter++)
		{
			if (iter.value() == sender())
			{
				Q_EMIT s_navClick(iter.key());

			}
		}

	}

}

void PlatMenuNav::SetBtnPressStyle(QToolButton *btn, bool val )
{
    QString normall = "a_normal1";
    if( val == true)  normall = "a_press";

    btn->setStyleSheet(QString::fromUtf8("QToolButton{\n"
        "	border-image: url(:/Resources/Image/%1.png);	\n"
        "	padding-top: 5px;\n"
        "	color: #ffffff;	\n"
        "}\n"
        "QToolButton:hover{\n"
        "	border-image: url(:/Resources/Image/a_press.png);\n"
        "	padding-top: 5px;\n"
        "	color: #ffffff;	\n"
        "}\n"
        "QToolButton:pressed{\n"
        "	border-image: url(:/Resources/Image/a_press.png);\n"
        "	padding-top: 5px;\n"
        "	color: #ffffff;	\n"
        "}\n"
        "").arg(normall));

}
