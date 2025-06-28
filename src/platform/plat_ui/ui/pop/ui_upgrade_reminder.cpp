#include "stdafx.h"
#include "ui_upgrade_reminder.h"
#include "ui_ui_upgrade_reminder.h"

#include "utils/utilshelper.h"
#include "uimessagebox.h"


UI_Upgrade_Reminder::UI_Upgrade_Reminder(QWidget *parent) : QWidget(parent), ui(new Ui::UI_Upgrade_Reminder)
{
    ui->setupUi(this);
    setVersion();
}

UI_Upgrade_Reminder::~UI_Upgrade_Reminder()
{
    delete ui;
}

void UI_Upgrade_Reminder::setVersion()
{
    UINewVersionInfo *version = gUISystemConfigManager->NewVersion();
    if (version)
    {
        ui->versionLabel->setText(tr("发现新版本V ") + version->Version());
        ui->explain->setHtml(version->Update_description()); //更新说明 替换clientApi时有用
    }
}

void UI_Upgrade_Reminder::on_later_clicked()//关闭
{
    Q_EMIT S_Closed();
}

#ifdef _MSC_VER
#include <windows.h>

// 若tlhelp32.h 在 windows.h 之前添加 CreateToolhelp32Snapshot函数会报错
#include <Psapi.h>
#include <string>
#include <tchar.h>
#include <tlhelp32.h>


#include <qtextcodec.h>
#include <shellapi.h>
#endif
#include <iostream>
using namespace std;
#include <fstream>

QString UI_Upgrade_Reminder_GetProcessPathByPId(DWORD dwProcessId)
{

    QString cstrPath;
#if _MSC_VER       
    HANDLE  hProcess = NULL;
    HMODULE hModule;
    DWORD   need;
    char    szPath[MAX_PATH + 1] = {0};

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
    if (hProcess)
    {
        EnumProcessModules(hProcess, &hModule, sizeof(hModule), &need);
        GetModuleFileNameExA(hProcess, hModule, (LPSTR)szPath, sizeof(szPath));
        cstrPath = QString::fromLatin1(szPath);

        QByteArray  encodedString = cstrPath.toLatin1();
        QTextCodec *codec         = QTextCodec::codecForName("GB2312"); //支持压缩文件中包含中文目录or文件
        cstrPath                  = codec->toUnicode(encodedString);
    }
    cstrPath.replace(QString("\\"), QString("/"));
#endif    
    return cstrPath;
}

void UI_Upgrade_Reminder::on_restart_Upgrade_clicked()//重启升级
{
#ifdef _MSC_VER    
    int runCount = 0;//当前目录运行实例数
    QString processname = QCoreApplication::applicationDirPath() + "/EasyLiaoClient.exe";
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnapshot, &pe))
    {
        while (Process32Next(hSnapshot, &pe))
        {
            QString str1       = UI_Upgrade_Reminder_GetProcessPathByPId(pe.th32ProcessID);
            if (str1.toLower() == processname.toLower())
            {
                runCount++;
            }
        }
    }
    CloseHandle(hSnapshot);
    if(runCount/3 > 1)//当运行实例大于1时,出提示信息
    {
        UIMessageBox *box = new UIMessageBox();
        box->SetContentTip(tr("您当前电脑登录多个账号，升级会自动将所有账号退出"));
        box->SetCancelBtnVisible(true);
        if(box->exec() != 1)//没有点击确定,则取消重启升级
        {
            return;
        }
    }
//todo 待处理
    //UtilsViewHelper::Restart_UP();
#endif    
}
