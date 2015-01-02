#ifndef UIHANDLER_H
#define UIHANDLER_H

#include <QObject>

#include "uihelper.h"

class LedLocker {
public:
    LedLocker();
    ~LedLocker();
};


class UiHandler : public QObject
{
    Q_OBJECT

    UiHelper *ui;

private:
    void setResult(const QString &line1, const QString &line2="");
    void uiPolling();

public:
    explicit UiHandler(QObject *parent = 0);

    void setup(UiHelper *_ui);

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

#endif // UIHANDLER_H
