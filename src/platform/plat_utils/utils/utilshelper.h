#ifndef UTILSHELPER_H
#define UTILSHELPER_H

#include "utils/DefineHelper.h"
#include "utils/Plat_EnumDefine.h"
#include <QDateTime>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QDebug>


#ifndef _MSC_VER
unsigned long GetTickCount();
#endif

class UtilsHelper : public QObject
{
    Q_OBJECT

public:
    UtilsHelper(QObject *parent);
    ~UtilsHelper();

    static QString GetDownLoadFileDir();

    static QString GetClientIp();
    static QString GetClientMac();

    static ECLIENTTYPE     GetClientTypeByString(const QString &strType);
    static EMESSAGETYPE    GetMessageTypeByString(const QString &strType);
    static EMESSAGETYPE    GetWebMessageTypeByString(const QString &strType);
    static EGROUPEVENTTYPE GetGroupEventTypeByString(const QString &strEvent);
    static EPRESENCESTATUS GetPresenceStatusByString(const QString &status);
    static EVISITORSTATUS  GetVisitorStatusByString(const QString &status);
    static CUSTOMER_LOGIN_TYPE GetLoginTypeByString(const QString &strType);

    static QString replaceTemplate(const QString &jsonString, const QJsonObject &values);
    static QString GetMd5(const QString &val);
    static QString GetUUid();
    static bool    IsPicUrl(const QString &strFileUrl);
    static bool    IsMobilePhone(const QString &val);

    static EPRESENCESTATUS GetPressenceStatus(QString status, QString runningStatus);

    static QString   GetTimeByMSecs(const QString &val);
    static QDateTime GetDateTimeByMSecs(const QString &val);
    static QString   GetDateTimeStringByMSecs(const QString &val, const QString &format);
    static QString   GetDataTimeDurationByMSecs(const QString &val);

    static void SetAutoStart(bool is_auto_start);

    static QString LoadFile(const QString &fileName);
    static void    SaveFile(const QString &fileName, const QString &dataInfo);
    static QString GetRunAppDir();
    static QString GetFullPathByRunDir(const QString &fileName);
    static QString GetAppdataCacheDir();
    static QString GetAppDataCustomSoundDir();
    static QString GetAppdataHttpCacheDir();
    static QString GetAppDataDouyinDir();
    static QString GetAppDataUpdateDir();
    static QString GetAppDataMenuicoDir();

    static QString GetDumpDir();
    static QString GetDumpBakDir();

    static QString GetCacheDir(QString userid);
    static QString GetHttpCacheDataDir(QString userid);
    static QString GetScreenFileName(QString userId);

    static QString GetExtendHtmlDir();
    static QString GetDefaultName(QString position, QString vid);
    static QString GetDefaultNameByDistrict(QString position, QString vid);
    static int     GetRandNumber(int maxNumber);

    static QString GetSearchEngineIcon(QString name, QString searchingType, QString &IconMeaning);
    static QString GetWeixinByString(const QString &body,const QString pconfig);
    static QString GetQQByString(const QString &body);
    static QString GetMobileByString(const QString &body,const QString pconfig);
    static QString GetTelephoneByString(const QString &body,const QString pconfig);

    static QString GetTokenWZ(const QString &uid, const QString &userName, const QString &pwd);
    static QString GetToken(const QString &uid, const QString &userName, const QString &pwd);
    static quint32 NativeKeyCode(quint32 key);
    static quint32 NativeModifiers(quint32 modifiers);

    static unsigned int GetModifyKeyByHotKey(QString strHotKey);
    static unsigned int GetKeyByShotKey(QString strHotKey);

    static QString EncodeUrlByString(const QString &valUrl);
    static void    Execute(QString body);

    static void Execute(QString receiver, QString body);

    static QString GetCustomerHeadIcon(EPRESENCESTATUS state);
    static QString GetCustomerStateIcon(EPRESENCESTATUS eType);
    static QString GetCustomerStateString(EPRESENCESTATUS eType);

    static QString ReplaceHtmlTag(QString line);
    static QString GetNumberByString(const QString &body);

    static bool IsRobot(int robotFlag, QString strUserId);

    static QPixmap ChangeQPixmapDPI(QPixmap pix, QString pixname, QSize size);


    static QString URLtostr(QString url);
    static QPixmap PixmapToRound(const QPixmap &src, int radius, QString colorstr);

    static QString converRGB16HexStr(QColor _color);


    //生成json字符串
    static QString jsontostr(const QVariantMap &remsgMap);
    static QString jsontostr(const QJsonObject &remsgJso);
    //解析json字符串
    static QVariantMap strtojson(QString json);

    static int ptconvertpx(int pt, QWidget *ui = nullptr);

private:
};

#endif // UTILSHELPER_H
