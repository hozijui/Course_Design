#ifndef LEDDIALOG_H
#define LEDDIALOG_H

#include <QtGui/QDialog>
#include <Qt/QtGui>
#include "led.h"

class LedDialog: public QDialog
{
    Q_OBJECT

private :
    QLabel *label;
    QPushButton *led1Button;
    QPushButton *led2Button;
    QPushButton *led3Button;
    QPushButton *led4Button;
    QPushButton *exitButton;
    Led *ledobj;

private
    slots
        :
        void
    Light_style1(void);
    void Light_style2(void);
    void Light_style3(void);
    void Light_style4(void);
    void Light_style_exit(void);
    void Light_delay(int times);

public:
    LedDialog(QWidget *parent = 0);
    ~LedDialog();
};

#endif // LEDDIALOG_H
