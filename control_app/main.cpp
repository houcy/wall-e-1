#include "main_widget.h"
#include "tcp_server.h"
#include "log.h"
#include "common.h"
#include <QApplication>
#ifdef Q_WS_QWS
#include <QAbstractSlider>
#include <QWSServer>
#endif

void outOfMemHandler()
{
    fatal("Failed to allocate memory");
}

int main(int argc, char *argv[])
{
    std::set_new_handler(outOfMemHandler);
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName(COMPANY_NAME);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    MainWidget mainWidget;

#ifdef Q_WS_QWS
    QList<QAbstractSlider *> sliders = mainWidget.findChildren<QAbstractSlider *>();
    foreach (QAbstractSlider *slider, sliders)
        slider->setAttribute(Qt::WA_AcceptTouchEvents);

    QWSServer::setCursorVisible(false);
#endif

    mainWidget.show();

    return a.exec();
}
