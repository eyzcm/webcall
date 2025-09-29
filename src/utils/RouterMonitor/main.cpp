#include "../common/utils/version.h"
#include "RMAgent.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES); // OpenGL,否则系统退出会导致异常
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); //高分屏支持

    QString strCmd = "";
    for (int i = 0; i < argc; i++)
    {
        if (QString(argv[i]) == "logout")
        {
            strCmd = argv[i];
        }
    }
    QApplication a(argc, argv);
    RMAgent::instance()->start(strCmd);
    return a.exec();
}
