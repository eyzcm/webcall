#ifndef UICUSTOMTITLEBARWIDGET_H
#define UICUSTOMTITLEBARWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

class UICustomTitleBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UICustomTitleBarWidget(QWidget *parent = nullptr);

signals:
    void SignalMinimize();
    void SignalMaximizeRestore();
    void SignalClose();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QPoint m_dragPosition;
    bool m_isMaximized;

    QLabel *m_titleLabel;
    QPushButton *m_minimizeButton;
    QPushButton *m_maximizeRestoreButton;
    QPushButton *m_closeButton;

    void UpdateMaximizeRestoreButton();

private slots:
    void OnMinimizeButtonClicked();
    void OnMaximizeRestoreButtonClicked();
    void OnCloseButtonClicked();
};

#endif // UICUSTOMTITLEBARWIDGET_H
