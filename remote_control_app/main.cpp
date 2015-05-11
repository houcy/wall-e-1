#include "main_window.h"
#include "common.h"
#include <QApplication>
#include <QGridLayout>

void logHandler(QtMsgType type, const QMessageLogContext &context,
    const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();

    switch (type) {
    case QtDebugMsg:
#ifdef QT_DEBUG
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(),
            context.file, context.line, context.function);
#endif
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(),
            context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(),
            context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(),
            context.file, context.line, context.function);
        exit(EXIT_FAILURE);
    }
}

void outOfMemHandler()
{
    qFatal("Failed to allocate memory");
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(logHandler);
    std::set_new_handler(outOfMemHandler);
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName(COMPANY_NAME);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    MainWindow w;

    w.show();

    return a.exec();
}
