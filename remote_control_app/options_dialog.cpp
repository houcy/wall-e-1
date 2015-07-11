#include "options_dialog.h"
#include "cmd.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QGridLayout>

OptionsDialog::OptionsDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Settings"));
    setModal(true);
    resize(200, 200);

    setLayout(createDialogLayout());
}

QGroupBox *OptionsDialog::createTurnMethodConfigurationUi()
{
    carTurnMethodComboBox = new QComboBox;
    carTurnMethodComboBox->addItem("Skid Steer",
        QVariant(Cmd::CMD_DATA_TURN_METHOD_SKID_STEER));
    carTurnMethodComboBox->addItem("Diferential",
        QVariant(Cmd::CMD_DATA_TURN_METHOD_DIRERENTIAL));

    QVBoxLayout *configurationGroupBoxLayout = new QVBoxLayout;
    configurationGroupBoxLayout->addWidget(carTurnMethodComboBox);

    QGroupBox *configurationGroupBox = new QGroupBox(tr("Turn method:"));
    configurationGroupBox->setLayout(configurationGroupBoxLayout);
    configurationGroupBox->setStyleSheet("QGroupBox { border: 0px; "
        "margin-top: 15px; }");

    return configurationGroupBox;
}

QDialogButtonBox *OptionsDialog::createDialogButtons()
{
    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    buttonBox->addButton(tr("OK"), QDialogButtonBox::AcceptRole);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    buttonBox->addButton(tr("Cancel"),QDialogButtonBox::RejectRole);
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    return buttonBox;
}

QGridLayout *OptionsDialog::createDialogLayout()
{
    QGridLayout *optionsDialogLayout = new QGridLayout;
    optionsDialogLayout->addWidget(createTurnMethodConfigurationUi(), 0, 0,
        Qt::AlignTop | Qt::AlignLeft);
    optionsDialogLayout->addWidget(createDialogButtons(), 1, 0);

    return optionsDialogLayout;
}

void OptionsDialog::setTurnMethod(int turnMethod)
{
    if (turnMethod != Cmd::CMD_DATA_TURN_METHOD_UNDEF)
    {
        carTurnMethodComboBox->setEnabled(true);
        carTurnMethodComboBox->setCurrentIndex(carTurnMethodComboBox->
            findData(QVariant(turnMethod)));
    }
    else
        carTurnMethodComboBox->setEnabled(false);
}

int OptionsDialog::getTurnMethod()
{
    if (carTurnMethodComboBox->isEnabled())
    {
        return carTurnMethodComboBox->
            itemData(carTurnMethodComboBox->currentIndex()).toInt();
    }

    return Cmd::CMD_DATA_TURN_METHOD_UNDEF;
}
