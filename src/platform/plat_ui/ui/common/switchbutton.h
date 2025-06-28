#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QToolButton>

class SwitchButton : public QToolButton
{
    Q_OBJECT

public:
    SwitchButton(QWidget* parent = 0);
    ~SwitchButton();
    void Init();
    void SetCheckStyle(const QString& style);
    void SetUnCheckStyle(const QString& style);
    void SetDisableStyple(const QString& style);

    bool IsBtnChecked() const
    {
        return m_isBtnChecked;
    }
    void IsBtnChecked(bool val);

    bool IsBtnEnable() const
    {
        return m_isBtnEnable;
    }
    void IsBtnEnable(bool val);

    void DoClicked();

signals:
    void S_CheckChange(bool isChecked);
    void S_Clicked();
private slots:
    void R_clicked();

private:
    bool    m_isBtnChecked;
    QString m_checkStyle;
    QString m_unCheckStyle;
    QString m_disableStyple;
    bool    m_isBtnEnable;
};

#endif // SWITCHBUTTON_H
