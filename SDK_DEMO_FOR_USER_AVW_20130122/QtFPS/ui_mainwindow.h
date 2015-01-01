/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowDialog
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_3;
    QGraphicsView *canvas;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *save_image_to_file;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QLabel *result;
    QGridLayout *gridLayout_2;
    QPushButton *close;
    QPushButton *verify_template;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_3;
    QPushButton *open;
    QPushButton *is_press_finger;
    QPushButton *verify_1_1;
    QLabel *label_2;
    QPushButton *identify_template;
    QPushButton *get_user_count;
    QComboBox *baudrate;
    QPushButton *enroll;
    QPushButton *get_raw_image;
    QPushButton *cancel;
    QPushButton *get_live_image;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *id;
    QPushButton *set_database;
    QPushButton *get_image;
    QComboBox *serial_port_number;
    QPushButton *get_template;
    QPushButton *delete_all;
    QPushButton *identify_1_n;
    QPushButton *delete_id;
    QPushButton *set_template;
    QPushButton *get_database;
    QLabel *label_9;

    void setupUi(QMainWindow *MainWindowDialog)
    {
        if (MainWindowDialog->objectName().isEmpty())
            MainWindowDialog->setObjectName(QStringLiteral("MainWindowDialog"));
        MainWindowDialog->resize(600, 479);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindowDialog->sizePolicy().hasHeightForWidth());
        MainWindowDialog->setSizePolicy(sizePolicy);
        MainWindowDialog->setMinimumSize(QSize(600, 479));
        MainWindowDialog->setMaximumSize(QSize(600, 479));
        centralWidget = new QWidget(MainWindowDialog);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_3 = new QHBoxLayout(centralWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        canvas = new QGraphicsView(centralWidget);
        canvas->setObjectName(QStringLiteral("canvas"));
        canvas->setMinimumSize(QSize(320, 240));
        canvas->setMaximumSize(QSize(320, 240));
        canvas->setFrameShape(QFrame::NoFrame);

        verticalLayout->addWidget(canvas);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, 0, -1, -1);
        save_image_to_file = new QPushButton(centralWidget);
        save_image_to_file->setObjectName(QStringLiteral("save_image_to_file"));
        save_image_to_file->setMaximumSize(QSize(180, 16777215));

        horizontalLayout_4->addWidget(save_image_to_file);


        verticalLayout->addLayout(horizontalLayout_4);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);
        label_4->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_2->addWidget(label_4);

        result = new QLabel(centralWidget);
        result->setObjectName(QStringLiteral("result"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(result->sizePolicy().hasHeightForWidth());
        result->setSizePolicy(sizePolicy2);
        result->setMinimumSize(QSize(0, 40));
        result->setFrameShape(QFrame::Panel);
        result->setFrameShadow(QFrame::Sunken);

        horizontalLayout_2->addWidget(result);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        close = new QPushButton(centralWidget);
        close->setObjectName(QStringLiteral("close"));
        close->setEnabled(false);
        close->setMinimumSize(QSize(120, 25));
        close->setMaximumSize(QSize(120, 16777215));

        gridLayout_2->addWidget(close, 3, 2, 1, 1);

        verify_template = new QPushButton(centralWidget);
        verify_template->setObjectName(QStringLiteral("verify_template"));
        verify_template->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(verify_template, 9, 1, 1, 1);

        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setMaximumSize(QSize(16777215, 5));

        gridLayout_2->addWidget(label_6, 2, 1, 1, 1);

        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setMaximumSize(QSize(16777215, 5));

        gridLayout_2->addWidget(label_7, 15, 1, 1, 1);

        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setMaximumSize(QSize(16777215, 5));

        gridLayout_2->addWidget(label_8, 4, 1, 1, 1);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_3, 0, 1, 1, 1);

        open = new QPushButton(centralWidget);
        open->setObjectName(QStringLiteral("open"));
        open->setMinimumSize(QSize(120, 25));
        open->setMaximumSize(QSize(120, 16777215));

        gridLayout_2->addWidget(open, 3, 1, 1, 1);

        is_press_finger = new QPushButton(centralWidget);
        is_press_finger->setObjectName(QStringLiteral("is_press_finger"));
        is_press_finger->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(is_press_finger, 11, 1, 1, 1);

        verify_1_1 = new QPushButton(centralWidget);
        verify_1_1->setObjectName(QStringLiteral("verify_1_1"));
        verify_1_1->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(verify_1_1, 7, 1, 1, 1);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_2, 1, 1, 1, 1);

        identify_template = new QPushButton(centralWidget);
        identify_template->setObjectName(QStringLiteral("identify_template"));
        identify_template->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(identify_template, 10, 1, 1, 1);

        get_user_count = new QPushButton(centralWidget);
        get_user_count->setObjectName(QStringLiteral("get_user_count"));
        get_user_count->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(get_user_count, 6, 2, 1, 1);

        baudrate = new QComboBox(centralWidget);
        baudrate->setObjectName(QStringLiteral("baudrate"));
        baudrate->setMinimumSize(QSize(0, 20));
        baudrate->setMaximumSize(QSize(90, 16777215));

        gridLayout_2->addWidget(baudrate, 1, 2, 1, 1);

        enroll = new QPushButton(centralWidget);
        enroll->setObjectName(QStringLiteral("enroll"));
        enroll->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(enroll, 6, 1, 1, 1);

        get_raw_image = new QPushButton(centralWidget);
        get_raw_image->setObjectName(QStringLiteral("get_raw_image"));
        get_raw_image->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(get_raw_image, 13, 1, 1, 1);

        cancel = new QPushButton(centralWidget);
        cancel->setObjectName(QStringLiteral("cancel"));
        cancel->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(cancel, 14, 1, 1, 1);

        get_live_image = new QPushButton(centralWidget);
        get_live_image->setObjectName(QStringLiteral("get_live_image"));
        get_live_image->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(get_live_image, 16, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(label);

        id = new QSpinBox(centralWidget);
        id->setObjectName(QStringLiteral("id"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(id->sizePolicy().hasHeightForWidth());
        id->setSizePolicy(sizePolicy3);
        id->setMaximum(199);

        horizontalLayout->addWidget(id);


        gridLayout_2->addLayout(horizontalLayout, 5, 1, 1, 1);

        set_database = new QPushButton(centralWidget);
        set_database->setObjectName(QStringLiteral("set_database"));
        set_database->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(set_database, 12, 2, 1, 1);

        get_image = new QPushButton(centralWidget);
        get_image->setObjectName(QStringLiteral("get_image"));
        get_image->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(get_image, 12, 1, 1, 1);

        serial_port_number = new QComboBox(centralWidget);
        serial_port_number->setObjectName(QStringLiteral("serial_port_number"));
        serial_port_number->setMinimumSize(QSize(0, 20));
        serial_port_number->setMaximumSize(QSize(90, 16777215));
        serial_port_number->setMaxVisibleItems(30);
        serial_port_number->setFrame(true);

        gridLayout_2->addWidget(serial_port_number, 0, 2, 1, 1);

        get_template = new QPushButton(centralWidget);
        get_template->setObjectName(QStringLiteral("get_template"));
        get_template->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(get_template, 9, 2, 1, 1);

        delete_all = new QPushButton(centralWidget);
        delete_all->setObjectName(QStringLiteral("delete_all"));
        delete_all->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(delete_all, 8, 2, 1, 1);

        identify_1_n = new QPushButton(centralWidget);
        identify_1_n->setObjectName(QStringLiteral("identify_1_n"));
        identify_1_n->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(identify_1_n, 8, 1, 1, 1);

        delete_id = new QPushButton(centralWidget);
        delete_id->setObjectName(QStringLiteral("delete_id"));
        delete_id->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(delete_id, 7, 2, 1, 1);

        set_template = new QPushButton(centralWidget);
        set_template->setObjectName(QStringLiteral("set_template"));
        set_template->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(set_template, 10, 2, 1, 1);

        get_database = new QPushButton(centralWidget);
        get_database->setObjectName(QStringLiteral("get_database"));
        get_database->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(get_database, 11, 2, 1, 1);

        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setMaximumSize(QSize(16777215, 5));

        gridLayout_2->addWidget(label_9, 17, 1, 1, 1);


        horizontalLayout_3->addLayout(gridLayout_2);

        MainWindowDialog->setCentralWidget(centralWidget);

        retranslateUi(MainWindowDialog);

        baudrate->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(MainWindowDialog);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowDialog)
    {
        MainWindowDialog->setWindowTitle(QApplication::translate("MainWindowDialog", "QT_SDK_DEMO", 0));
        save_image_to_file->setText(QApplication::translate("MainWindowDialog", "Save Image to File", 0));
        label_4->setText(QApplication::translate("MainWindowDialog", "Result :", 0));
        result->setText(QApplication::translate("MainWindowDialog", "-- result, line 1 --\n"
"-- result, line 2 --", 0));
        close->setText(QApplication::translate("MainWindowDialog", "Close", 0));
        verify_template->setText(QApplication::translate("MainWindowDialog", "Verify Template", 0));
        label_6->setText(QString());
        label_7->setText(QString());
        label_8->setText(QString());
        label_3->setText(QApplication::translate("MainWindowDialog", "Serial Port Number :", 0));
        open->setText(QApplication::translate("MainWindowDialog", "Open", 0));
        is_press_finger->setText(QApplication::translate("MainWindowDialog", "Is Pressed Finger", 0));
        verify_1_1->setText(QApplication::translate("MainWindowDialog", "Verify (1:1)", 0));
        label_2->setText(QApplication::translate("MainWindowDialog", "Baudrate :", 0));
        identify_template->setText(QApplication::translate("MainWindowDialog", "Identify Template", 0));
        get_user_count->setText(QApplication::translate("MainWindowDialog", "Get User Count", 0));
        baudrate->clear();
        baudrate->insertItems(0, QStringList()
         << QApplication::translate("MainWindowDialog", "9600", 0)
         << QApplication::translate("MainWindowDialog", "19200", 0)
         << QApplication::translate("MainWindowDialog", "38400", 0)
         << QApplication::translate("MainWindowDialog", "57600", 0)
         << QApplication::translate("MainWindowDialog", "115200", 0)
        );
        enroll->setText(QApplication::translate("MainWindowDialog", "Enroll", 0));
        get_raw_image->setText(QApplication::translate("MainWindowDialog", "Get Raw Image", 0));
        cancel->setText(QApplication::translate("MainWindowDialog", "Cancel", 0));
        get_live_image->setText(QApplication::translate("MainWindowDialog", "Get Live Image", 0));
        label->setText(QApplication::translate("MainWindowDialog", "ID :", 0));
        set_database->setText(QApplication::translate("MainWindowDialog", "Set Database", 0));
        get_image->setText(QApplication::translate("MainWindowDialog", "Get Image", 0));
        serial_port_number->clear();
        serial_port_number->insertItems(0, QStringList()
         << QApplication::translate("MainWindowDialog", "COM1", 0)
         << QApplication::translate("MainWindowDialog", "COM2", 0)
         << QApplication::translate("MainWindowDialog", "COM3", 0)
         << QApplication::translate("MainWindowDialog", "COM4", 0)
         << QApplication::translate("MainWindowDialog", "COM5", 0)
         << QApplication::translate("MainWindowDialog", "COM6", 0)
         << QApplication::translate("MainWindowDialog", "COM7", 0)
         << QApplication::translate("MainWindowDialog", "COM8", 0)
         << QApplication::translate("MainWindowDialog", "COM26", 0)
         << QApplication::translate("MainWindowDialog", "COM27", 0)
         << QApplication::translate("MainWindowDialog", "COM28", 0)
         << QApplication::translate("MainWindowDialog", "COM29", 0)
         << QApplication::translate("MainWindowDialog", "COM30", 0)
         << QApplication::translate("MainWindowDialog", "COM31", 0)
         << QApplication::translate("MainWindowDialog", "COM32", 0)
        );
        get_template->setText(QApplication::translate("MainWindowDialog", "Get Template", 0));
        delete_all->setText(QApplication::translate("MainWindowDialog", "Delete All", 0));
        identify_1_n->setText(QApplication::translate("MainWindowDialog", "Identify (1:N)", 0));
        delete_id->setText(QApplication::translate("MainWindowDialog", "Delete ID", 0));
        set_template->setText(QApplication::translate("MainWindowDialog", "Set Template", 0));
        get_database->setText(QApplication::translate("MainWindowDialog", "Get Database", 0));
        label_9->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindowDialog: public Ui_MainWindowDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
