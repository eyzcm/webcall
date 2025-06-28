#pragma once
#include "utils/Plat_EnumDefine.h"
#include "data/MBaseData.h"
#include <QColor>
#include <QFont>
#include <QObject>
#include "plat_model_global.h"

class PLAT_MODEL_EXPORT MFontData : public MBaseData
{
    Q_OBJECT
public:
    MFontData(void);
    MFontData(const MFontData& info);
    MFontData(MFontData* info);
    ~MFontData(void);

public:
    void SetBold(bool isVal);
    void SetUnderline(bool isVal);
    void SetItalic(bool isVal);
    void SetFontSize(int size);
    void SetFontColor(QString colorName);
    void SetFamily(QString family);

    QFont     Font() const;
    void      Font(QFont val);
    QColor    Color() const;
    void      Color(QColor val);
    EUSERTYPE UserType() const;
    void      UserType(EUSERTYPE val);

private:
    QFont     m_font;
    QColor    m_color;
    EUSERTYPE m_userType;
};
