#ifndef HANDLER_H
#define HANDLER_H

#include "helper.h"
#include "oem.h"

#include "config.h"
#include <QDebug>
#include <QObject>

/**
 * @brief The LedLocker class implements a simple mechanism to turn on/off the CMOS led included the
 * fingerprint sensor.
 */
class LedLocker {
private:
    /**
     * @brief oem reference to the Oem object we will issue commands for turning on/off the CMOS
     * led.
     */
    Oem *oem;
public:

    /**
     * @brief LedLocker turns the CMOS led on.
     * @param _oem object representing the fingerprint sensor interface we will be talking to.
     */
    LedLocker(Oem *_oem);

    /**
      * Turns the CMOS led off of the connected fingerprint sensor.
      */
    ~LedLocker();
};

// Deprecated: remove in future versions
void handlerUiPolling(void *parameter=NULL);

/**
 * @brief The Controller class implements the high-level logic for talking to the sensor. It
 * includes an Oem object which is the sensor interface itself. This class does the
 * parameter/error/response handling, as well as it calls the proper UI methods for updating it.
 *
 * See the Helper class to understand the way it takes parameter and uses them to perform all the
 * actions.
 */
class Controller : public QObject {
    Q_OBJECT

    Helper *ui;
    bool bContinue;
    Oem oem;

private:
    /**
     * @brief setResult outputs a message (optionally with a second line) in the current UI (no
     * matter it is MODE_WINDOW o console).
     * @param line1 first line of the message.
     * @param line2 second line of the message (is empty, only the first line is shown).
     */
    void setResult(const QString &line1, const QString &line2="");

public:
    /**
     * @brief Controller builds a new instance of Controller.
     */
    explicit Controller();

    /**
     * @brief setup sets the object where UI calls will be delegated.
     * @param _ui helper object where to delegate the UI related calls (updating the UI, drawing
     * images, showing text, showing modal dialogs to prompt filenames, etc.)
     */
    void setup(Helper *_ui);

signals:

public
#ifdef MODE_WINDOW
    slots
#endif
    :

    /**
     * @brief __saveImageToFile saves the last captured image to disk.
     */
    void __saveImageToFile();

    /**
     * @brief __open opens the serial port and initializes the sensor communication.
     */
    void __open();

    /**
     * @brief __close closes the sensor communication and closes the serial port.
     */
    void __close();

    /**
     * @brief __enroll starts the process for enrolling a new user.
     */
    void __enroll();

    /**
     * @brief __getUserCount returns the count of enrolled users (users in the fingerprint sensor
     * internal database).
     */
    void __getUserCount();

    /**
     * @brief __verify verifies if the user matches an id.
     */
    void __verify();

    /**
     * @brief __deleteId deletes a previously enrolled user from the fingerprint internal database.
     */
    void __deleteId();

    /**
     * @brief __identify gets the id of an unknown fingerprint being taken from the sensor.
     */
    void __identify();

    /**
     * @brief __deleteAll clears the whole fingerprint internal database.
     */
    void __deleteAll();

    /**
     * @brief __verifyTemplate verifies if a template taken from a file matches an id.
     */
    void __verifyTemplate();

    /**
     * @brief __getTemplate gets a template for a user from the fingerprint internal database and
     * writes it to disk.
     */
    void __getTemplate();

    /**
     * @brief __identifyTemplate gets the id of an unknown template being taken from a file.
     */
    void __identifyTemplate();

    /**
     * @brief __setTemplate sets a template into the fingerprint internal database taking it from a
     * file.
     */
    void __setTemplate();

    /**
     * @brief __isPressedFinger checks whether a finger is pressed against the sensor (kind of a
     * button).
     */
    void __isPressedFinger();

    /**
     * @brief __getDatabase retrieves the whole fingerprint internal database and saves it to a
     * file.
     */
    void __getDatabase();

    /**
     * @brief __getImage captures and download to computer ram a fingerprint as an image. Make sure
     * to call __saveImageToFile() afterwards, to get the image file.
     */
    void __getImage();

    /**
     * @brief __setDatabase replaces the whole fingerprint internal database from a database taken
     * from a file.
     */
    void __setDatabase();

    /**
     * @brief __getRawImage captures and download to computer ram a raw fingerprint as an image.
     * Make sure to call __saveImageToFile() afterwards, to get the image file.
     */
    void __getRawImage();

    /**
     * @brief __cancel currently cancels the __getLiveImage() execution. It only works when
     * executing in MODE_WINDOW because it needs to be called from GUI thread.
     */
    void __cancel();

    /**
     * @brief __getLiveImage performs a continuous capturing and downloading of the current finger
     * pressed against the fingerprint sensor.
     */
    void __getLiveImage();
};

#endif // HANDLER_H
