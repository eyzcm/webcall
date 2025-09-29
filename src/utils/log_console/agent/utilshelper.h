#ifndef UTILSHELPER_H
#define UTILSHELPER_H

#include <QObject>

class UtilsHelper : public QObject
{
	Q_OBJECT

public:
	UtilsHelper(QObject *parent);
	~UtilsHelper();

	static void Execute(QString body);

	static void Execute(QString receiver, QString body);

    static QString GetAppdataCacheDir();
    static QString GetAppdataLogDir();

private:
};

#endif // UTILSHELPER_H
