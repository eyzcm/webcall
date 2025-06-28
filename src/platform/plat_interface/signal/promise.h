#ifndef PROMISE_H
#define PROMISE_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT Promise : public QObject
{

    Q_OBJECT
public:
    explicit Promise(QObject *parent = nullptr);

    virtual Promise * success(std::function<void(const QVariant &)> successFunc) {
            m_successFunc = successFunc;
            return this;
        }

    virtual    Promise * error(std::function<void(const QVariant &)> errorFunc) {
            m_errorFunc = errorFunc;
            return this;
        }
    virtual    Promise * final(std::function<void(const QVariant &)> finalFunc) {
            m_finalFunc = finalFunc;
            return this;
        }

    virtual    void execute(bool isSuccess, const QVariant &map ) {

                bool success = isSuccess;
                if (success) {
                    if (m_successFunc) {
                        m_successFunc(map);
                    }
                    emit_success(map );
                } else {
                    if (m_errorFunc) {
                        m_errorFunc(map);
                    }
                    emit_error(map);
                }
                if( m_finalFunc)
                {
                    m_finalFunc(map);
                    deleteLater();
                }
                emit_finished();

            }
signals:
    void s_error(const QVariant &val);
    void s_success(const QVariant &val);
    void s_finished();


private:
    void emit_error(const QVariant &val);
    void emit_success(const QVariant &val);
    void emit_finished();

    std::function<void(const QVariant &)> m_successFunc;
    std::function<void(const QVariant &)> m_errorFunc;
    std::function<void(const QVariant &)> m_finalFunc;

};
typedef QSharedPointer<Promise> PromisePtr;

#endif // PROMISE_H
