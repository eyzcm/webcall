#ifndef UICENTRALAREAWIDGET_H
#define UICENTRALAREAWIDGET_H

#include <QWidget>
#include <QSplitter>


class UICentralAreaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UICentralAreaWidget(QWidget *parent = nullptr);

private:
    void SetupLayout();
    QWidget *CreateConversationArea();
    QWidget *CreateInputArea();

    void Init();

    QSplitter *m_centralSplitter;
};

#endif // UICENTRALAREAWIDGET_H
