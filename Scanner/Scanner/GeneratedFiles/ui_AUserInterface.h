/********************************************************************************
** Form generated from reading UI file 'AUserInterface.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUSERINTERFACE_H
#define UI_AUSERINTERFACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AUserInterfaceClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *TopLayout;
    QGroupBox *VisibleGroup;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *VisibleDefault;
    QCheckBox *VisibleHBoard;
    QCheckBox *VisibleVBoard;
    QCheckBox *VisibleLineLaser;
    QCheckBox *VisibleCursorPosition;
    QGroupBox *UpdateGroup;
    QVBoxLayout *verticalLayout_3;
    QCheckBox *UpdateHBoardBox;
    QCheckBox *UpdateVBoardBox;
    QCheckBox *UpdateLineLaserBox;
    QCheckBox *UpdateNoneBox;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QLabel *InfoHBoard;
    QLabel *InfoVBoard;
    QLabel *InfoLineLaser;
    QHBoxLayout *BotLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QCheckBox *ScanBox;
    QCheckBox *ExportBox;

    void setupUi(QWidget *AUserInterfaceClass)
    {
        if (AUserInterfaceClass->objectName().isEmpty())
            AUserInterfaceClass->setObjectName(QString::fromUtf8("AUserInterfaceClass"));
        AUserInterfaceClass->resize(725, 321);
        verticalLayout = new QVBoxLayout(AUserInterfaceClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        TopLayout = new QHBoxLayout();
        TopLayout->setSpacing(6);
        TopLayout->setObjectName(QString::fromUtf8("TopLayout"));
        VisibleGroup = new QGroupBox(AUserInterfaceClass);
        VisibleGroup->setObjectName(QString::fromUtf8("VisibleGroup"));
        VisibleGroup->setAlignment(Qt::AlignCenter);
        VisibleGroup->setCheckable(false);
        verticalLayout_2 = new QVBoxLayout(VisibleGroup);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        VisibleDefault = new QCheckBox(VisibleGroup);
        VisibleDefault->setObjectName(QString::fromUtf8("VisibleDefault"));

        verticalLayout_2->addWidget(VisibleDefault);

        VisibleHBoard = new QCheckBox(VisibleGroup);
        VisibleHBoard->setObjectName(QString::fromUtf8("VisibleHBoard"));

        verticalLayout_2->addWidget(VisibleHBoard);

        VisibleVBoard = new QCheckBox(VisibleGroup);
        VisibleVBoard->setObjectName(QString::fromUtf8("VisibleVBoard"));

        verticalLayout_2->addWidget(VisibleVBoard);

        VisibleLineLaser = new QCheckBox(VisibleGroup);
        VisibleLineLaser->setObjectName(QString::fromUtf8("VisibleLineLaser"));

        verticalLayout_2->addWidget(VisibleLineLaser);

        VisibleCursorPosition = new QCheckBox(VisibleGroup);
        VisibleCursorPosition->setObjectName(QString::fromUtf8("VisibleCursorPosition"));

        verticalLayout_2->addWidget(VisibleCursorPosition);


        TopLayout->addWidget(VisibleGroup);

        UpdateGroup = new QGroupBox(AUserInterfaceClass);
        UpdateGroup->setObjectName(QString::fromUtf8("UpdateGroup"));
        UpdateGroup->setAlignment(Qt::AlignCenter);
        verticalLayout_3 = new QVBoxLayout(UpdateGroup);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        UpdateHBoardBox = new QCheckBox(UpdateGroup);
        UpdateHBoardBox->setObjectName(QString::fromUtf8("UpdateHBoardBox"));

        verticalLayout_3->addWidget(UpdateHBoardBox);

        UpdateVBoardBox = new QCheckBox(UpdateGroup);
        UpdateVBoardBox->setObjectName(QString::fromUtf8("UpdateVBoardBox"));

        verticalLayout_3->addWidget(UpdateVBoardBox);

        UpdateLineLaserBox = new QCheckBox(UpdateGroup);
        UpdateLineLaserBox->setObjectName(QString::fromUtf8("UpdateLineLaserBox"));

        verticalLayout_3->addWidget(UpdateLineLaserBox);

        UpdateNoneBox = new QCheckBox(UpdateGroup);
        UpdateNoneBox->setObjectName(QString::fromUtf8("UpdateNoneBox"));

        verticalLayout_3->addWidget(UpdateNoneBox);


        TopLayout->addWidget(UpdateGroup);

        groupBox_2 = new QGroupBox(AUserInterfaceClass);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setAlignment(Qt::AlignCenter);
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        InfoHBoard = new QLabel(groupBox_2);
        InfoHBoard->setObjectName(QString::fromUtf8("InfoHBoard"));

        verticalLayout_4->addWidget(InfoHBoard);

        InfoVBoard = new QLabel(groupBox_2);
        InfoVBoard->setObjectName(QString::fromUtf8("InfoVBoard"));

        verticalLayout_4->addWidget(InfoVBoard);

        InfoLineLaser = new QLabel(groupBox_2);
        InfoLineLaser->setObjectName(QString::fromUtf8("InfoLineLaser"));

        verticalLayout_4->addWidget(InfoLineLaser);


        TopLayout->addWidget(groupBox_2);


        verticalLayout->addLayout(TopLayout);

        BotLayout = new QHBoxLayout();
        BotLayout->setSpacing(6);
        BotLayout->setObjectName(QString::fromUtf8("BotLayout"));
        groupBox = new QGroupBox(AUserInterfaceClass);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setAlignment(Qt::AlignCenter);
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        ScanBox = new QCheckBox(groupBox);
        ScanBox->setObjectName(QString::fromUtf8("ScanBox"));

        horizontalLayout->addWidget(ScanBox);

        ExportBox = new QCheckBox(groupBox);
        ExportBox->setObjectName(QString::fromUtf8("ExportBox"));

        horizontalLayout->addWidget(ExportBox);


        BotLayout->addWidget(groupBox);


        verticalLayout->addLayout(BotLayout);


        retranslateUi(AUserInterfaceClass);

        QMetaObject::connectSlotsByName(AUserInterfaceClass);
    } // setupUi

    void retranslateUi(QWidget *AUserInterfaceClass)
    {
        AUserInterfaceClass->setWindowTitle(QApplication::translate("AUserInterfaceClass", "AUserInterface", nullptr));
        VisibleGroup->setTitle(QApplication::translate("AUserInterfaceClass", "Visible", nullptr));
        VisibleDefault->setText(QApplication::translate("AUserInterfaceClass", "Default", nullptr));
        VisibleHBoard->setText(QApplication::translate("AUserInterfaceClass", "HBoard", nullptr));
        VisibleVBoard->setText(QApplication::translate("AUserInterfaceClass", "VBoard", nullptr));
        VisibleLineLaser->setText(QApplication::translate("AUserInterfaceClass", "Line Laser", nullptr));
        VisibleCursorPosition->setText(QApplication::translate("AUserInterfaceClass", "Cursor Position", nullptr));
        UpdateGroup->setTitle(QApplication::translate("AUserInterfaceClass", "Update", nullptr));
        UpdateHBoardBox->setText(QApplication::translate("AUserInterfaceClass", "Update H Board", nullptr));
        UpdateVBoardBox->setText(QApplication::translate("AUserInterfaceClass", "Update V Board", nullptr));
        UpdateLineLaserBox->setText(QApplication::translate("AUserInterfaceClass", "Update Line Laser", nullptr));
        UpdateNoneBox->setText(QApplication::translate("AUserInterfaceClass", "Update None", nullptr));
        groupBox_2->setTitle(QApplication::translate("AUserInterfaceClass", "Information", nullptr));
        InfoHBoard->setText(QApplication::translate("AUserInterfaceClass", "H Board", nullptr));
        InfoVBoard->setText(QApplication::translate("AUserInterfaceClass", "V Board", nullptr));
        InfoLineLaser->setText(QApplication::translate("AUserInterfaceClass", "Line Laser", nullptr));
        groupBox->setTitle(QApplication::translate("AUserInterfaceClass", "Scan", nullptr));
        ScanBox->setText(QApplication::translate("AUserInterfaceClass", "Scan", nullptr));
        ExportBox->setText(QApplication::translate("AUserInterfaceClass", "Export", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AUserInterfaceClass: public Ui_AUserInterfaceClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUSERINTERFACE_H
