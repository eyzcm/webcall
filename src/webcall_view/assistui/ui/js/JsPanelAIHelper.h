#ifndef JsPanelAIHelper_H
#define JsPanelAIHelper_H

#include "ui/common/BaseJsCommHelper.h"
#include "data/MChatItemData.h"

class JsPanelAIHelper : public BaseJsCommonHelper
{
    Q_OBJECT

public:
    JsPanelAIHelper();
    ~JsPanelAIHelper();

public:
    void         SetInitData(QString val);
    virtual void LoadFinished();

    void SetUrl(const QString& url);

    void SetClientParam(QString key, QString val);

    int CheckModify();

signals:

    void S_ReturnAIData(QString key, QString json);
    void S_OpenWindowParam(QString key, QString json);

public slots:

    void    JsSetClientParam(QString key, QString val);
    QString JsGetClientParam(QString key);

    void JsOpenAIWindow(QString key, QString json);

private slots:
    void R_ReturnAIData(QString key, QString json);

private:
    int     m_isModify;
    int     m_aiInit;
    QString m_initData;
};

#endif // JsPanelNewTabHelpe_H
