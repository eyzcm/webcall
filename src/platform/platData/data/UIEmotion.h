#pragma once
#include "platdata_global.h"
#include <QObject>

class PLATDTA_EXPORT UIEmotion : public QObject
{
    Q_OBJECT
public:
    UIEmotion(void);
    UIEmotion(const UIEmotion& info);
    UIEmotion(UIEmotion* info);
    ~UIEmotion(void);

public:
    QString Tip() const;
    void    Tip(QString val);

    QString Key() const;
    void    Key(QString val);

    QString IconName() const;
    void    IconName(QString val);

private:
    QString m_key;
    QString m_iconName;
    QString m_tip;
};
