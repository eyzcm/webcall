#include "promise.h"

Promise::Promise(QObject *parent) : QObject(parent)
{

}


void Promise::emit_error(const QVariant & val)
{
    Q_EMIT  s_error( val);
}

void Promise::emit_success(const QVariant & val)
{
    Q_EMIT  s_success(val);
}

void Promise::emit_finished()
{
    Q_EMIT  s_finished();
}
