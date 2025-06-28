#include "utils/utilshelper.h"
#include <qpainter.h>
#include "utils/ConstStringHelper.h"
#include "utils/DefineHelper.h"
#include <QApplication>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDateTime>
#include <QDesktopWidget>
#include <QDir>
#include <QFile>
#include <QHostInfo>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QPainter>
#include <QProcessEnvironment>
#include <QRegExp>
#include <QSettings>
#include <QSvgRenderer>
#include <QUrl>
#include <QUuid>
#include <QStandardPaths>
#ifdef _MSC_VER
#include <Windows.h>
#else 
#ifndef UINT 
#define UINT unsigned int
#endif
#endif
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QScreen>
#include <QPainterPath>

#ifndef NOMAINUI
//#include "control/IUIController.h"
#include "service/dbuiservice.h"
#endif

#include "service/dbuiservice.h"

#include <QValidator>

UtilsHelper::UtilsHelper(QObject *parent) : QObject(parent)
{
}

UtilsHelper::~UtilsHelper()
{
}

QString UtilsHelper::GetClientIp()
{
    QString   localHostName = QHostInfo::localHostName();
    QHostInfo info          = QHostInfo::fromName(localHostName);
    foreach (QHostAddress address, info.addresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();
    }
    return "";
}

QString UtilsHelper::GetClientMac()
{
    QString                  strMac;
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces(); //获取所有网卡信息
    for (int i = 0; i < ifaces.count(); i++)
    {
        QNetworkInterface iface = ifaces.at(i);
        if (iface.flags().testFlag(QNetworkInterface::IsUp) && iface.flags().testFlag(QNetworkInterface::IsRunning) && !iface.flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            //获取当前有效网卡
            for (int j = 0; j < iface.addressEntries().count(); j++) //这个循环很重要，读者可以尝试不用这个循环看看获取的是不是当前有效网卡：）
            {
                /* we have an interface that is up, and has an ip address
                 * therefore the link is present
                 * we will only enable this check on first positive,
                 * all later results are incorrect * * */
                strMac = iface.hardwareAddress();
                i      = ifaces.count(); //跳出外层循环
                break;
            }
        }
    }
    if (strMac.isEmpty())
    {
        //有时候还是获取不到，所以这边再次尝试获取
        foreach (QNetworkInterface iface, ifaces)
        {
            if (!iface.flags().testFlag(QNetworkInterface::IsLoopBack))
            {
                strMac = iface.hardwareAddress();
                break;
            }
        }
    }
    strMac = strMac.replace(":", "-");
    return strMac;
}

QString UtilsHelper::GetSearchEngineIcon(QString name, QString searchingType, QString &IconMeaning)
{
    QString iconUri = "";
    IconMeaning     = tr("搜索引擎-未知");
    do
    {
        if (!name.isEmpty())
        {
            if (name == "google")
            {
                iconUri     = "a_source_google";
                IconMeaning = tr("搜索引擎-谷歌");
            }
            else if (name == "baidu")
            {
                iconUri     = "a_source_baidu";
                IconMeaning = tr("搜索引擎-百度");
            }
            else if (name == "sogou")
            {
                iconUri     = "a_source_sogou";
                IconMeaning = tr("搜索引擎-搜狗");
            }
            else if (name == "360")
            {
                iconUri     = "a_source_360";
                IconMeaning = tr("搜索引擎-360");
            }
            else if (name == "so.m.sm") //神马
            {
                iconUri     = "a_source_so.m.sm";
                IconMeaning = tr("搜索引擎-神马");
            }
            else if (name == "chengzijianzhan") //头条
            {
                iconUri     = "a_source_chengzijianzhan";
                IconMeaning = tr("搜索引擎-今日头条");
            }
            else if (name == "fmc.qq") //广点通
            {
                iconUri     = "a_source_fmc.qq";
                IconMeaning = tr("搜索引擎-广点通");
            }
            else if (name == "kuaishou") //快手
            {
                iconUri     = "a_source_kuaishou";
                IconMeaning = tr("搜索引擎-快手");
            }
            else if (name == "WeChatPublic")
            {
                iconUri     = "a_source_weixin";
                IconMeaning = tr("搜索引擎-微信");
            }
            else if (name == "EnterpriseWeChat")
            {
                iconUri     = "a_source_enterpriseWeChat";
                IconMeaning = tr("搜索引擎-微信客服");
            }
            else if (name == "WeiBo")
            {
                iconUri     = "a_source_weibo";
                IconMeaning = tr("搜索引擎-微博");
            }
            else if (name == "DouYin")
            {
                iconUri     = "a_source_douyin";
                IconMeaning = tr("搜索引擎-抖音");
            }
            else if (name == "origMiniApps")
            {
                iconUri     = "a_source_origMiniApps";
                IconMeaning = tr("搜索引擎-微信小程序");
            }
            else
            {
                iconUri = "a_source_unknow";
            }
        }
        else
        {
            if (searchingType == "so.m.sm")
            {
                iconUri     = "a_source_so.m.sm";
                IconMeaning = tr("搜索引擎-神马");
            }
            else if (searchingType == "chengzijianzhan")
            {
                iconUri     = "a_source_chengzijianzhan";
                IconMeaning = tr("搜索引擎-今日头条");
            }
            else if (searchingType == "fmc.qq")
            {
                iconUri     = "a_source_fmc.qq";
                IconMeaning = tr("搜索引擎-广点通");
            }
            else if (searchingType == "kuaishou")
            {
                iconUri     = "a_source_kuaishou";
                IconMeaning = tr("搜索引擎-快手");
            }
            else if (searchingType == "EnterpriseWeChat")
            {
                iconUri     = "a_source_enterpriseWeChat";
                IconMeaning = tr("搜索引擎-微信客服");
            }
            else if (searchingType == "WeChatPublic")
            {
                iconUri     = "a_source_weixin";
                IconMeaning = tr("搜索引擎-微信");
            }
            else if (searchingType == "WeiBo")
            {
                iconUri     = "a_source_weibo";
                IconMeaning = tr("搜索引擎-微博");
            }
            else if (searchingType == "DouYin")
            {
                iconUri     = "a_source_douyin";
                IconMeaning = tr("搜索引擎-抖音");
            }
            else if (searchingType == "origMiniApps")
            {
                iconUri     = "a_source_origMiniApps";
                IconMeaning = tr("搜索引擎-微信小程序");
            }
            else
            {
                iconUri = "a_source_unknow";
            }
        }
    } while (false);
    return iconUri;
}


