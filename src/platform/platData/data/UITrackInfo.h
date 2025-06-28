#pragma once

#include <QDateTime>
#include <QObject>
#include "platdata_global.h"

class PLATDTA_EXPORT UITrackInfo : public QObject
{
    Q_OBJECT
public:
    UITrackInfo(void);
    UITrackInfo(const UITrackInfo& info);
    UITrackInfo(UITrackInfo* info);
    ~UITrackInfo(void);

private:
    QString m_activeTime;
    QString m_companyId;
    QString m_createTime;
    QString m_htmlName;
    QString m_id;
    QString m_keyWord;
    QString m_promotionId;
    QString m_promotionName;
    QString m_refer;
    QString m_searchEngineId;
    QString m_searching;
    QString m_siteId;
    QString m_spreadFlag;
    QString m_url;
    //静态id
    QString m_userId;
    // vid 访客id
    QString m_viewerId;
    QString m_htmlAlias;

    bool m_isEndBrowser;

public:
    QString ActiveTime() const;
    void    ActiveTime(QString val);

    QString CompanyId() const;
    void    CompanyId(QString val);

    QString CreateTime() const;
    void    CreateTime(QString val);

    QString HtmlAlias() const;
    void    HtmlAlias(QString val);

    QString HtmlName() const;
    void    HtmlName(QString val);

    QString Id() const;
    void    Id(QString val);

    QString KeyWord() const;
    void    KeyWord(QString val);

    QString PromotionId() const;
    void    PromotionId(QString val);

    QString PromotionName() const;
    void    PromotionName(QString val);

    QString Refer() const;
    void    Refer(QString val);

    QString SearchEngineId() const;
    void    SearchEngineId(QString val);

    QString Searching() const;
    void    Searching(QString val);

    QString SiteId() const;
    void    SiteId(QString val);

    QString SpreadFlag() const;
    void    SpreadFlag(QString val);

    QString Url() const;
    void    Url(QString val);

    QString StaticId() const;
    void    StaticId(QString val);

    QString ViewerId() const;
    void    ViewerId(QString val);

    bool IsEndBrowser() const
    {
        return m_isEndBrowser;
    }
    void IsEndBrowser(bool val)
    {
        m_isEndBrowser = val;
    }
};
