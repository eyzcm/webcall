#ifndef AUXILIARYFUNPANENLINTERFACE_H
#define AUXILIARYFUNPANENLINTERFACE_H

#include <QWidget>

class AuxiliaryFunPanenlInterface : public QWidget
{
    Q_OBJECT
public:
    explicit AuxiliaryFunPanenlInterface(QWidget *parent = nullptr);

    virtual QPoint getNoticepos() = NULL; //获取公告按钮位置

signals:

    /*
     *
    m_pPanenl_AuxiliaryFun = new UI_Widget_AuxiliaryFun_Panenl;
    connect(m_pPanenl_AuxiliaryFun, SIGNAL(S_ChangeSkin_Clicked(int)), this, SLOT(R_ChangeSkin_Clicked(int)));
    connect(m_pPanenl_AuxiliaryFun, SIGNAL(S_Assistant_Clicked()), this, SLOT(R_Assistant_Clicked()));
    connect(m_pPanenl_AuxiliaryFun, SIGNAL(S_ShowNotice_Clicked()), this, SLOT(R_ShowNoticePanel()));
    connect(m_pPanenl_AuxiliaryFun, SIGNAL(S_ShowAnnouncement_Details_Clicked()), this, SLOT(R_ShowAnnouncement_Details()));
    */

private slots:
    virtual void R_ChangeSkin_Clicked(int type) = 0;


    /* 曹振超 顶部功能按钮 */
private slots:
    virtual void  R_CloseWidget_Assistant() = 0;
    virtual void  R_Assistant_Clicked() = 0;

};

#endif // AUXILIARYFUNPANENLINTERFACE_H
