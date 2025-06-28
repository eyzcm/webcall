#include "mainwindow.h"

#include <QApplication>
#include <QLibrary>
#include <QLibrary>
#include <QMessageBox>
#include <QTextCodec>
#include <QToolTip>
#include <QTranslator>
#include <el_login.h>
#include <qdebug.h>
#include "dllloadinterface.h"

typedef  void  *   ( *fgetDllInterface)();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForLocale(codec);

    QTranslator *translator = new QTranslator(qApp);
    translator->load(":/Resources/translations/qt_zh_CN.qm");
    qDebug() << "qt_zh_CN.qm " << qApp->installTranslator(translator);

    QTranslator *translator2 = new QTranslator(qApp);
    translator2->load(":/Resources/translations/widgets.qm");
    qDebug() << "widgets.qm " << qApp->installTranslator(translator2);

    QFont font(tr("微软雅黑"), -1, 50);
    font.setPointSize(9);
    QToolTip::setFont(font); //设置ToolTip字体
    qApp->setFont(font);

    QMessageBox::question(nullptr,"asd","asdf");

    QLibrary dll("plat_tcp");

    if( dll.load() )
    {


        fgetDllInterface dll_load = (fgetDllInterface)dll.resolve("GetDllInterface");
        if( dll_load )
        {
           DllLoadInterface * dllloadserver =  (DllLoadInterface *)dll_load();
           if( dllloadserver )
           {
               char * tt[1];
               tt[0] = nullptr;
                QString moduname = dllloadserver->GetModuleName();


               dllloadserver->Init( 0, nullptr);
           }
        }

    }


    El_login * ellogin = new El_login();
    ellogin->runLogin();


    return a.exec();
}
