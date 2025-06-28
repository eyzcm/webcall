#pragma once
#include <QNetworkAccessManager>

class UINetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT

public:
    QString Token() const
    {
        return m_token;
    }
    void Token(QString val)
    {
        m_token = val;
    }

protected:
    virtual QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData = 0);

private:
    QString m_token;
};
