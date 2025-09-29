#ifndef EXCELHELPER_H
#define EXCELHELPER_H

#include <QMap>
#include <QObject>

#include "BasicExcelVC6.hpp"
#include "include/json/value.h"
using namespace YExcelVC6;

class ExcelHelper : public QObject
{
    Q_OBJECT

public:
    ExcelHelper();
    ~ExcelHelper();

    static QList<QMap<int, QString>> ReadExcelFile(QString fileName);
    static bool                      WriteExcelFile(QString fileName);

private:
    static QString ReplaceHtmlTag(QString line, QString replaceStr);
    static QString GetCellContentToString(BasicExcelCell* cell);
};

#endif // EXCELHELPER_H
