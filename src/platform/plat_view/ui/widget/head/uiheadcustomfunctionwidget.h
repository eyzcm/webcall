#ifndef UIHEADCUSTOMFUNCTIONWIDGET_H
#define UIHEADCUSTOMFUNCTIONWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMap>

class UIHeadCustomFunctionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UIHeadCustomFunctionWidget(QWidget *parent = nullptr);
    ~UIHeadCustomFunctionWidget();

    void AddFunctionButton(const QString &buttonName, const QIcon &icon, const QString &buttonText);
    void RemoveFunctionButton(const QString &buttonName);
    QPushButton* GetFunctionButton(const QString &buttonName) const;

private:
    QHBoxLayout *m_functionLayout;
    QMap<QString, QPushButton*> m_functionButtonMap;

    void LoadStylesheet();
};

#endif // UIHEADCUSTOMFUNCTIONWIDGET_H
