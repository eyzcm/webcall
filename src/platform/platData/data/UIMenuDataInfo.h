#pragma once
#include "QMap"
#include <QObject>
#include <QStringList>
#include "platdata_global.h"


class PLATDTA_EXPORT UIMenuDataInfo : public QObject
{
    Q_OBJECT

public:
    UIMenuDataInfo(void);
    UIMenuDataInfo(UIMenuDataInfo *info);
    ~UIMenuDataInfo(void);

public:
    int Id() const
    {
        return m_id;
    }
    void Id(int val)
    {
        m_id = val;
    }

    QString Name() const;
    void    Name(QString val);

    QString Code() const;
    void    Code(QString val);

    QString Url() const;
    void    Url(QString val);

    int  Score() const;
    void Score(int val);

    QString IconFile() const;
    void    IconFile(QString val);

    QString IconUrl() const;
    void    IconUrl(QString val);

    QString IconUrlClicked() const;
    void    IconUrlClicked(QString val);

    int  ParentId() const;
    void ParentId(int val);

    int  Flag() const;
    void Flag(int val);

    int  Opentype() const;
    void Opentype(int val);

	QString SubName() const;
    void    SubName(QString val);

    void                   LoadMapInfo(const QMap<QString, QString> &mapInfo);
    QMap<QString, QString> InfoMap() const;

private:
    QString m_MenuDataInfoId;

    QString m_name;           //查看名片
	QString m_subName;       //菜单类别
    QString m_code;           //代码
    QString m_url;            // url
    int     m_score;          //排序
    QString m_iconUrl;        //图标
    QString m_iconFile;       //本地图标
    QString m_iconUrlClicked; //点击图标
    int     m_id;
    int     m_parentId; //父id
    int     m_flag;     //标志
    int     m_opentype; //打开方式
};
