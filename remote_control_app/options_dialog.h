#ifndef OPTIONS_DIALOG_H
#define OPTIONS_DIALOG_H

#include <QDialog>

class QGridLayout;
class QDialogButtonBox;
class QGroupBox;
class QComboBox;

class OptionsDialog : public QDialog
{
    QComboBox *carTurnMethodComboBox;

    QGroupBox *createTurnMethodConfigurationUi();
    QDialogButtonBox *createDialogButtons();
    QGridLayout *createDialogLayout();

public:
    explicit OptionsDialog(QWidget *parent = 0);

    void setTurnMethod(int turnMethod);
    int getTurnMethod();
};

#endif // OPTIONS_DIALOG_H
