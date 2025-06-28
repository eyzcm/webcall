#pragma once
#include "platdata_global.h"
#include <QObject>

class PLATDTA_EXPORT UISummarizeInfo : public QObject
{
    Q_OBJECT
public:
    UISummarizeInfo(void);
    ~UISummarizeInfo(void);
    UISummarizeInfo(const UISummarizeInfo& info);
    UISummarizeInfo(UISummarizeInfo* info);

public:
    QString Id() const;
    void    Id(QString val);

    QString ParentId() const;
    void    ParentId(QString val);

    QString Content() const;
    void    Content(QString val);

    QString Title() const;
    void    Title(QString val);

    QList<UISummarizeInfo*> ChildList() const;
    void                    ChildList(QList<UISummarizeInfo*> val);
    void                    AddChild(const UISummarizeInfo& info);
    void                    AddChild(UISummarizeInfo* info);

    bool HasChildren()
    {
        if (m_childList.size() > 0)
        {
            return true;
        }
        return false;
    }

    QString Type() const;
    void    Type(QString val);
    QString SortIndex() const;
    void    SortIndex(QString val);
    QString Color() const;
    void    Color(QString val);
    QString categorySource() const;
    void    categorySource(QString val);


private:
    QString m_id;
    QString m_parentId;
    QString m_content;
    QString m_title;
    //标签类型 0 全部 1 访客名片 2 访客 3 对话记录
    QString                 m_type;
    QString                 m_sortIndex;
    QString                 m_color;
    QString                 m_categorySource;
    QList<UISummarizeInfo*> m_childList;
};
