#ifndef UIINPUTAREAWIDGET_H
#define UIINPUTAREAWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

class UIInputAreaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UIInputAreaWidget(QWidget *parent = nullptr);

    // Public method to set callback for the send button
    void SetSendButtonCallback(const std::function<void()>& callback);

private:
    // UI Components
    QHBoxLayout* m_toolBarLayout;
    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_buttonContainerLayout;
    QVBoxLayout* m_textInputLayout;
    QTextEdit* m_richTextEdit;
    QPushButton* m_sendButton;
    QPushButton* m_styleToggleButton;

    // Callback function for the send button
    std::function<void()> m_sendButtonCallback;

    void SetupUI(); // Method to setup UI components
    void LoadStylesheet(); // Method to load CSS file

private slots:
    void OnSendButtonClicked(); // Slot for send button click
};

#endif // UIINPUTAREAWIDGET_H
