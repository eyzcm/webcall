#pragma once
#include "data/CommInfo.h"
#include "platdata_global.h"
#include <QList>
#include <QObject>

class PLATDTA_EXPORT UIVisitorCol : public QObject
{
    Q_OBJECT
public:
    UIVisitorCol(void);
    UIVisitorCol(const UIVisitorCol& info);
    UIVisitorCol(UIVisitorCol* info);
    ~UIVisitorCol(void);

private:
    QString          m_colType;
    bool             m_isHidden;
    QString          m_id;
    QString          m_colName;
    QString          m_selfText;
    QString          m_Itemtype;
    QString          m_text;
    int              m_sortIndex;
    QList<CommInfo*> m_itemList;

public:
    int  SortIndex() const;
    void SortIndex(int val);

    QString ColType() const;
    void    ColType(QString val);

    bool Hidden() const;
    void Hidden(bool val);

    QString Id() const;
    void    Id(QString val);

    QString ColName() const;
    void    ColName(QString val);

    QString SelfText() const;
    void    SelfText(QString val);

    QString Itemtype() const;
    void    Itemtype(QString val);
    QString Text() const;
    void    Text(QString val);

    QList<CommInfo*> ItemList() const;
    void             ItemList(QList<CommInfo*> val);

    void AddItem(const CommInfo& info);
};
