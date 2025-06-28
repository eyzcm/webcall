#pragma once

#include <QMenu>
#include <QWidget>

#include "utils/EnumDefine.h"
#include "ui_UIBackgroundColorWidget.h"

class UIBackgroundColorWidget : public QWidget
{
    Q_OBJECT

public:
    UIBackgroundColorWidget(QWidget *parent = 0);
    ~UIBackgroundColorWidget();

private slots:
    void R_changeColor(QString);
    void R_changeOpacity(int);
    void R_Reset();
    void R_Close();
    void R_Save();
    void R_setChatChecked(bool checked);
    void R_setEditChecked(bool checked);

private:
    void                        init();
    QString                     QColorToQString(QColor color, int apl);
    Ui::UIBackgroundColorWidget ui;
    int                         m_chatOpacity;
    QString                     m_chatColor;
    int                         m_editOpacity;
    QString                     m_editColor;
};
