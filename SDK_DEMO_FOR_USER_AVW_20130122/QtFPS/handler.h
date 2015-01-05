#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>

#include "helper.h"

class LedLocker {
public:
    LedLocker();
    ~LedLocker();
};

class Handler : public QObject
{
    Q_OBJECT

    Helper *ui;
    bool bContinue;

private:
    void setResult(const QString &line1, const QString &line2="");
    void uiPolling();

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
