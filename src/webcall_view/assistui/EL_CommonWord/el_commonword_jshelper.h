#ifndef EL_COMMONWORD_JSHELPER_H
#define EL_COMMONWORD_JSHELPER_H

#include "ui/common/BaseJsCommHelper.h"
#include "utils/EnumDefine.h"

class EL_CommonWord_JsHelper : public BaseJsCommonHelper
{
    Q_OBJECT
public:
    EL_CommonWord_JsHelper(int type = 0);
    void SetUrl(const QString &url);
    void LoadFinished(bool isl);
    int  m_showType;

public slots:
    void R_Import_complete(int affiliation); //常用语导入完成 affiliation 0 公司 1 个人
    void R_GetCommonWordComplete();
};

#endif // EL_COMMONWORD_JSHELPER_H
