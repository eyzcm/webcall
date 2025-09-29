#pragma once
#include <QObject>
#include <QString>

class BaseConstructionData
{
public:
    BaseConstructionData(void);
    ~BaseConstructionData(void);
    /************************************************************************
     * CreateObj 构制新的对象函数,根据原来数据层数据类构建新的数据对象
     * datatype 数据类型  ,"chat",CommonWord"
     * itemobj  原来数据对象指针  ,如：UIChatInfo * 指针
     * 返回    新的对象指针         新的对象类型可以跟原来对象一样，但创建内存是由请求类创建的，释放也请求类释放
     ************************************************************************/
    virtual void *CreateObj(const QString &datatype, void *itemobj);

    /************************************************************************
     * 将构建后的对象加到数据集里
     * datatype 数据类型  ,"chat",CommonWord"
     * itemobj   新的对象
     * outlist   数据值对象，可以是QList, QMap等
     *
     ************************************************************************/
    virtual int AddObjToList(const QString &datatype, void *itemobj, void *outlist);

    /************************************************************************
     * 释放内存
     ************************************************************************/
    virtual int FreeObj(const QString &datatype, void *itemobj);

protected:
    bool IsIndexOfString(QString sourceStr, QString indexStr);
};