ECLIENTTYPE UtilsHelper::GetClientTypeByString(const QString &strType)
{
    ECLIENTTYPE retType = E_CLIENT_NEWMEDICAL; //新医疗版;
    if (strType == "5")
    {
        retType = E_CLIENT_EDUEDITION;
	}
	else if (strType == "7")
	{
		retType = E_CLIENT_WEB;
	}
    return retType;
}

CUSTOMER_LOGIN_TYPE UtilsHelper::GetLoginTypeByString(const QString &strType)
{
    CUSTOMER_LOGIN_TYPE loginType = CUSTOMER_LOGIN_CLIENT;
    if (strType == "TcpServer")
    {
        loginType = CUSTOMER_LOGIN_CLIENT;
    }
    else if (strType == "TRUST")
    {
        loginType = CUSTOMER_LOGIN_TRUST;
    }
    else if (strType == "AppServer")
    {
        loginType = CUSTOMER_LOGIN_APP;
    }
    return loginType;
}


EMESSAGETYPE UtilsHelper::GetMessageTypeByString(const QString &strType)
{
    EMESSAGETYPE ret = E_MSGTYPE_DEFAULT;

    if (strType == "close")
    {
        ret = E_MSGTYPE_EVENT_CLOSE;
    }
    else if (strType == "quit")
    {
        ret = E_MSGTYPE_EVENT_END;
    }
    else if (strType == "autoAcceptInvite")
    {
        ret = E_MSGTYPE_AUTOACCEPTINVITE;
    }
    else if (strType == "autoMonitor")
    {
        ret = E_MSGTYPE_AUTOMONTOR;
    }
    else if (strType == "composing")
    {
        ret = E_MSGTYPE_COMPOSING;
    }
    else if (strType == "deny")
    {
        ret = E_MSGTYPE_DENY;
    }
    else if (strType == "file")
    {
        ret = E_MSGTYPE_FILE;
    }
    else if (strType == "message")
    {
        ret = E_MSGTYPE_MESSAGE;
    }
    else if (strType == "system_message")
    {
        ret = E_MSGTYPE_SYSMESSAGE;
    }
    else if (strType == "system_message_welcome")
    {
        ret = E_MSGTYPE_SYSMESSAGE_WELCOME;
    }
    else if (strType == "monitorSuccess")
    {
        ret = E_MSGTYPE_MONITORSUCCESS;
    }
    else if (strType == "opinion")
    {
        ret = E_MSGTYPE_OPINION;
    }
    else if (strType == "screenShots")
    {
        ret = E_MSGTYPE_SCREENSHOTS;
    }
    else if (strType == "trans")
    {
        ret = E_MSGTYPE_TRANS;
    }
    else if (strType == "EVENT_ACK_RESPONSE")
    {
        //会话保活协议类型
        ret = E_MSGTYPE_ACK_RESPONSE;
    }
    else if (strType == "EVENT_LEAVE_MSG")
    {
        //留言
        ret = E_MSGTYPE_LEAVE_MSG;
    }
    else if (strType == "EVENT_FREE_PHONE")
    {
        //免费电话
        ret = E_MSGTYPE_FREE_PHONE;
    }
    else if (strType == "")
    {
        // ret = E_MSGTYPE_FREE_PHONE;
    }
    else if (strType == "EVENT_POP_MESSAGE")
    {
        //加入销售分配到名片的提示
        ret = E_EVENT_POP_MESSAGE;
    }
    else if (strType == "1")
    {
        ret = E_MSGTYPE_MESSAGE;
    }
    else if (strType == "2")
    {
        ret = E_MSGTYPE_FILE;
    }
    else if (strType == "3")
    {
        ret = E_MSGTYPE_SCREENSHOTS;
    }
    else if (strType == "EVENT_CLOSE")
    {
        ret = E_MSGTYPE_EVENT_CLOSE;
    }
    else if (strType == "EVENT_END")
    {
        ret = E_MSGTYPE_EVENT_END;
    }
    else if (strType == "extendInfoChanged")
    {
        ret = E_EXENDINFO_CHANGED;
    }
    else if (strType == "revoke")
    {
        ret = E_MSGTYPE_REVOKE;
    }
    else if (strType == "takeover")
    {
        ret = E_MSGTYPE_TOKEOVER;
    }
    else if (strType == "acceptTakeover")
    {
        ret = E_MSGTYPE_ACCEPTTAKEOVER;
    }
    else if (strType == "readReceipt")
    {
        ret = E_MSGTYPE_READRECEIPT;
    }
    else if (strType == "eventCustom")
    {
        ret = E_MSGTYPE_EVENTCUSTOM;
    }
    else if (strType == "wxVoice")
        {
            ret = E_MSGTYPE_WXVOICE;
        }
        else if (strType == "video")
        {
            ret = E_MSGTYPE_VIDEO;
        }
        else if (strType == "voice")
        {
            ret = E_MSGTYPE_VOICE;
        }
        else if (strType == "image")
        {
            ret = E_MSGTYPE_IMAGE;
        }
    else
    {
        Q_ASSERT(false);
    }
    return ret;
}


