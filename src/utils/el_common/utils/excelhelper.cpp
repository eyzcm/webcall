#include "excelhelper.h"

#include "DefineHelper.h"
#include <QDebug>
#include <QFileInfo>
#include <QMap>
#include <QRegExp>

#include "BasicExcelVC6.hpp"

ExcelHelper::ExcelHelper() : QObject()
{
}

ExcelHelper::~ExcelHelper()
{
}

QList<QMap<int, QString>> ExcelHelper::ReadExcelFile(QString fileName)
{
    QList<QMap<int, QString>> shapeMapList;
    QFileInfo                 info(fileName);
    QByteArray                fileNameu   = QFile::encodeName(info.filePath());
    const char*               encodedName = fileNameu.constData(); // Valid as long as fileName exists

    BasicExcel e;
    if (!e.Load(encodedName))
        return shapeMapList;

    BasicExcelWorksheet* sheet = e.GetWorksheet(size_t(0));
    if (!sheet)
        return shapeMapList;

    int rowTotal = (int)sheet->GetTotalRows();
    int colTotal = (int)sheet->GetTotalCols();

    if (rowTotal <= 0 || colTotal <= 0)
        return shapeMapList;

    // QVector<QString> vecData;
    //  创建 MapList
    //  坐标对应shape
    for (int k = 0; k < rowTotal; k++)
    {
        QMap<int, QString> mmshapeHash;
        shapeMapList.append(mmshapeHash); //.insert(j, mmshapeHash);
    }

    BasicExcelCell* cell;
    for (int i = 0; i < rowTotal; i++)
    {
        for (int j = 0; j < colTotal; j++)
        {
            cell = sheet->Cell(i, j);

            QString cellStr = GetCellContentToString(cell);
            if (!cellStr.isEmpty())
            {
                QString      val = ReplaceHtmlTag(cellStr, "");
                shapeMapList.operator[](i).insert(j, val);
            }
        }
    }
    return shapeMapList;
}

QString ExcelHelper::GetCellContentToString(BasicExcelCell* cell)
{
    QString cellStr;
    switch (cell->Type())
    {
        case BasicExcelCell::INT:
            cellStr = QString::number(cell->GetInteger(), 10);
            break;
        case BasicExcelCell::DOUBLE:
            cellStr = QString::number(cell->GetDouble(), 'g', 6);
            break;
        case BasicExcelCell::STRING:
            cellStr = QString::fromUtf8(cell->GetString());
            break;
        case BasicExcelCell::WSTRING:
            cellStr = QString::fromWCharArray(cell->GetWString());
            break;
        case BasicExcelCell::UNDEFINED:
            cellStr = QString::fromRawData((const QChar*)cell->GetString(), cell->GetStringLength());
            break;
    }
    return cellStr;
}

QString ExcelHelper::ReplaceHtmlTag(QString line, QString replaceStr)
{
    QString ret = "";
    if (!line.isEmpty())
    {
        ret = line.replace(QRegExp("<(\"[^\"]*\"|'[^']*'|[^'\">])*>"), replaceStr);
        ret = ret.replace("&nbsp;", " ");
        ret = ret.trimmed();
    }
    return ret;
}

bool ExcelHelper::WriteExcelFile(QString fileName)
{
    BasicExcel e;
    e.New(1);

    BasicExcelWorksheet* sheet = e.AddWorksheet(reinterpret_cast<const wchar_t*>(fileName.utf16()), -1);
    sheet                      = e.GetWorksheet(reinterpret_cast<const wchar_t*>(fileName.utf16()));

    if (!sheet)
        return false;

    BasicExcelCell* cell;
    cell        = sheet->Cell(0, 0);
    QString str = "test";
    if (cell)
    {
        cell->SetWString(reinterpret_cast<const wchar_t*>(str.utf16()));
    }
    e.Save();
    return true;
}
