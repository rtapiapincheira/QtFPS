#ifndef HANDLER_H
#define HANDLER_H

#include "helper.h"
#include "oem/oem.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QObject>

extern Oem oem;

class LedLocker {
public:
    LedLocker();
    ~LedLocker();
};

void handlerUiPolling(void *parameter=NULL);

class Handler : public QObject {
    Q_OBJECT

    Helper *ui;
    bool bContinue;

private:
    void setResult(const QString &line1, const QString &line2="");


public:
    explicit Handler(QObject *parent = 0);

    void setup(Helper *_ui);

signals:

private slots:
    void handleSaveImageToFileClicked();
    void handleOpenClicked();
    void handleCloseClicked();
    void handleEnrollClicked();
    void handleGetUserCountClicked();
    void handleVerifyClicked();
    void handleDeleteIdClicked();
    void handleIdentifyClicked();
    void handleDeleteAllClicked();
    void handleVerifyTemplateClicked();
    void handleGetTemplateClicked();
    void handleIdentifyTemplateClicked();
    void handleSetTemplateClicked();
    void handleIsPressedFingerClicked();
    void handleGetDatabaseClicked();
    void handleGetImageClicked();
    void handleSetDatabaseClicked();
    void handleGetRawImageClicked();
    void handleCancelClicked();
    void handleGetLiveImageClicked();
};

#endif // HANDLER_H
