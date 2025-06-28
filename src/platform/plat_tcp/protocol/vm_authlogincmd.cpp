#include "VM_AuthLoginCmd.h"
#include "utils/utilshelper.h"

VM_AuthLoginCmd::VM_AuthLoginCmd() : VisitorMonitorTcpCmd()
{
}

VM_AuthLoginCmd::~VM_AuthLoginCmd()
{
}

void VM_AuthLoginCmd::SetKey(const QString& key)
{
    m_key = key;
}

void VM_AuthLoginCmd::MakeCmd()
{
    // token为md5加密码(公司ID+用户名+上一步获取的key+固定值(!$JESONG^!@))得到的字符串， 如StringUtils.md5("1"+"xieyulin"+key+"!$JESONG^!@")
    // R /auth?u=xieyulin&token=c513749ab3a30a2b49a0a685dcda5d32&c=1 0 1458200828660

    QString strFormat = "/auth?u=%1&token=%2&c=%3";
    QString md5Format = "%1%2%3!$JESONG^!@";
    QString oldStr    = md5Format.arg(CompanyId()).arg(UserId()).arg(m_key);
    QString md5       = UtilsHelper::GetMd5(oldStr);
    QString ret       = strFormat.arg(UserId()).arg(md5).arg(CompanyId());
    Uri(ret);
}
