#ifndef DbUiPersonalConfigInterface_H
#define DbUiPersonalConfigInterface_H


#include "datafactory/ConstructionData.h"
#include "utils/Plat_EnumDefine.h"

#include "data/CommInfo.h"

#include "data/GlobalConfig.h"
#include "data/LicenceSetting.h"
#include "data/PermissionsSetting.h"
#include "plat_interface_global.h"
//#include "data/UIAutoReplayInfo.h"
#include "data/UILoginData.h"
//#include "data/UISystemTipData.h"
//#include "data/AutoReplaySetting.h"

#include <QList>
#include <QMap>
#include <QMutex>
#include <QObject>
#include "service/DbUiBaseService.h"
#include <QObject>
#include <QStringList>
#include <QFont>
#include <include/json/value.h>
#include <data/FileUploadFilterSetting.h>
//#include <data/UIRobotChangTimeInfo.h>

class PLAT_INTERFACE_EXPORT DbPersonalConfigSign : public QObject
{
      Q_OBJECT
public:
    DbPersonalConfigSign()
    {
    }
    ~DbPersonalConfigSign()
    {
    }

public:
      void ChatSettingChanged()
    {
          Q_EMIT S_ChatSettingChanged();
    }
      void OtherSettingChanged()
    {
          Q_EMIT S_OtherSettingChanged();
    }


      void MessageExtTipSettingChanged()
    {
          Q_EMIT S_MessageExtTipSettingChanged();
    }

      void MonitorChatFilterSettingChanged()
    {
          Q_EMIT S_MonitorChatFilterSettingChanged();
    }
      void MonitorVisitorFilterSettingChanged()
    {
          Q_EMIT S_MonitorVisitorFilterSettingChanged();
    }
      void MonitorCustomerFilterSettingChanged()
    {
          Q_EMIT S_MonitorCustomerFilterSettingChanged();
    }

      void GetIconFinished()
    {
          Q_EMIT S_GetIconFinshed();
    }
      void GetTopMenuFinished()
    {
          Q_EMIT S_GetTopMenuFinshed();
    }

      void GetMenuListFinshed()
    {
          Q_EMIT S_GetMenuListFinshed();
    }

      void MessageTipSettingChanged()
      {
          Q_EMIT S_MessageTipSettingChanged();
      }
signals:
      void S_OtherSettingChanged();
      void S_ChatSettingChanged();
      void S_MonitorChatFilterSettingChanged();
      void S_MonitorVisitorFilterSettingChanged();
      void S_MonitorCustomerFilterSettingChanged();

      void S_MessageExtTipSettingChanged();
      void S_GetIconFinshed();
      void S_GetTopMenuFinshed();
      void S_GetMenuListFinshed();
      void S_MessageTipSettingChanged();
};

class IBaseProtocolAgent;
class UIExtSystemTipData;



class PLAT_INTERFACE_EXPORT DbUiPersonalConfigInterface : public DbUiBaseService
{
    friend class DbUiWebcallService;
    friend class DbUiWebcallServer;

public:
    DbUiPersonalConfigInterface(DbUiService *mainService){ };
    DbUiPersonalConfigInterface(){ };

public:
    ~DbUiPersonalConfigInterface(){ };

private:
public:
    DbPersonalConfigSign *PersonalConfigSign()
    {
        return &m_sign;
    }

    virtual bool IsShieldAiboard(){return false;};

    virtual QString      ServiceId( ){return "DbUiPersonalConfigInterface";};
     virtual void        ProtocolAgent(IBaseProtocolAgent *agent){ };
     virtual void        InitPersonalConfig(){ };
     virtual void        LoadOtherSetting(){ };
     virtual void        SaveOtherSetting(){ };
    virtual ECLIENTTYPE Clienttype(){ return E_CLIENT_NONE; };
    virtual void    HtmlVersion(QString val){};
    virtual    QString HtmlVersion() const{return "";};

