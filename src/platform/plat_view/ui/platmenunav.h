#ifndef PLATMENUNAV_H
#define PLATMENUNAV_H

#include "plat_view_global.h"
#include "session/el_menunavinterface.h"
#include <QWidget>
#include "ui/SwitchButton.h"
#include <QMap>

namespace Ui {
class PlatMenuNav;
}

class PLAT_VIEW_EXPORT PlatMenuNav : public El_MenuNavInterface
{
    Q_OBJECT

public:
    explicit PlatMenuNav(QWidget *parent = nullptr);
    ~PlatMenuNav();

    virtual void SetMenuData(QString key , UIMenuDataInfo * val ) ;
    virtual QMap<QString, UIMenuDataInfo *> GetMenuData( ) ;

private slots:
	void R_CheckChange(bool val);
private:
    void SetBtnPressStyle(QToolButton *btn, bool val );
private:
    Ui::PlatMenuNav *ui;

    QToolButton * m_currentBtn;
    QMap<QString, UIMenuDataInfo *>  m_menu;
    QMap<QString, QToolButton *>  m_MenuBtnList;
};

#endif // PLATMENUNAV_H
