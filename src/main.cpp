#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QSplashScreen>
#include <QScreen>
#include <QTimeZone>
#include <sysutil.h>
#include "nethelper.h"
#include "version.h"

#define _ QStringLiteral

MainWindow *w;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qss(QStringLiteral(":/res/light.qss"));
    if (qss.open(QFile::ReadOnly)) {
        a.setStyleSheet(qss.readAll());
        qss.close();
    }

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(QStringLiteral(":/icon/images/splash.jpeg")));
    splash->show();

    splash->showMessage(QObject::tr("正在加载资源..."),
                        Qt::AlignBottom | Qt::AlignLeft, Qt::white);

    splash->showMessage(QObject::tr("正在加载站点数据..."),
                        Qt::AlignBottom | Qt::AlignLeft, Qt::white);

    QCoreApplication::setOrganizationName(_("yuny"));
    QCoreApplication::setOrganizationDomain(_("op9.top"));
    QCoreApplication::setApplicationName(_("云映"));

    w = new MainWindow;
    w->setUp();
    w->setWindowTitle(QObject::tr("云映 ") + QObject::tr(THISVERSION));
    w->setWindowIcon(QIcon(_(":/icon/images/ticket.ico")));

    QList<QScreen *> screen = QGuiApplication::screens();
    if (!screen.isEmpty()) {
        QRect rect = screen[0]->geometry();
        w->move((rect.width() - w->width()) / 2, (rect.height() - w->height()) / 2);
    }

    splash->showMessage(QObject::tr("加载完成"),
                        Qt::AlignBottom | Qt::AlignLeft, Qt::white);
    splash->finish(w);
    delete splash;

    NetHelper *nh = NetHelper::instance();
    nh->initLoginCookie();
    nh->getLoginConf();
    nh->checkUpdate();
    nh->leftTicketInit();
#ifdef HAS_CDN
    w->getCdn();
#endif
    w->syncTime();
    w->show();

    return a.exec();
}
