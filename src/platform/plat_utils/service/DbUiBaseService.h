#pragma once
#include <QString>

class DbUiService;

/************************************************************************/
/*数据转换函数定义
数据类型:  datatype  如： "list" 返加 QList结构数据
单条数据:  itemList  如： UIChatInfo * 数据实体对象
参数1:      param1    如:  ""
参数2:      param2    如:  QString *
输出结果集：  outlist  void *, 这个由用户可以自己定义,如QList  QMap 等集合用结构

*/
/************************************************************************/
typedef void (*pFunDataChange)(const QString &datatype, void *itemList, QString &param1, QString &param2, void *outlist);

class DbUiBaseService
{
public:
    DbUiBaseService(void);
    ~DbUiBaseService(void);
    virtual QString      ServiceId();
    virtual void         MainDbService(DbUiService *servcie);
    virtual DbUiService *MainDbService();
    int                  GetListData(QString datatype, void *outlist, pFunDataChange fun);

protected:
    DbUiService *m_maindb;
};
