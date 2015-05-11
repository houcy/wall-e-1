#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CentralWidget;
class OptionsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    CentralWidget *centWidget;
    OptionsDialog *optionsDialog;

    void createMenus();
    void createSettingsMenu();
    QAction *createOptionsSubmenu();

public:
    explicit MainWindow(QWidget *parent = 0);
    MainWindow(const MainWindow &) = delete;
    MainWindow &operator=(const MainWindow &) = delete;
    ~MainWindow();

public slots:
    void showOptionsDialog();
    void saveOptions();
};

#endif // MAINWINDOW_H
