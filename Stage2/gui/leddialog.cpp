#include "leddialog.h"
#include <QtGui>

//Construction Function
LedDialog::LedDialog(QWidget *parent)
    : QDialog(parent)
{
    // init component
    label = new QLabel(tr("Set Led"));
    led1Button = new QPushButton(tr("Led1"));
    led2Button = new QPushButton(tr("Led2"));
    led3Button = new QPushButton(tr("Led3"));
    led4Button = new QPushButton(tr("Led4"));
    exitButton = new QPushButton(tr("Exit"));
    ledobj = new Led();

    //layout component
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label, Qt::AlignTop);
    layout->addWidget(led1Button);
    layout->addWidget(led2Button);
    layout->addWidget(led3Button);
    layout->addWidget(led4Button);
    layout->addWidget(exitButton);
    setLayout(layout);
    //resize(200,230);
    //setFixedSize(200,230);
    // this->setFixedSize( this->width (),this->height ());
    setMinimumSize(200, 230);
    setMaximumSize(200, 230);

    // signal-slots connect
    QObject::connect(led1Button, SIGNAL(clicked()), this, SLOT(Light_style1()));
    QObject::connect(led2Button, SIGNAL(clicked()), this, SLOT(Light_style2()));
    QObject::connect(led3Button, SIGNAL(clicked()), this, SLOT(Light_style3()));
    QObject::connect(led4Button, SIGNAL(clicked()), this, SLOT(Light_style4()));
    QObject::connect(exitButton, SIGNAL(clicked()), this, SLOT(Light_style_exit()));

}

//self-declaration function implement
void LedDialog::Light_style1(void)
{
    ledobj->led1_on();
    Light_delay(100);
}

void LedDialog::Light_style2(void)
{

    ledobj->led2_on();
    Light_delay(2000);
}

void LedDialog::Light_style3(void)
{
    ledobj->led3_on();
    Light_delay(2000);
}

void LedDialog::Light_style4(void)
{
    ledobj->led4_on();
    Light_delay(2000);;
}

void LedDialog::Light_style_exit(void)
{
    exit(0);
}

void LedDialog::Light_delay(int times)
{
    int i;
    int j;
    for (i = 0; i < times; i++)
        for (j = 0; j < times; j++) {
            // Null function
        }
}

LedDialog::~LedDialog()
{

}