EMESSAGETYPE UtilsHelper::GetWebMessageTypeByString(const QString &strType)
{
    EMESSAGETYPE ret = E_MSGTYPE_DEFAULT;

    if (strType == "close")
    {
        ret = E_MSGTYPE_EVENT_CLOSE;
    }
    else if (strType == "quit")
    {
        ret = E_MSGTYPE_EVENT_END;
    }
    else if (strType == "autoAcceptInvite")
    {
        ret = E_MSGTYPE_AUTOACCEPTINVITE;
    }
    else if (strType == "autoMonitor")
    {
        ret = E_MSGTYPE_AUTOMONTOR;
    }
    else if (strType == "composing")
    {
        ret = E_MSGTYPE_COMPOSING;
    }
    else if (strType == "deny")
    {
        ret = E_MSGTYPE_DENY;
    }
    else if (strType == "file")
    {
        ret = E_MSGTYPE_FILE;
    }
    else if (strType == "message")
    {
        ret = E_MSGTYPE_MESSAGE;
    }
    else if (strType == "system_message")
    {
        ret = E_MSGTYPE_SYSMESSAGE;
    }
    else if (strType == "system_message_welcome")
    {
        ret = E_MSGTYPE_SYSMESSAGE_WELCOME;
    }
    else if (strType == "monitorSuccess")
    {
        ret = E_MSGTYPE_MONITORSUCCESS;
    }
    else if (strType == "opinion")
    {
        ret = E_MSGTYPE_OPINION;
    }
    else if (strType == "screenShots")
    {
        ret = E_MSGTYPE_SCREENSHOTS;
    }
    else if (strType == "trans")
    {
        ret = E_MSGTYPE_TRANS;
    }
    else if (strType == "EVENT_ACK_RESPONSE")
    {
        //会话保活协议类型
        ret = E_MSGTYPE_ACK_RESPONSE;
    }
    else if (strType == "EVENT_LEAVE_MSG")
    {
        //留言
        ret = E_MSGTYPE_LEAVE_MSG;
    }
    else if (strType == "EVENT_FREE_PHONE")
    {
        //免费电话
        ret = E_MSGTYPE_FREE_PHONE;
    }
    else if (strType == "")
    {
        // ret = E_MSGTYPE_FREE_PHONE;
    }
    else if (strType == "EVENT_POP_MESSAGE")
    {
        //加入销售分配到名片的提示
        ret = E_EVENT_POP_MESSAGE;
    }
    else if (strType == "1")
    {
        ret = E_MSGTYPE_MESSAGE;
    }
    else if (strType == "2")
    {
        ret = E_MSGTYPE_FILE;
    }
    else if (strType == "3")
    {
        ret = E_MSGTYPE_SCREENSHOTS;
    }
    else if (strType == "EVENT_CLOSE")
    {
        ret = E_MSGTYPE_EVENT_CLOSE;
    }
    else if (strType == "EVENT_END")
    {
        ret = E_MSGTYPE_EVENT_END;
    }
    else if (strType == "extendInfoChanged")
    {
        ret = E_EXENDINFO_CHANGED;
    }
    else if (strType == "revoke")
    {
        ret = E_MSGTYPE_REVOKE;
    }
    else if (strType == "takeover")
    {
        ret = E_MSGTYPE_TOKEOVER;
    }
    else if (strType == "acceptTakeover")
    {
        ret = E_MSGTYPE_ACCEPTTAKEOVER;
    }
    else if (strType == "readReceipt")
    {
        ret = E_MSGTYPE_READRECEIPT;
    }
    else if (strType == "eventCustom")
    {
        ret = E_MSGTYPE_EVENTCUSTOM;
    }
    else if (strType == "wxVoice")
        {
            ret = E_MSGTYPE_WXVOICE;
        }
        else if (strType == "video")
        {
            ret = E_MSGTYPE_VIDEO;
        }
        else if (strType == "voice")
        {
            ret = E_MSGTYPE_VOICE;
        }
        else if (strType == "image")
        {
            ret = E_MSGTYPE_IMAGE;
        }
    else
    {
        Q_ASSERT(false);
    }
    return ret;
}


EGROUPEVENTTYPE UtilsHelper::GetGroupEventTypeByString(const QString &strEvent)
{
    EGROUPEVENTTYPE ret = E_GROUPEVENTTYPE_DEFAULT;
    if (strEvent == "1")
    {
        ret = E_GROUPEVENTTYPE_ADD;
    }
    else if (strEvent == "2")
    {
        ret = E_GROUPEVENTTYPE_DELETE;
    }
    else if (strEvent == "3")
    {
        ret = E_GROUPEVENTTYPE_RENAME;
    }
    else if (strEvent == "4")
    {
        ret = E_GROUPEVENTTYPE_ADDMEMBER;
    }
    else if (strEvent == "5")
    {
        ret = E_GROUPEVENTTYPE_DELETEMEMBER;
    }
    else if (strEvent == "6")
    {
        ret = E_GROUPEVENTTYPE_VISITOR_IN;
    }
    else if (strEvent == "7")
    {
        ret = E_GROUPEVENTTYPE_VISITOR_OUT;
    }
    else
    {
        Q_ASSERT(false);
    }
    return ret;
}

EPRESENCESTATUS UtilsHelper::GetPresenceStatusByString(const QString &status)
{
    EPRESENCESTATUS presenceStatus = E_OFFLINE;
    if (status == "online")
    {
        presenceStatus = E_ONLINE;
    }
    else if (status == "leave")
    {
        presenceStatus = E_LEAVE;
    }
    else if (status == "buzy")
    {
        presenceStatus = E_BUSY;
    }
    else if (status == "offline")
    {
        presenceStatus = E_OFFLINE;
    }
    else if (status == "waiting")
    {
        presenceStatus = E_WAITING;
    }
    else
    {
        Q_ASSERT(false);
    }
    return presenceStatus;
}

EVISITORSTATUS UtilsHelper::GetVisitorStatusByString(const QString &status)
{
    EVISITORSTATUS ret = E_VSTATUS_DEFAULT;
    if (status == "1")
    {
        ret = E_VSTATUS_BROWSE_WEB;
    }
    else if (status == "2")
    {
        ret = E_VSTATUS_CONVERSATION;
    }
    else if (status == "3")
    {
        ret = E_VSTATUS_INVITE;
    }
    else if (status == "4")
    {
        ret = E_VSTATUS_LEAVE;
    }
    else if (status == "5")
    {
        ret = E_VSTATUS_QUEUE;
    }
    else
    {
        Q_ASSERT(false);
    }
    return ret;
}

QString UtilsHelper::GetMd5(const QString &val)
{
    QString    md5;
    QByteArray bb;
    bb  = QCryptographicHash::hash(val.toLatin1(), QCryptographicHash::Md5);
    md5 = QString(bb.toHex());
    return md5;
}

QString UtilsHelper::GetUUid()
{
    QString strId = QUuid::createUuid().toString();
    strId.remove("{").remove("}").remove("-");
    return strId;
}

