#ifndef STDAFX_H
#define STDAFX_H

//qt库文件
#include <QDateTime>
#include <QList>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QThread>
#include <QEvent>
#include <QStringList>
#include <QLabel>
#include <QMovie>
#include <QPushButton>
#include <QWidget>
#include <QColor>
#include <QMutex>
#include <QTimer>
#include <QVariant>
#include <QMap>
#include <qdebug.h>
#include <QListWidgetItem>
#include <QFont>
#include <QValidator>
#include <QMultiMap>
#include <QtCore/qglobal.h>
#include <QRect>
#include <QVector>
#include <qtextedit>
#include <QResizeEvent>
#include <QApplication>
#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QVBoxLayout>
#include <QToolButton>
#include <QLayout>
#include <QScrollArea>
#include <QGraphicsDropShadowEffect>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QComboBox>
#include <QLineEdit>
#include <QListView>
#include <QMouseEvent>
#include <QScrollBar>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QToolTip>
#include <QFontMetrics>
#include <QAbstractTextDocumentLayout>
#include <QTreeView>
#include <QTextCharFormat>
#include <QKeyEvent>
#include <QCheckBox>
#include <QDebug>
#include <QSignalMapper>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QAction>
#include <QStyleOptionViewItem>
#include <QDialog>
#include <QIcon>
#include <QPointer>
#include <QWebEngineCookieStore>
#include <QWebEngineDownloadItem>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineView>
#include <QtWebChannel>
#include <QShortcut>
#include <QTreeWidgetItem>
#include <QNetworkReply>
#include <QCloseEvent>
#include <QFocusEvent>
#include <QCalendarWidget>
#include <QProcess>
#include <QString.h>
#include <QTextEdit.h>
#include <QDragEnterEvent>
#include <QDrag>
#include <QMimeData>
#include <QScopedPointer>
#include <QColorDialog>
#include <QFileDialog>
#include <QHeaderView>
#include <QTableView>
#include <QAbstractTableModel>
#include <QSize>
#include <qobject.h>
#include <QClipboard>
#include <QDesktopServices>
#include <QScreen>
#include <QProxyStyle>
#include <QDesktopWidget>
#include <QPoint>
#include <QStyleOption>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <qevent.h>
#include <qtextcodec.h>
#include <QUrl>
#include <QListWidget>
#include <QtGlobal>
#include <QDateTimeEdit>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QUuid>
#include <QtDebug>
#include <QRegExpValidator>
#include <QRadioButton>
#include <QDate>
#include <QFileInfo>
#include <QCursor>
#include <QWheelEvent>
#include <QTextCodec>
#include <QTreeWidget>
#include <qapplication.h>
#include <QMutexLocker>
#include <QPainterPath>



//utils文件
#include "utils/UISingletonTimer.h"
#include "utils/jsonhelper.h"


//plat层文件
#include "session/el_sessionservice.h"
#include "service/util_service.h"
#include "utils/utilshelper.h"


#include "serviceinterface/dbuiplatservice.h"
#include "service/PlatCommonSignalService.h"
#include "service/platcommonmodelservice.h"
#include "protocol/platprotocolservice.h"
#include "model/uiplatcommondatamodel.h"



#ifndef _MSC_VER
#define UINT unsigned int
#define DWORD unsigned long
#endif


#define LOADCSSFILE(X) \
    QFile file(QString("://Resources/css/%1").arg(X));\
    if (file.open(QFile::ReadOnly)) \
    {\
        QTextStream stream(&file);\
        setStyleSheet(stream.readAll());\
        file.close();\
    }


#endif // STDAFX_H
