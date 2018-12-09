#ifndef LED_H
#define LED_H

#include <QObject>

class Led: public QObject
{
    Q_OBJECT
public:
    explicit Led(QObject *parent = 0);

public:
    int fd;
    int on[4];

public:
    // Led();
    ~Led();

public:
    void led1_on();
    void led2_on();
    void led3_on();
    void led4_on();
};

#endif // LED_H
