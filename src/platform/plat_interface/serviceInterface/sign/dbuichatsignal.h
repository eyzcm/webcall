#ifndef DBUICHATSIGNAL_H
#define DBUICHATSIGNAL_H

#include "data/UIChatInfo.h"
#include <QObject>
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT DbUiChatSignal : public QObject
{
      Q_OBJECT

public:
    DbUiChatSignal();
    ~DbUiChatSignal();

signals:
      void S_AddChat(UIChatInfo info);
      void S_UpdateChat(UIChatInfo info);
      void S_DelChat(QString chatId, QString customerID, QMap<QString, QString> extendInfo);

private:
};

#endif // DBUICHATSIGNAL_H
