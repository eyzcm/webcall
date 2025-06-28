#ifndef JsUserCheckHelper_H
#define JsUserCheckHelper_H

#include "BaseJsLoginHelper.h"

class JsUserCheckHelper : public BaseJsLoginHelper
{
    Q_OBJECT

public:
    JsUserCheckHelper();
    ~JsUserCheckHelper();

public:
    void         SetInitData();
    virtual void LoadFinished();

    void SetUrl(const QString& url);
    void SetIsPwdSafed(int isPwdSafed);

    void SetClientParam(QString key, QString val);

    int CheckModify();

signals:

    void S_quitSys();

public slots:

    void    JsSetClientParam(QString key, QString val);
    QString JsGetClientParam(QString key);

    void JsQuitSys();

private:
    QString m_token;
    QString m_userId;
    QString m_companyId;
    QString m_version;

    int m_isModify;
    int m_isPwdSafed;
};

#endif // JsUserCheckHelper_H
