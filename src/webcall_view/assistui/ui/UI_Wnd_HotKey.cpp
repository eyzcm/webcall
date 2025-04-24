#include "UI_Wnd_HotKey.h"
#include "model\YL_Model\UI_Model_HotKey.h"

UI_Wnd_HotKey::UI_Wnd_HotKey(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    this->setWindowTitle(tr("设置快捷键"));
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    UI_Model_HotKey::GetInstance()->RegisterObserver(ui.panel_hotKey);
    UI_Model_HotKey::GetInstance()->InitData();

    connect(ui.panel_hotKey, SIGNAL(S_AddHotKey(QString)), this, SIGNAL(S_AddHotKey(QString)));
}

UI_Wnd_HotKey::~UI_Wnd_HotKey()
{
}
