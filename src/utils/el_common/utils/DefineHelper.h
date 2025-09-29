#pragma once

#include <QObject>
#include <QString>
#include <QStringList>


#define REG_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"


#define StringCompare(x, y) x.compare(y, Qt::CaseInsensitive) == 0

#define Property_QString(x, y) \
public:                        \
    QString x() const          \
    {                          \
        return m_##y;          \
    }                          \
    void x(const QString& v)   \
    {                          \
        m_##y = v;             \
    }                          \
                               \
protected:                     \
    QString m_##y;

#define Property_Int(x, y) \
public:                    \
    int x() const          \
    {                      \
        return m_##y;      \
    }                      \
    void x(int v)          \
    {                      \
        m_##y = v;         \
    }                      \
                           \
protected:                 \
    int m_##y;

#define Property_BOOL(x, y) \
public:                     \
    bool x() const          \
    {                       \
        return m_##y;       \
    }                       \
    void x(bool v)          \
    {                       \
        m_##y = v;          \
    }                       \
                            \
protected:                  \
    bool m_##y;


#define Property_QStringList(x, y) \
public:                            \
    QStringList x() const          \
    {                              \
        return m_##y;              \
    }                              \
    void x(QStringList v)          \
    {                              \
        m_##y = v;                 \
    }                              \
                                   \
protected:                         \
    QStringList m_##y;

#define CompareType(x, y) ((x) & (y)) == (y)

#define STDPRINTF(x) \
    printf(x);       \
    printf("\n");    \
    fflush(stdout);
#define STDERR(x)          \
    fprintf(stderr, x);    \
    fprintf(stderr, "\n"); \
    fflush(stderr);


