#ifndef UIYL_CENTERVIEW_H
#define UIYL_CENTERVIEW_H

#include <QWidget>
#include "plat_view_global.h"
#include "interface/uiyl_centercontentinterface.h"

namespace Ui {
class UIYL_CenterView;
}

class PLAT_VIEW_EXPORT UIYL_CenterView : public QWidget
{
    Q_OBJECT

public:
    explicit UIYL_CenterView(QWidget *parent = nullptr);
    ~UIYL_CenterView();


private:
    void InitShowOrHideEditPanel();

private slots:
    void R_LoadUi(int type);
    void R_ShowEditFunWidget(bool show);

private:
    Ui::UIYL_CenterView *ui;

	UIYL_CenterContentInterface * m_centerContent;

    bool m_IsUnfold;
    QList<int>     m_splitterSizes;

};

#endif // UIYL_CENTERVIEW_H
