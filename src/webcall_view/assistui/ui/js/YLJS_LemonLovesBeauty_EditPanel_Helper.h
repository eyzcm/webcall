#pragma once

#include "ui/common/BaseJsCommHelper.h"

class YLJS_LemonLovesBeauty_EditPanel_Helper : public BaseJsCommonHelper
{
    Q_OBJECT

public:
    YLJS_LemonLovesBeauty_EditPanel_Helper();
    ~YLJS_LemonLovesBeauty_EditPanel_Helper();

public:
    void SetUrl(const QString& url);
    void SetInitData();

public slots:
    void R_RedEnvelopesLinks_Checked(QString databody, QString links);

signals:
    void S_RedEnvelopesLinks_Checked(QString databody, QString links);

private:
    QString m_token;
    QString m_userId;
    QString m_companyId;
    QString m_version;
};
