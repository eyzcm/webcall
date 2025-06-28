#include "plat_resouce.h"

#include <QDir>
#include <QResource>
#include <QTranslator>
#include <QApplication>

Plat_resouce::Plat_resouce()
{
}

Plat_resouce::~Plat_resouce()
{
}

Plat_resouce * gInterface = new Plat_resouce();
DllLoadInterface * GetDllInterface()
{
//    gInterface->connect( gInterface->m_main, SIGNAL(S_Init()),gInterface,SLOT(R_Init()));

    return gInterface;

}

QString Plat_resouce::GetModuleName()
{
    return "El_resouce";
}

void Plat_resouce::Init( int tcount ,  char * argv[]  )
{
    QResource::registerResource("plat_easyliaoclient.rcc");
    QFileInfoList fileInfoList = QDir(qApp->applicationDirPath().append("\\el_translations") ).entryInfoList();

    foreach( QFileInfo info , fileInfoList)
    {
        if( !info.isFile()|| 0 != info.suffix().compare("qm",Qt::CaseInsensitive)) continue;
        QTranslator *eltranslator = new QTranslator(qApp);
        eltranslator->load(info.filePath());
         qApp->installTranslator(eltranslator);

    }
}

bool Plat_resouce::CheckVersion( QString version )
{
    return false;
}
