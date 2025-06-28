#pragma once
#include "data/UIVisitorInfo.h"
#include <QObject>
#include <QString>
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT DbUiVisitorSignal : public QObject
{
      Q_OBJECT

protected:
    DbUiVisitorSignal(void);

public:
    ~DbUiVisitorSignal(void);

signals:
      void S_AddVisitor(UIVisitorInfo info);
      void S_UpdateVisitor(UIVisitorInfo info);
      void S_DelVisitor(QString uid);

protected:
};
