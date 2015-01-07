#ifndef ARGSPARSER_H
#define ARGSPARSER_H

#include "config.h"

#include <QtCore>

#ifdef ENABLED_QT5
#   include <QCommandLineOption>
#   include <QCommandLineParser>
#endif

class ArgsParser {
private:
    QStringList args;
#ifdef ENABLED_QT5
    QCommandLineParser parser;
#endif

    void maybeExit(bool cond, int ret=1);

public:
    static QStringList actions;

    void prepare(QCoreApplication &a);

    Config validate();
};

#endif // ARGSPARSER_H