    virtual    void    HtmlEnv(QString val){};
    virtual    QString HtmlEnv() const {return "";};

     virtual QString GetOtherConfigByKey(const QString &key, const QString &defaultString){return ""; };
     virtual QString CurrentUserId(){ return ""; };
     virtual void    CurrentUserId(QString val){ };

     virtual QFont GetUserFontStyle(){ return QFont();};
     virtual void  SetUserFontStyle(QFont val){ };

     virtual QFont GetVisitorFontStyle(){return QFont(); };
     virtual void  SetVisitorFontStyle(QFont val){ };

     virtual void LoadVisitorFontStyle(){ };
     virtual void LoadUserFontStyle(){ };

     virtual void SaveUserFontStyle(){ };
     virtual void SaveVisitorFontStyle(){ };

     virtual void    SaveSettingToServer(){ };
     virtual void    SetVisitorColor(QString val){ };
     virtual void    SetUserColor(QString val){ };
     virtual QString GetUserColor(){return ""; };
     virtual QString GetVisitorColor(){return ""; };

     virtual void SetFunMenuList(const QString &body){ };
     virtual int  GetFunMenuList(const QString &datatype, ConstructionData *factory, void *outlist){ return 0;};




     virtual int CustomExtSystemTipList(const QString &datatype, ConstructionData *factory, void *outlist){return 0; };

     virtual void CustomExtSystemTipList(QList<UIExtSystemTipData *> list){ };

     virtual void ClearExtCustomList(){ };

     virtual void SaveCustomExtSystemTipSetting(){ };


     //virtual void SetAutoReplaySetting(AutoReplaySetting config){ };




    virtual CommInfo *GetUserOtherSetting(){ return nullptr;};
     virtual void      SetUserOtherSetting(const CommInfo &val){ };

    virtual CommInfo *GetChatSetting(){ return nullptr;};
     virtual void      SetChatSetting(const CommInfo &val){ };

    virtual CommInfo *GetMonitorVisitorFilterSetting(){ return nullptr; };
     virtual void      SetMonitorVisitorFilterSetting(const CommInfo &val){ };

    virtual CommInfo *GetMonitorCustomerFilterSetting(){ return nullptr;};
     virtual void      SetMonitorCustomerFilterSetting(const CommInfo &val){ };

    virtual CommInfo *GetMonitorChatFilterSetting(){ return nullptr;};
     virtual void      SetMonitorChatFilterSetting(const CommInfo &val){ };

     virtual void *GetIconInfoByKey(QString key, const QString &type, ConstructionData *factory){ return nullptr;};

     virtual void SetIcon2String(const QString &body){ };

     virtual QStringList GetTopMenuKeys(){ return QStringList(); };
     virtual void       *GetTopMenuInfoByKey(QString key, const QString &type, ConstructionData *factory){ return nullptr;};
     virtual void        SetTopMenuString(const QString &body){ };



    virtual ESUMMARIZEONCLOSE SummarizeStatus(){ return E_NO_FORCE;};
     virtual void              SummarizeStatus(ESUMMARIZEONCLOSE val){ };

    virtual CommInfo *GetCompanyInfoSetting(){return nullptr; };

    virtual CommInfo *GetCompanyInfoStatistics(){return nullptr; };

     virtual void SetCompanyInfoSetting(const CommInfo &info){ };
     virtual void SetCompanyInfoStatistics(const CommInfo &info){ };

     virtual void SetExtString(QString val){ };

     virtual QString            GetExtNameByKey(QString key){ return "";};
    //virtual AutoReplaySetting *GetAutoReplaySetting(){ return  nullptr;};

     virtual void *GetSystemTipByKey(const QString &type, const QString &typeData, ConstructionData *factory){ return nullptr;};


     virtual void    AddExtendMap(QString strKey, QString strVal){ };
     virtual QString GetExtendInitDataByKey(const QString &key){ return "";};




