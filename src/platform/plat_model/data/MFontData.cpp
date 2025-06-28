#include "MFontData.h"

MFontData::MFontData(void)
{
}

MFontData::MFontData(const MFontData& info)
{

    this->ExtendInfo(info.ExtendInfo());
    this->Font(info.Font());
    this->Color(info.Color());
    this->UserType(info.UserType());
}

MFontData::MFontData(MFontData* info)
{
    if (info == nullptr)
    {
        return;
    }
    this->ExtendInfo(info->ExtendInfo());
    this->Font(info->Font());
    this->Color(info->Color());
    this->UserType(info->UserType());
}

MFontData::~MFontData(void)
{
}

void MFontData::UserType(EUSERTYPE val)
{
    m_userType = val;
}

EUSERTYPE MFontData::UserType() const
{
    return m_userType;
}

void MFontData::Color(QColor val)
{
    m_color = val;
}

QColor MFontData::Color() const
{
    return m_color;
}

void MFontData::Font(QFont val)
{
    m_font = val;
}

QFont MFontData::Font() const
{
    return m_font;
}

void MFontData::SetBold(bool isVal)
{
    m_font.setBold(isVal);
}

void MFontData::SetUnderline(bool isVal)
{
    m_font.setUnderline(isVal);
}

void MFontData::SetItalic(bool isVal)
{
    m_font.setItalic(isVal);
}

void MFontData::SetFontSize(int size)
{
    m_font.setPixelSize(size);
}

void MFontData::SetFontColor(QString colorName)
{
    m_color.setNamedColor(colorName);
}

void MFontData::SetFamily(QString family)
{
    m_font.setFamily(family);
}
