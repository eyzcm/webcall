#pragma once
#include <QObject>

class ITcpWork : public QObject
{
    Q_OBJECT
public:
    ITcpWork(void);
    virtual ~ITcpWork(void);
    virtual void InitSignalConnect(QObject *obj);
    virtual void TcpConnect();
    virtual void Login();
    virtual void DispConnect();
    virtual void SetStatus(int status);

signals:
    void S_TcpConnect();
    void S_Login();
    void S_DispConnect();

private slots:
    virtual void R_TcpConnect();
    virtual void R_Login();
    virtual void R_DispConnect();
    virtual void R_connected();

private:
    void Init();
};
