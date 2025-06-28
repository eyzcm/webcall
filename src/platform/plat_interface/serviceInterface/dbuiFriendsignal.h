#ifndef DbUiFriendSignal_H
#define DbUiFriendSignal_H

#include "data/UIChatInfo.h"
#include "data/UIContactInfo.h"
#include <QObject>
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT DbUiFriendSignal : public QObject
{
      Q_OBJECT

public:
    DbUiFriendSignal();
    ~DbUiFriendSignal();

signals:
      void S_AddFriend(UIContactInfo info);
      void S_UpdateFriend(UIContactInfo info);
      void S_DelFriend(QString uid);

private:
};

#endif // DbUiFriendSignal_H