bool UtilsHelper::IsPicUrl(const QString &strFileUrl)
{
    // QString strFileUrl = "http://pre.easyliao.cn:80/live/down2.jsp?ext=.jpg&file=yDWWlSCMGUtVyE9nm64I3MvEVZYTaFmEidmgq83fKaknpLPfuTgC6w%3D%3D";
    int pos = strFileUrl.indexOf("?");
    if (pos != -1)
    {
        QString     temp = strFileUrl.mid(pos + 1);
        QStringList list = temp.split("&");
        Q_FOREACH (QString item, list)
        {
            if (item.startsWith("ext="))
            {
                QString ext = item.mid(4).toLower();
                if (ext == ".png" || ext == ".jpeg" || ext == ".gif" || ext == ".jpg" || ext == ".bmp" || ext == ".ico" || ext == ".tiff")
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }
    return false;
}

bool UtilsHelper::IsMobilePhone(const QString &val)
{
    int               posDecode       = 0;
    QString           strSourceDecode = val;
    QRegExp           regxDecode("^0?(13|14|15|16|17|18|19)[0-9]{9}$");
    QRegExpValidator  regsDecode(regxDecode, 0);
    QValidator::State resDecode = regsDecode.validate(strSourceDecode, posDecode);

    if (QValidator::Acceptable == resDecode)
    {
        return true;
    }
    else
    {
        int               posEncode       = 0;
        QString           strSourceEncode = val;
        QRegExp           regxEncode("^0?(13|14|15|16|17|18|19)[0-9]\\*{4}[0-9]{4}$");
        QRegExpValidator  regsEncode(regxEncode, 0);
        QValidator::State resEncode = regsEncode.validate(strSourceEncode, posEncode);

        if (QValidator::Acceptable == resEncode)
        {
            return true;
        }
    }

    return false;
}

EPRESENCESTATUS UtilsHelper::GetPressenceStatus(QString status, QString runningStatus)
{
    EPRESENCESTATUS ret = E_OFFLINE;
    if (status == "1")
    {
        if (runningStatus == "1")
        {
            ret = E_ONLINE;
        }
        else if (runningStatus == "2")
        {
            ret = E_BUSY;
        }
        else if (runningStatus == "3")
        {
            ret = E_LEAVE;
        }
        else
        {
            Q_ASSERT(false);
        }
    }
    else if (status == "0")
    {
        ret = E_OFFLINE;
    }
    else
    {
        Q_ASSERT(false);
    }
    return ret;
}

QString UtilsHelper::GetTimeByMSecs(const QString &val)
{
    long long msecs = val.toLongLong();
    QDateTime time;
    time        = QDateTime::fromMSecsSinceEpoch(msecs);
    QString ret = time.toString("yyyy-MM-dd hh:mm:ss");
    return ret;
}

QString UtilsHelper::GetDateTimeStringByMSecs(const QString &val, const QString &format)
{
    long long msecs = val.toLongLong();
    QDateTime time;
    time        = time.fromMSecsSinceEpoch(msecs);
    QString ret = time.toString(format);
    return ret;
}

QString UtilsHelper::GetDataTimeDurationByMSecs(const QString &val)
{
    long long msecs = val.toLongLong();

    QDateTime startTime = QDateTime::fromMSecsSinceEpoch(msecs);
    QDateTime endTime   = QDateTime::currentDateTime();

    QTime m_time;
    m_time.setHMS(0, 0, 0);
    QString duration = m_time.addSecs(startTime.secsTo(endTime)).toString("hh:mm:ss");

    return duration;
}

void UtilsHelper::SetAutoStart(bool is_auto_start)
{
    QString    application_name = QCoreApplication::applicationName();
    QSettings *settings         = new QSettings(REG_RUN, QSettings::NativeFormat);
    if (is_auto_start)
    {
        QString application_path = QCoreApplication::applicationDirPath();
        application_path         = application_path.replace("/", "\\");
        application_path         = application_path.append("\\EasyLiao.exe");
        settings->setValue(application_name, application_path);
    }
    else
    {
        settings->remove(application_name);
    }
    delete settings;
}

QString UtilsHelper::LoadFile(const QString &fileName)
{
    QString ret = "";
    QFile   f(fileName);
    f.open(QFile::ReadOnly);
    if (f.isOpen())
    {
        ret = QString::fromUtf8(f.readAll());
        f.close();
    }
    return ret;
}

void UtilsHelper::SaveFile(const QString &fileName, const QString &dataInfo)
{
    QFile f(fileName);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    f.write(dataInfo.toUtf8());
    f.close();
}

QString UtilsHelper::GetRunAppDir()
{
    QString path =  QCoreApplication::applicationDirPath();

    return path;
}

QString UtilsHelper::GetAppdataCacheDir()
{
#ifdef _MSC_VER
    QString appdataPath = QProcessEnvironment::systemEnvironment().value("APPDATA");
#else
    QString appdataPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
#endif
    appdataPath.replace("\\", "/");
    QString pathDir = QString("%1/%2").arg(appdataPath).arg("EASYLIAOIM");
    QDir    dir;
    if (!dir.exists(pathDir))
    {
        dir.mkdir(pathDir);
    }
    return pathDir;
}

QString UtilsHelper::GetAppDataCustomSoundDir()
{
    QString customSoundDir = UtilsHelper::GetAppdataCacheDir() + "/CustomSound";

    QDir dir;
    if (!dir.exists(customSoundDir))
    {
        dir.mkdir(customSoundDir);
    }

    return customSoundDir;
}


QString UtilsHelper::GetAppDataDouyinDir()
{
    QString menuicoDir = UtilsHelper::GetAppdataCacheDir() + "/douyin";
    QDir    dir;
    if (!dir.exists(menuicoDir))
    {
        dir.mkdir(menuicoDir);
    }
    return menuicoDir;
}

QString UtilsHelper::GetAppDataUpdateDir()
{
    QString updateDir = UtilsHelper::GetAppdataCacheDir() + "/update";

    QDir dir;
    if (!dir.exists(updateDir))
    {
        dir.mkdir(updateDir);
    }

    return updateDir;
}

QString UtilsHelper::GetAppDataMenuicoDir()
{
    QString menuicoDir = UtilsHelper::GetAppdataCacheDir() + "/menuico";

    QDir dir;
    if (!dir.exists(menuicoDir))
    {
        dir.mkdir(menuicoDir);
    }

    return menuicoDir;
}

QString UtilsHelper::GetAppdataHttpCacheDir()
{
    
    QString pathDir = UtilsHelper::GetAppdataCacheDir();
    QDir    dir;
    if (!dir.exists(pathDir))
    {
        dir.mkdir(pathDir);
    }
    QString cacheDir = QString("%1/%2").arg(pathDir).arg("cache");
    if (!dir.exists(cacheDir))
    {
        dir.mkdir(cacheDir);
    }
    return cacheDir;
}



QString UtilsHelper::GetDumpDir()
{
    QString pathDir = QString("%1/%2/").arg(GetAppdataCacheDir()).arg("DUMP");
    QDir    dir;
    if (!dir.exists(pathDir))
    {
        dir.mkdir(pathDir);
    }
    return pathDir;
}

QString UtilsHelper::GetDumpBakDir()
{
    QString pathDir = QString("%1/%2/").arg(GetAppdataCacheDir()).arg("DUMPBAK");
    QDir    dir;
    if (!dir.exists(pathDir))
    {
        dir.mkdir(pathDir);
    }
    return pathDir;
}


QString UtilsHelper::GetCacheDir(QString userid)
{
    QString pathDir = QString("%1/%2").arg(GetAppdataCacheDir()).arg(userid);
    QDir    dir;
    if (!dir.exists(pathDir))
    {
        dir.mkdir(pathDir);
    }
    return pathDir;
}

QString UtilsHelper::GetHttpCacheDataDir(QString userid)
{
    QString pathDir = QString("%1/%2").arg(GetAppdataCacheDir()).arg(userid);
    QDir    dir;
    if (!dir.exists(pathDir))
    {
        dir.mkdir(pathDir);
    }
    QString cacheDir = QString("%1/%2").arg(pathDir).arg("cache");
    if (!dir.exists(cacheDir))
    {
        dir.mkdir(cacheDir);
    }
    return cacheDir;
}

QString UtilsHelper::GetScreenFileName(QString userId)
{
    QString pathDir = QString("%1/%2").arg(UtilsHelper::GetCacheDir(userId)).arg("screen");
    QDir    dir;
    if (!dir.exists(pathDir))
    {
        dir.mkdir(pathDir);
    }

    QString ret = QString("%1/screen_%2.png").arg(pathDir).arg(QDateTime::currentMSecsSinceEpoch());
    return ret;
}

QString UtilsHelper::GetExtendHtmlDir()
{
    QString ret = QString("%1/%2/").arg(UtilsHelper::GetRunAppDir()).arg("extend");
    return ret;
}

QString UtilsHelper::GetDefaultName(QString position, QString vid)
{
    QString ret;
    if (vid.isEmpty())
    {
        return ret;
    }
    if (vid.length() < 6)
    {
        ret = QString("%1%2").arg(position).arg(vid);
        return ret;
    }
    ushort n5 = vid.at(vid.length() - 5).unicode();
    ushort n4 = vid.at(vid.length() - 4).unicode();
    ushort n3 = vid.at(vid.length() - 3).unicode();
    ushort n2 = vid.at(vid.length() - 2).unicode();
    ushort n1 = vid.at(vid.length() - 1).unicode();

    QString format = "%1%2%3%4%5%6";
    ret            = format.arg(position).arg(n5).arg(n4).arg(n3).arg(n2).arg(n1);
    if (ret.length() > 12)
    {
        ret = ret.left(12);
    }
    return ret;
}


QString UtilsHelper::GetDefaultNameByDistrict(QString position, QString vid)
{
    QString ret;
    if (vid.isEmpty())
    {
        return ret;
    }
    if (vid.length() < 6)
    {
        ret = QString("%1%2").arg(position).arg(vid);
        return ret;
    }
    ushort n5 = vid.at(vid.length() - 5).unicode();
    ushort n4 = vid.at(vid.length() - 4).unicode();
    ushort n3 = vid.at(vid.length() - 3).unicode();
    ushort n2 = vid.at(vid.length() - 2).unicode();
    ushort n1 = vid.at(vid.length() - 1).unicode();

    QString format = "%1%2%3%4%5%6";
    ret            = format.arg(position).arg(n5).arg(n4).arg(n3).arg(n2).arg(n1);
    if (ret.length() > 14)
    {
        if (position.length() < 14)
            ret = ret.left(14);
        else
            ret = ret.left(position.length() + 4);
    }
    return ret;
}

int UtilsHelper::GetRandNumber(int maxNumber)
{
    int ret = qrand() % maxNumber;
    return ret;
}

QString UtilsHelper::GetWeixinByString(const QString &body,const QString pconfig)
{QString strRet  = "";
    QString strVal  = "[a-zA-Z]{1}[-_a-zA-Z0-9]{5,19}";
    if (!pconfig.isEmpty())
    {
        strVal = pconfig;
    }
    QRegExp rx(strVal);
    int     pos = 0;
    pos         = rx.indexIn(body, pos);
    if (pos != -1)
    {
        strRet = rx.cap(0);
    }

    return strRet;
}

QString UtilsHelper::GetQQByString(const QString &body)
{
       QString strRet = "";
       QString strVal = "[1-9][0-9]{4,14}";

       QRegExp rx(strVal);
       int     pos = 0;
       pos         = rx.indexIn(body, pos);
       if (pos != -1)
       {
           strRet = rx.cap(0);
       }

       return strRet;
}

QString UtilsHelper::GetMobileByString(const QString &body,const QString pconfig)
{
    QString strRet  = "";
    QString strVal  = "[1][3456789][0-9]{9}|1[3456789]\\d[-\\s]*\\d{4}[-\\s]*\\d{4}|1[3456789]\\d{2}[-\\s]*\\d{3}[-\\s]*\\d{4}|1[3456789]\\d[-\\s]*\\d{3}[-\\s]*\\d{5}";

    if (!pconfig.isEmpty())
    {
        strVal = pconfig;
    }
    QRegExp rx(strVal);
    int     pos = 0;
    pos         = rx.indexIn(body, pos);
    if (pos != -1)
    {
        strRet = rx.cap(0);
    }
    if (!strRet.isEmpty())
    {
        QString strNumber = GetNumberByString(body);
        if (strNumber.length() > 11)
        {
            strRet = "";
        }
    }

    return strRet;
}

QString UtilsHelper::GetTelephoneByString(const QString &body,const QString pconfig)
{
    QString strRet  = "";
    QString strVal  = "(\\(\\d{3,4}\\)|\\d{3,4}-|\\s)?\\d{8}";

    if (!pconfig.isEmpty())
    {
        strVal = pconfig;
    }
    QRegExp rx(strVal);
    int     pos = 0;
    pos         = rx.indexIn(body, pos);
    if (pos != -1)
    {
        strRet = rx.cap(0);
    }
    if (!strRet.isEmpty())
    {
        QString strNumber = GetNumberByString(body);
        if (strNumber.length() > 11)
        {
            strRet = "";
        }
    }
    return strRet;
}

QString UtilsHelper::GetTokenWZ(const QString &uid, const QString &userName, const QString &pwd)
{
    QString ret = "";
    //$uid,$timestamp,$password,$userName
    QString strFormat = "%1%2%3%4";
    qint64  time      = QDateTime::currentMSecsSinceEpoch() / 60000;
    QString valJoin   = strFormat.arg(uid).arg(time).arg(pwd).arg(userName);

    // qDebug()<< "joinString："<<valJoin;
    QByteArray          valByt = valJoin.toLatin1();
    QCryptographicHash *hash   = new QCryptographicHash(QCryptographicHash::Sha1);
    hash->addData(valByt);
    QByteArray sha1 = hash->result();

    QString val1 = sha1.toHex();

    // qDebug()<<"第一次sha1， 16进制字符串:"<<val1;

    QCryptographicHash *hash2 = new QCryptographicHash(QCryptographicHash::Sha1);
    hash2->addData(val1.toLatin1());
    QByteArray sha2 = hash2->result();

    QString val = sha2.toHex();
    // qDebug()<<"第二次sha1， 16进制字符串:"<<val;

    QString retformat = "%1,%2";

    ret = retformat.arg(time).arg(val);

    // qDebug()<<"第二次sha1， 返回的token结果:"<<ret;
    return ret;
}

QString UtilsHelper::GetToken(const QString &uid, const QString &userName, const QString &pwd)
{
    QString             ret       = "";
    QString             strFormat = "%1%2%3";
    qint64              time      = QDateTime::currentMSecsSinceEpoch() / 60000;
    QString             strSource = strFormat.arg(userName).arg(pwd).arg(time);
    QByteArray          string    = strSource.toLatin1();
    QCryptographicHash *hash      = new QCryptographicHash(QCryptographicHash::Sha1);
    hash->addData(string);
    QByteArray sha1      = hash->result();
    QString    retformat = "%1,%2,%3,%4";

    ret               = retformat.arg(uid).arg(userName).arg(time).arg(QString::fromUtf8(sha1));
    QString retBase64 = QString::fromUtf8(ret.toUtf8().toBase64());
    return retBase64;
}

quint32 UtilsHelper::NativeKeyCode(quint32 key)
{
#ifdef _MSC_VER
    switch (key)
    {
    case VK_ESCAPE:
        return Qt::Key_Escape;
    case VK_TAB:
        return Qt::Key_Tab;
    case VK_BACK:
        return Qt::Key_Backspace;
    case VK_RETURN:
        return Qt::Key_Return;
    case VK_INSERT:
        return Qt::Key_Insert;
    case VK_DELETE:
        return Qt::Key_Delete;
    case VK_PAUSE:
        return Qt::Key_Pause;
    case VK_PRINT:
        return Qt::Key_Print;
    case VK_CLEAR:
        return Qt::Key_Clear;
    case VK_HOME:
        return Qt::Key_Home;
    case VK_END:
        return Qt::Key_End;
    case VK_LEFT:
        return Qt::Key_Left;
    case VK_UP:
        return Qt::Key_Up;
    case VK_RIGHT:
        return Qt::Key_Right;
    case VK_DOWN:
        return Qt::Key_Down;
    case VK_PRIOR:
        return Qt::Key_PageUp;
    case VK_NEXT:
        return Qt::Key_PageDown;
    case VK_F1:
        return Qt::Key_F1;
    case VK_F2:
        return Qt::Key_F2;
    case VK_F3:
        return Qt::Key_F3;
    case VK_F4:
        return Qt::Key_F4;
    case VK_F5:
        return Qt::Key_F5;
    case VK_F6:
        return Qt::Key_F6;
    case VK_F7:
        return Qt::Key_F7;
    case VK_F8:
        return Qt::Key_F8;
    case VK_F9:
        return Qt::Key_F9;
    case VK_F10:
        return Qt::Key_F10;
    case VK_F11:
        return Qt::Key_F11;
    case VK_F12:
        return Qt::Key_F12;
    case VK_F13:
        return Qt::Key_F13;
    case VK_F14:
        return Qt::Key_F14;
    case VK_F15:
        return Qt::Key_F15;
    case VK_F16:
        return Qt::Key_F16;
    case VK_F17:
        return Qt::Key_F17;
    case VK_F18:
        return Qt::Key_F18;
    case VK_F19:
        return Qt::Key_F19;
    case VK_F20:
        return Qt::Key_F20;
    case VK_F21:
        return Qt::Key_F21;

    case VK_F22:
        return Qt::Key_F22;
    case VK_F23:
        return Qt::Key_F23;
    case VK_F24:
        return Qt::Key_F24;

    case VK_SPACE:
        return Qt::Key_Space;

    case VK_MULTIPLY:
        return Qt::Key_Asterisk;

    case VK_ADD:
        return Qt::Key_Plus;

    case VK_SEPARATOR:
        return Qt::Key_Comma;

    case VK_SUBTRACT:
        return Qt::Key_Minus;

    case VK_DIVIDE:
        return Qt::Key_Slash;
    default:
        return key;
    }
#else
    return 0;
#endif    
}

quint32 UtilsHelper::NativeModifiers(quint32 modifiers)
{

    quint32 key = 0;
    //todo 9999
#ifdef _MSC_VER    
    if (modifiers & MOD_ALT)
    {
        key |= Qt::AltModifier;
    }
    if (modifiers & MOD_SHIFT)
    {
        key |= Qt::ShiftModifier;
    }
    if (modifiers & MOD_CONTROL)
    {
        key |= Qt::ControlModifier;
    }
    if (modifiers & MOD_WIN)
    {
        key |= Qt::MetaModifier;
    }
#endif    
    return key;
}

unsigned int UtilsHelper::GetModifyKeyByHotKey(QString strHotKey)
{
    QStringList hotKeyList = strHotKey.split("+");
    UINT        modifys    = 0;
    //todo 9999
#ifdef _MSC_VER    
    if (hotKeyList.indexOf("CTRL") != -1)
    {
        modifys = modifys | MOD_CONTROL;
    }
    if (hotKeyList.indexOf("ALT") != -1)
    {
        modifys = modifys | MOD_ALT;
    }

    if (hotKeyList.indexOf("SHIFT") != -1)
    {
        modifys = modifys | MOD_SHIFT;
    }
#endif    
    return modifys;
}

unsigned int UtilsHelper::GetKeyByShotKey(QString strHotKey)
{
    QString key = strHotKey.mid(strHotKey.lastIndexOf("+") + 1).trimmed();
    auto    val = key.toUcs4();
    UINT    ret = 0;
    if (val.size() > 0)
    {
        ret = val[0];
    }
    return ret;
}

void UtilsHelper::Execute(QString body)
{
    body           = body.toUtf8().toBase64();
    QString result = ConstStringHelper::ProtocolFormat.arg(ConstStringHelper::EasyLiao).arg(ConstStringHelper::RouteMonitor).arg(body);
    STDPRINTF(result.toUtf8().data());
}

void UtilsHelper::Execute(QString receiver, QString body)
{
    body           = body.toUtf8().toBase64();
    QString result = ConstStringHelper::ProtocolFormat.arg(ConstStringHelper::EasyLiao).arg(receiver).arg(body);
    STDPRINTF(result.toUtf8().data());
}

QString UtilsHelper::GetCustomerHeadIcon(EPRESENCESTATUS state)
{
    QString ret = "";
    if (state == E_OFFLINE || state == E_NONE)
    {
        ret = ":/YL/Resources/YL/MainTree/customer_offline.png";
    }
    else
    {
        ret = ":/YL/Resources/YL/MainTree/customer_online.png";
    }
    return ret;
}

QString UtilsHelper::GetCustomerStateIcon(EPRESENCESTATUS eType)
{
    QString ret = "";
    switch (eType)
    {
    case E_ONLINE:
        ret = ":/YL/Resources/YL/MainTree/online_status.png";
        break;
    case E_BUSY:
        ret = ":/YL/Resources/YL/MainTree/busy_status.png";
        break;
    case E_LEAVE:
        ret = ":/YL/Resources/YL/MainTree/leave_status.png";
        break;
    case E_OFFLINE:
    default:
        ret = "";
        break;
    }
    return ret;
}

QString UtilsHelper::GetCustomerStateString(EPRESENCESTATUS eType)
{
    QString valStatus = "";
    switch (eType)
    {
    case E_BUSY:
        valStatus = tr("忙碌");
        break;
    case E_LEAVE:
        valStatus = tr("离开");
        break;
    case E_ONLINE:
        valStatus = tr("在线");
        break;
    case E_OFFLINE:
    default:
        valStatus = tr("离线");
        break;
    }
    return valStatus;
}

QString UtilsHelper::EncodeUrlByString(const QString &valUrl)
{
    QString ret = QUrl::toPercentEncoding(valUrl, "", "?&");
    return ret;
}

QDateTime UtilsHelper::GetDateTimeByMSecs(const QString &val)
{
    long long msecs = val.toLongLong();
    QDateTime time;
    time = QDateTime::fromMSecsSinceEpoch(msecs);

    return time;
}

QString UtilsHelper::GetFullPathByRunDir(const QString &fileName)
{
    return QString("%1/%2").arg(GetRunAppDir()).arg(fileName);
}

QString UtilsHelper::ReplaceHtmlTag(QString line)
{
    QString ret = "";
    ret         = line.replace(QRegExp("<(\"[^\"]*\"|'[^']*'|[^'\">])*>"), "  ");
    ret.trimmed();
    return ret;
}

QString UtilsHelper::GetNumberByString(const QString &body)
{
    QString strRet = "";
    QString strVal = "\\d{5}\\d*";
    QRegExp rx(strVal);
    int     pos = 0;
    pos         = rx.indexIn(body, pos);
    if (pos != -1)
    {
        strRet = rx.cap(0);
    }

    return strRet;
}

bool UtilsHelper::IsRobot(int robotFlag, QString strUserId)
{
    //机器人标识为1，或者用户名是AI开头。
    bool isRet = false;
    do
    {
        if (robotFlag == 1)
        {
            isRet = true;
            break;
        }
        if (strUserId.toUpper().startsWith("AI"))
        {
            isRet = true;
            break;
        }
    } while (false);

    return isRet;
}

QPixmap UtilsHelper::ChangeQPixmapDPI(QPixmap pix, QString pixname, QSize size)
{
    QPixmap repix;
    if (!pixname.isEmpty() && pixname.endsWith(".svg"))
    {
        // tp = QIcon(pixname).pixmap(tempOption.decorationSize);
        // 输出的img一定要是拉伸dpi倍数之后尺寸
        //        BaseMainWnd *wnd = UIMainController::GetInstance()->BaseUIMainWnd();
        //        qreal xDPI = wnd->logicalDpiX() / 96;
        qreal        xDPI = 3.5; //拉伸比例暂时固定,后续优化为上方比例模式,
        QSvgRenderer svg_remder(pixname);
        QSize        label_size = size; //取控件的大小即可
        QImage       image(QSize(label_size.width() * xDPI, label_size.height() * xDPI), QImage::Format_ARGB32_Premultiplied);
        image.fill(Qt::transparent);

        QPainter painter(&image);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
        svg_remder.render(&painter);
        repix = repix.fromImage(image);
        repix.setDevicePixelRatio(xDPI); //设置pixmap伸缩比例
    }
    else
    {
        repix = pix;
    }
    return repix;
}


QString UtilsHelper::URLtostr(QString url)
{
    QByteArray urlDecode = QByteArray::fromPercentEncoding(url.toLocal8Bit());
    QString    re        = QString::fromLocal8Bit(urlDecode);
    return re;
}


QPixmap UtilsHelper::PixmapToRound(const QPixmap &src, int radius, QString colorstr)
{
    if (src.isNull())
    {
        return QPixmap();
    }
    qreal xDPI = 3.5; //拉伸比例暂时固定,后续优化为上方比例模式,
    radius     = radius * xDPI;
    QPixmap pixmapa;
    QImage  pixmap(QSize(radius, radius), QImage::Format_ARGB32_Premultiplied);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath path;
    path.addEllipse(0, 0, radius, radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, radius, radius, src);
    //加圈
    if (!colorstr.isEmpty())
    {
        painter.setPen(QPen(QColor(colorstr), 2 * xDPI));
        painter.setBrush(Qt::transparent);
        painter.drawEllipse(0, 0, radius, radius);
    }
    pixmapa = pixmapa.fromImage(pixmap);
    pixmapa.setDevicePixelRatio(xDPI); //设置pixmap伸缩比例
    //加小图标
    return pixmapa;
}


QString UtilsHelper::converRGB16HexStr(QColor _color)
{
    // r的rgb转化为16进制
    QString redStr = QString("%1").arg(_color.red(), 2, 16, QChar('0'));
    // g的rgb转化为16进制
    QString greenStr = QString("%1").arg(_color.green(), 2, 16, QChar('0'));
    // b的rgb转化为16进制
    QString blueStr = QString("%1").arg(_color.blue(), 2, 16, QChar('0'));
    //将各rgb的拼接在一起形成#000000
    QString hexStr = "#" + redStr + greenStr + blueStr;
    //返回hexStr
    return hexStr;
}


QString UtilsHelper::jsontostr(const QVariantMap &remsgMap)
{
    QJsonObject jsonObject = QJsonObject::fromVariantMap(remsgMap);
    return jsontostr(jsonObject);
}

QString UtilsHelper::jsontostr(const QJsonObject &remsgJso)
{
    QJsonDocument jsondoc;
    jsondoc.setObject(remsgJso);
    if (!jsondoc.isObject())
    {
        // json 生成失败
    }
    QByteArray json     = jsondoc.toJson(QJsonDocument::Compact);
    QString    json_str = QString::fromUtf8(json.data());
    return json_str;
}

QVariantMap UtilsHelper::strtojson(QString json)
{
    QVariant        var;
    QJsonParseError json_error;
    QJsonDocument   parse_doucment = QJsonDocument::fromJson(json.toUtf8(), &json_error);
    if (json_error.error != QJsonParseError::NoError)
    {
        // json 解析失败
        qDebug() << json_error.errorString();
    }
    var = parse_doucment.toVariant();
    return var.toMap();
}


int UtilsHelper::ptconvertpx(int pt, QWidget *ui)
{ //不启用高分屏时 字体pt改为px,防止跟随比例因子变化字体
    double dScaleFactor = qgetenv("QT_SCALE_FACTOR").toDouble();
    if (dScaleFactor > 1.0)
    {
        return pt;
    }
    int index = QApplication::desktop()->screenNumber(ui);
    if (index < 0)
    {
        index = 0;
    }
    QScreen *sc  = QGuiApplication::screens()[index];
    qreal    dpi = sc->logicalDotsPerInch();
    return (int)(pt * 96.0 / dpi);
}


QString UtilsHelper::replaceTemplate(const QString &jsonString, const QJsonObject &values) {
    QString result = jsonString;

    // 正则表达式匹配模板占位符
    QRegularExpression regex("\\{\\{(\\w+)\\}\\}");
    QRegularExpressionMatchIterator matchIterator = regex.globalMatch(jsonString);

    // 逐个替换模板占位符
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        if (match.hasMatch()) {
            QString placeholder = match.captured(1);
            if (values.contains(placeholder)) {
                // 替换占位符为实际值
                QString repval ;
                QJsonValue jsonplaceholder = values[placeholder];
                qDebug()<<jsonplaceholder.type()<<endl;
                switch (jsonplaceholder.type()) {
                case QJsonValue::Type::String:
                    repval = jsonplaceholder.toString();
                    repval.replace("\\","\\\\").replace("\"","\\\"").replace("\b","\\b").replace("\f","\\f").replace("\n","\\n").replace("\r","\\r").replace("\t","\\t");
                    repval = "\""+repval+"\"";
                    break;
                case QJsonValue::Type::Array:
                {
                    const QJsonArray array1 = jsonplaceholder.toArray();

                    QJsonDocument jsonDocument(array1);
                    repval = QString::fromUtf8(jsonDocument.toJson());
                    qDebug().noquote()<<repval;
                }

#if 0
                    QVariant temparray = jsonplaceholder.toVariant();
                    QList<QVariant> vlist = temparray.toList();
                    repval +="[";
                    for(QVariant var: vlist )
                    {

                            switch( var.type() )
                            {
                            case QVariant::Type::String:
                                if( repval.length() == 2  )
                                {


                                }

                                break;

                            }
                            if( var.type() == QVariant::Type::String)
                            repval += var.toString();


                    }
                    repval +"]";
#endif
                    break;
                default:
                    repval = QString("%1").arg(jsonplaceholder.toInt());

                }




                result.replace("{{"+placeholder+"}}", repval);
            } else {
                // 如果找不到对应的值，则保留占位符
                qDebug() << "Value not found for placeholder:" << placeholder;
            }
        }
    }

    return result;
}

QString UtilsHelper::GetDownLoadFileDir()
{

    return "";
}


#ifndef _MSC_VER
#include <time.h>
unsigned long GetTickCount()
{
    struct timespec ts; 
    clock_gettime(CLOCK_MONOTONIC,&ts); 
    return(ts.tv_sec*1000+ts.tv_nsec/1000000);
}
#endif
