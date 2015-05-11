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
    int index = carTurnMethodComboBox->findData(QVariant(turnMethod));

    if (index != -1)
        carTurnMethodComboBox->setCurrentIndex(index);
    else
        qWarning("Wrong turn method");
}

int OptionsDialog::getTurnMethod()
{
    return carTurnMethodComboBox->
        itemData(carTurnMethodComboBox->currentIndex()).toInt();
}
