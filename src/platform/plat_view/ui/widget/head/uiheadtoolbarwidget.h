#ifndef UIHEADTOOLBARWIDGET_H
#define UIHEADTOOLBARWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMap>

class UIHeadToolbarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UIHeadToolbarWidget(QWidget *parent = nullptr);
    ~UIHeadToolbarWidget();

    void AddButton(const QString &buttonName, const QIcon &icon, const QString &buttonText);
    void RemoveButton(const QString &buttonName);
    QPushButton* GetButton(const QString &buttonName) const;

private:
    QHBoxLayout *m_toolbarLayout;
    QMap<QString, QPushButton*> m_buttonMap;

    void LoadStylesheet();
};

#endif // UIHEADTOOLBARWIDGET_H