   virtual bool DenyBcpSysMsg(){ return false;};
     virtual void DenyBcpSysMsg(bool val){ };

     virtual void    MobileHidden(QString val){ };
     virtual QString MobileHidden(){ return "";};

   virtual bool IsHiddenMobile(QString userId){return false; }; //新接口处理手机隐藏

     virtual void    MobileHiddenPos(QString val){ };
     virtual QString MobileHiddenPos(){return ""; };

     virtual void    MobileHiddenTime(QString val){ };
     virtual QString MobileHiddenTime(){ return "";};

     virtual void    ChatWatermark(QString val){ };
     virtual QString ChatWatermark(){ return "";};

    virtual int  JoinTagTime(){return 120;};
    virtual void JoinTagTime(int timeval){};

    virtual int  ReadReceipt(){return 1;};  //需求10638 客户端消息增加已读未读开关，为1表示开启，为0表示不开启
    virtual void ReadReceipt(int val){};

    virtual QString  VisitorNameDisplay(){return "";};  //需求10626 访客名称显示内容，默认为空字符串, 分组方式为"group"
    virtual void VisitorNameDisplay(QString val){};

    virtual void    FileUploadURL(QString val){ };
    virtual QString FileUploadURL(){return "";};

    virtual void    ExpressionUploadUrl(QString val){ };
    virtual QString ExpressionUploadUrl(){return "";};

    virtual void    CompanyMaterialUrl(QString val){ };
    virtual QString CompanyMaterialUrl(){return "";};

     virtual void ChatLiveTime(int val){ };
     virtual int  ChatLiveTime(){return 0; };

     virtual void    CardInfoUrl(QString val){ };
     virtual QString CardInfoUrl(){return ""; };

     virtual void    CompanyCommonWordUrl(QString val){ };
     virtual QString CompanyCommonWordUrl(){return ""; };

     virtual void MobileCheck(int val){ };
     virtual int  MobileCheck(){ return 0;};
   virtual  bool IsAdaptResult() const
    {
        return false;//m_service->IsAdaptResult(){ };
    }
     virtual void IsAdaptResult(bool val)
    {
        return ;//m_service->IsAdaptResult(val){ };
    }

     virtual void DoGetSettingFinished(QString code, QString key, QString body){ };

     virtual QString MainLocalVersion(){return ""; };

     virtual void LoadLocdatasetting(){ };
     virtual void SaveLocdatasetting(){ };

    virtual Json::Value GetcommonwordTemp() { return Json::Value();};
     virtual void    SetLocDataByKey(const QString &key, const QString &valString){ };
    virtual QString GetLocDataConfigByKey(const QString &key, const QString &defaultString){return "";};

    virtual FileUploadFilterSetting *GetFileUploadFilterSetting() const{return nullptr;};
    virtual void                     SetFileUploadFilterSetting(FileUploadFilterSetting config){};
	virtual void SetPermissionsSetting(PermissionsSetting * config) {};

    virtual int  MaxVisitorChatCount(){return 0;};
    virtual void MaxVisitorChatCount(int count) {return ;};

    //virtual UIRobotChangTimeInfo *GetRobotChangTimeInfo(){return nullptr;};
    virtual void                  SetRobotChangTimeInfo(const QString &body){return ;};

    virtual void    AddAutoMatchingRegular(QString strKey, QString strVal){};
    virtual QString GetAutoMatchingRegular(const QString &key){return "";};

//todo 需求5332/10855 敏感词屏蔽
    /**
     * @brief GetSensitiveWordList  获取敏感词列表
     * @return 返回字符串數組
     */
    virtual QStringList GetSensitiveWordList(){return QStringList();} ;
    /**
     * @brief SetSensitiveWordList  设置敏感词
     * @param QStringList  字符串數組
     */
    virtual void                  SetSensitiveWordList(const QString QStringList){return ;};

 protected:

    DbPersonalConfigSign           m_sign;
};

#endif // DbUiPersonalConfigInterface_H
