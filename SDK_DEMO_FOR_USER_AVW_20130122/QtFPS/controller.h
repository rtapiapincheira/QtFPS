#ifndef HANDLER_H
#define HANDLER_H

#include "helper.h"
#include "oem.h"

#include "config.h"
#include <QDebug>
#include <QObject>

class LedLocker {
private:
    Oem *oem;
public:
    LedLocker(Oem *_oem);
    ~LedLocker();
};

void handlerUiPolling(void *parameter=NULL);

class Controller : public QObject {
    Q_OBJECT

    Helper *ui;
    bool bContinue;
    Oem oem;

private:
    void setResult(const QString &line1, const QString &line2="");

public:
    explicit Controller(QObject *parent = 0);

    void setup(Helper *_ui);

signals:

public
#ifdef MODE_WINDOW
    slots
#endif
    :
    void __saveImageToFile();
    void __open();
    void __close();
    void __enroll();
    void __getUserCount();
    void __verify();
    void __deleteId();
    void __identify();
    void __deleteAll();
    void __verifyTemplate();
    void __getTemplate();
    void __identifyTemplate();
    void __setTemplate();
    void __isPressedFinger();
    void __getDatabase();
    void __getImage();
    void __setDatabase();
    void __getRawImage();
    void __cancel();
    void __getLiveImage();
};

#endif // HANDLER_H
