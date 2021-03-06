#include "main_window.h"
#include "central_widget.h"
#include "common.h"
#include "options_dialog.h"
#include <QMenuBar>

#define ICON_NAME ":images/images/main_window/icon.png"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(APPLICATION_NAME);
    setWindowIcon(QIcon(ICON_NAME));

    createMenus();

    centWidget = new CentralWidget;
    setCentralWidget(centWidget);
}

MainWindow::~MainWindow()
{
}

void MainWindow::createMenus()
{
    createSettingsMenu();
}

void MainWindow::createSettingsMenu()
{
    menuBar()->addMenu(tr("Settings"))->addAction(createOptionsSubmenu());
}

QAction *MainWindow::createOptionsSubmenu()
{
    optionsDialog = new OptionsDialog(this);
    QAction *optionsAction = new QAction(tr("Options"), this);
    connect(optionsAction, SIGNAL(triggered()), this,
        SLOT(showOptionsDialog()));
    connect(optionsDialog, SIGNAL(accepted()), this, SLOT(saveOptions()));

    return optionsAction;
}

void MainWindow::showOptionsDialog()
{
    optionsDialog->setTurnMethod(centWidget->getTurnMethod());
    optionsDialog->show();
}

void MainWindow::saveOptions()
{
    // save local settings
    centWidget->writeLocalSettings();

    // save remote settings
    centWidget->setTurnMethod(optionsDialog->getTurnMethod());
    centWidget->sendRemoteSettings();
}

