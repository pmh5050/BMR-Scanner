/********************************************************************************
** Form generated from reading UI file 'AUserInterface.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUSERINTERFACE_H
#define UI_AUSERINTERFACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AUserInterfaceClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *LabelRGBFrame;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *LabelLaserFrame;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout;
    QLabel *SCPathText;
    QLabel *CCPathText;
    QPushButton *SCUpdateButton;
    QPushButton *CCLoadButton;
    QPushButton *SCLoadButton;
    QPushButton *CCUpdateButton;
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
    QGroupBox *InformationGroup;
    QVBoxLayout *verticalLayout_4;
    QLabel *InfoHBoard;
    QLabel *InfoVBoard;
    QLabel *InfoLineLaser;
    QPushButton *SCSaveButton;
    QGroupBox *SerialGroup;
    QVBoxLayout *verticalLayout_5;
    QLabel *SerialStateLabel;
    QHBoxLayout *ComSelectLayout;
    QSpinBox *SerialChannel;
    QPushButton *ConnectButton;
    QPushButton *UnitStepButton;
    QHBoxLayout *BotLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QSpinBox *RThSpinBox;
    QSlider *LimXYSlider;
    QSlider *LimZSlider;
    QLabel *LimXYText;
    QLabel *label_8;
    QSpinBox *WSizeSpinBox;
    QLabel *LimZText;
    QLabel *label_4;
    QLabel *label_3;
    QLabel *label_7;
    QLabel *label_2;
    QSlider *OdoCCSlider;
    QSlider *MeaCCSlider;
    QLabel *OdoCCText;
    QLabel *MeaCCText;
    QGridLayout *gridLayout_4;
    QCheckBox *ExportBox;
    QPushButton *ScanDataClearButton;
    QCheckBox *ScanBox;
    QPushButton *ScanProcessingButton;

    void setupUi(QWidget *AUserInterfaceClass)
    {
        if (AUserInterfaceClass->objectName().isEmpty())
            AUserInterfaceClass->setObjectName(QString::fromUtf8("AUserInterfaceClass"));
        AUserInterfaceClass->resize(978, 658);
        verticalLayout = new QVBoxLayout(AUserInterfaceClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox_2 = new QGroupBox(AUserInterfaceClass);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setAlignment(Qt::AlignCenter);
        horizontalLayout_3 = new QHBoxLayout(groupBox_2);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        LabelRGBFrame = new QLabel(groupBox_2);
        LabelRGBFrame->setObjectName(QString::fromUtf8("LabelRGBFrame"));
        LabelRGBFrame->setMinimumSize(QSize(256, 192));
        LabelRGBFrame->setMaximumSize(QSize(256, 192));

        horizontalLayout_3->addWidget(LabelRGBFrame);


        horizontalLayout_2->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(AUserInterfaceClass);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setAlignment(Qt::AlignCenter);
        horizontalLayout_4 = new QHBoxLayout(groupBox_3);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        LabelLaserFrame = new QLabel(groupBox_3);
        LabelLaserFrame->setObjectName(QString::fromUtf8("LabelLaserFrame"));
        LabelLaserFrame->setMinimumSize(QSize(256, 192));
        LabelLaserFrame->setMaximumSize(QSize(256, 192));

        horizontalLayout_4->addWidget(LabelLaserFrame);


        horizontalLayout_2->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(AUserInterfaceClass);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setAlignment(Qt::AlignCenter);
        gridLayout = new QGridLayout(groupBox_4);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        SCPathText = new QLabel(groupBox_4);
        SCPathText->setObjectName(QString::fromUtf8("SCPathText"));

        gridLayout->addWidget(SCPathText, 1, 1, 1, 1);

        CCPathText = new QLabel(groupBox_4);
        CCPathText->setObjectName(QString::fromUtf8("CCPathText"));

        gridLayout->addWidget(CCPathText, 0, 1, 1, 1);

        SCUpdateButton = new QPushButton(groupBox_4);
        SCUpdateButton->setObjectName(QString::fromUtf8("SCUpdateButton"));
        SCUpdateButton->setMaximumSize(QSize(60, 16777215));

        gridLayout->addWidget(SCUpdateButton, 1, 3, 1, 1);

        CCLoadButton = new QPushButton(groupBox_4);
        CCLoadButton->setObjectName(QString::fromUtf8("CCLoadButton"));
        CCLoadButton->setMaximumSize(QSize(60, 16777215));

        gridLayout->addWidget(CCLoadButton, 0, 2, 1, 1);

        SCLoadButton = new QPushButton(groupBox_4);
        SCLoadButton->setObjectName(QString::fromUtf8("SCLoadButton"));
        SCLoadButton->setMaximumSize(QSize(60, 16777215));

        gridLayout->addWidget(SCLoadButton, 1, 2, 1, 1);

        CCUpdateButton = new QPushButton(groupBox_4);
        CCUpdateButton->setObjectName(QString::fromUtf8("CCUpdateButton"));
        CCUpdateButton->setMaximumSize(QSize(60, 16777215));

        gridLayout->addWidget(CCUpdateButton, 0, 3, 1, 1);


        horizontalLayout_2->addWidget(groupBox_4);


        verticalLayout->addLayout(horizontalLayout_2);

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

        InformationGroup = new QGroupBox(AUserInterfaceClass);
        InformationGroup->setObjectName(QString::fromUtf8("InformationGroup"));
        InformationGroup->setAlignment(Qt::AlignCenter);
        verticalLayout_4 = new QVBoxLayout(InformationGroup);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        InfoHBoard = new QLabel(InformationGroup);
        InfoHBoard->setObjectName(QString::fromUtf8("InfoHBoard"));

        verticalLayout_4->addWidget(InfoHBoard);

        InfoVBoard = new QLabel(InformationGroup);
        InfoVBoard->setObjectName(QString::fromUtf8("InfoVBoard"));

        verticalLayout_4->addWidget(InfoVBoard);

        InfoLineLaser = new QLabel(InformationGroup);
        InfoLineLaser->setObjectName(QString::fromUtf8("InfoLineLaser"));

        verticalLayout_4->addWidget(InfoLineLaser);

        SCSaveButton = new QPushButton(InformationGroup);
        SCSaveButton->setObjectName(QString::fromUtf8("SCSaveButton"));
        SCSaveButton->setMaximumSize(QSize(170, 16777215));

        verticalLayout_4->addWidget(SCSaveButton);


        TopLayout->addWidget(InformationGroup);

        SerialGroup = new QGroupBox(AUserInterfaceClass);
        SerialGroup->setObjectName(QString::fromUtf8("SerialGroup"));
        SerialGroup->setAlignment(Qt::AlignCenter);
        verticalLayout_5 = new QVBoxLayout(SerialGroup);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        SerialStateLabel = new QLabel(SerialGroup);
        SerialStateLabel->setObjectName(QString::fromUtf8("SerialStateLabel"));
        SerialStateLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(SerialStateLabel);

        ComSelectLayout = new QHBoxLayout();
        ComSelectLayout->setSpacing(6);
        ComSelectLayout->setObjectName(QString::fromUtf8("ComSelectLayout"));
        SerialChannel = new QSpinBox(SerialGroup);
        SerialChannel->setObjectName(QString::fromUtf8("SerialChannel"));
        SerialChannel->setAlignment(Qt::AlignCenter);
        SerialChannel->setValue(3);

        ComSelectLayout->addWidget(SerialChannel);

        ConnectButton = new QPushButton(SerialGroup);
        ConnectButton->setObjectName(QString::fromUtf8("ConnectButton"));

        ComSelectLayout->addWidget(ConnectButton);


        verticalLayout_5->addLayout(ComSelectLayout);

        UnitStepButton = new QPushButton(SerialGroup);
        UnitStepButton->setObjectName(QString::fromUtf8("UnitStepButton"));

        verticalLayout_5->addWidget(UnitStepButton);


        TopLayout->addWidget(SerialGroup);


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
        groupBox_5 = new QGroupBox(groupBox);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setMaximumSize(QSize(600, 16777215));
        groupBox_5->setAlignment(Qt::AlignCenter);
        gridLayout_2 = new QGridLayout(groupBox_5);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label = new QLabel(groupBox_5);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(label, 4, 0, 1, 1);

        RThSpinBox = new QSpinBox(groupBox_5);
        RThSpinBox->setObjectName(QString::fromUtf8("RThSpinBox"));
        RThSpinBox->setMaximumSize(QSize(150, 16777215));
        RThSpinBox->setMinimum(-255);
        RThSpinBox->setMaximum(255);
        RThSpinBox->setValue(0);

        gridLayout_2->addWidget(RThSpinBox, 0, 1, 1, 1);

        LimXYSlider = new QSlider(groupBox_5);
        LimXYSlider->setObjectName(QString::fromUtf8("LimXYSlider"));
        LimXYSlider->setMaximumSize(QSize(300, 16777215));
        LimXYSlider->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(LimXYSlider, 2, 1, 1, 1);

        LimZSlider = new QSlider(groupBox_5);
        LimZSlider->setObjectName(QString::fromUtf8("LimZSlider"));
        LimZSlider->setMaximumSize(QSize(300, 16777215));
        LimZSlider->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(LimZSlider, 3, 1, 1, 1);

        LimXYText = new QLabel(groupBox_5);
        LimXYText->setObjectName(QString::fromUtf8("LimXYText"));
        LimXYText->setMaximumSize(QSize(50, 16777215));

        gridLayout_2->addWidget(LimXYText, 2, 2, 1, 1);

        label_8 = new QLabel(groupBox_5);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(label_8, 1, 0, 1, 1);

        WSizeSpinBox = new QSpinBox(groupBox_5);
        WSizeSpinBox->setObjectName(QString::fromUtf8("WSizeSpinBox"));
        WSizeSpinBox->setMaximumSize(QSize(150, 16777215));
        WSizeSpinBox->setMinimum(1);
        WSizeSpinBox->setMaximum(32);

        gridLayout_2->addWidget(WSizeSpinBox, 1, 1, 1, 1);

        LimZText = new QLabel(groupBox_5);
        LimZText->setObjectName(QString::fromUtf8("LimZText"));
        LimZText->setMaximumSize(QSize(50, 16777215));

        gridLayout_2->addWidget(LimZText, 3, 2, 1, 1);

        label_4 = new QLabel(groupBox_5);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(label_4, 2, 0, 1, 1);

        label_3 = new QLabel(groupBox_5);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(label_3, 3, 0, 1, 1);

        label_7 = new QLabel(groupBox_5);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(0, 0));
        label_7->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(label_7, 0, 0, 1, 1);

        label_2 = new QLabel(groupBox_5);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(label_2, 5, 0, 1, 1);

        OdoCCSlider = new QSlider(groupBox_5);
        OdoCCSlider->setObjectName(QString::fromUtf8("OdoCCSlider"));
        OdoCCSlider->setMaximumSize(QSize(300, 16777215));
        OdoCCSlider->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(OdoCCSlider, 4, 1, 1, 1);

        MeaCCSlider = new QSlider(groupBox_5);
        MeaCCSlider->setObjectName(QString::fromUtf8("MeaCCSlider"));
        MeaCCSlider->setMaximumSize(QSize(300, 16777215));
        MeaCCSlider->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(MeaCCSlider, 5, 1, 1, 1);

        OdoCCText = new QLabel(groupBox_5);
        OdoCCText->setObjectName(QString::fromUtf8("OdoCCText"));
        OdoCCText->setMaximumSize(QSize(50, 16777215));

        gridLayout_2->addWidget(OdoCCText, 4, 2, 1, 1);

        MeaCCText = new QLabel(groupBox_5);
        MeaCCText->setObjectName(QString::fromUtf8("MeaCCText"));
        MeaCCText->setMaximumSize(QSize(50, 16777215));

        gridLayout_2->addWidget(MeaCCText, 5, 2, 1, 1);


        horizontalLayout->addWidget(groupBox_5);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        ExportBox = new QCheckBox(groupBox);
        ExportBox->setObjectName(QString::fromUtf8("ExportBox"));
        ExportBox->setMaximumSize(QSize(60, 16777215));

        gridLayout_4->addWidget(ExportBox, 1, 0, 1, 1);

        ScanDataClearButton = new QPushButton(groupBox);
        ScanDataClearButton->setObjectName(QString::fromUtf8("ScanDataClearButton"));
        ScanDataClearButton->setMaximumSize(QSize(50, 16777215));

        gridLayout_4->addWidget(ScanDataClearButton, 0, 1, 1, 1);

        ScanBox = new QCheckBox(groupBox);
        ScanBox->setObjectName(QString::fromUtf8("ScanBox"));
        ScanBox->setMaximumSize(QSize(60, 16777215));

        gridLayout_4->addWidget(ScanBox, 0, 0, 1, 1);

        ScanProcessingButton = new QPushButton(groupBox);
        ScanProcessingButton->setObjectName(QString::fromUtf8("ScanProcessingButton"));
        ScanProcessingButton->setMaximumSize(QSize(80, 16777215));

        gridLayout_4->addWidget(ScanProcessingButton, 0, 2, 1, 1);


        horizontalLayout->addLayout(gridLayout_4);


        BotLayout->addWidget(groupBox);


        verticalLayout->addLayout(BotLayout);


        retranslateUi(AUserInterfaceClass);

        QMetaObject::connectSlotsByName(AUserInterfaceClass);
    } // setupUi

    void retranslateUi(QWidget *AUserInterfaceClass)
    {
        AUserInterfaceClass->setWindowTitle(QApplication::translate("AUserInterfaceClass", "LLScanner", nullptr));
        groupBox_2->setTitle(QApplication::translate("AUserInterfaceClass", "RGB Frame", nullptr));
        LabelRGBFrame->setText(QApplication::translate("AUserInterfaceClass", "RGB Frame", nullptr));
        groupBox_3->setTitle(QApplication::translate("AUserInterfaceClass", "Laser Frame", nullptr));
        LabelLaserFrame->setText(QApplication::translate("AUserInterfaceClass", "Laser Frame", nullptr));
        groupBox_4->setTitle(QApplication::translate("AUserInterfaceClass", "Calibration File System", nullptr));
        SCPathText->setText(QApplication::translate("AUserInterfaceClass", "SC Path", nullptr));
        CCPathText->setText(QApplication::translate("AUserInterfaceClass", "CC Path", nullptr));
        SCUpdateButton->setText(QApplication::translate("AUserInterfaceClass", "Update", nullptr));
        CCLoadButton->setText(QApplication::translate("AUserInterfaceClass", "CC Load", nullptr));
        SCLoadButton->setText(QApplication::translate("AUserInterfaceClass", "SC Load", nullptr));
        CCUpdateButton->setText(QApplication::translate("AUserInterfaceClass", "Update", nullptr));
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
        InformationGroup->setTitle(QApplication::translate("AUserInterfaceClass", "Information", nullptr));
        InfoHBoard->setText(QApplication::translate("AUserInterfaceClass", "H Board", nullptr));
        InfoVBoard->setText(QApplication::translate("AUserInterfaceClass", "V Board", nullptr));
        InfoLineLaser->setText(QApplication::translate("AUserInterfaceClass", "Line Laser", nullptr));
        SCSaveButton->setText(QApplication::translate("AUserInterfaceClass", "Save Calibration Data", nullptr));
        SerialGroup->setTitle(QApplication::translate("AUserInterfaceClass", "Serial", nullptr));
        SerialStateLabel->setText(QApplication::translate("AUserInterfaceClass", "Connection dosen't exist", nullptr));
        ConnectButton->setText(QApplication::translate("AUserInterfaceClass", "Connect", nullptr));
        UnitStepButton->setText(QApplication::translate("AUserInterfaceClass", "Unit Step", nullptr));
        groupBox->setTitle(QApplication::translate("AUserInterfaceClass", "Scan", nullptr));
        groupBox_5->setTitle(QApplication::translate("AUserInterfaceClass", "Hyper Parameter", nullptr));
        label->setText(QApplication::translate("AUserInterfaceClass", "Odometry CC", nullptr));
        LimXYText->setText(QApplication::translate("AUserInterfaceClass", "0", nullptr));
        label_8->setText(QApplication::translate("AUserInterfaceClass", "Window Size", nullptr));
        LimZText->setText(QApplication::translate("AUserInterfaceClass", "0", nullptr));
        label_4->setText(QApplication::translate("AUserInterfaceClass", "Limit XY", nullptr));
        label_3->setText(QApplication::translate("AUserInterfaceClass", "Limit Z", nullptr));
        label_7->setText(QApplication::translate("AUserInterfaceClass", "Red Threshold", nullptr));
        label_2->setText(QApplication::translate("AUserInterfaceClass", "Measurement CC", nullptr));
        OdoCCText->setText(QApplication::translate("AUserInterfaceClass", "0", nullptr));
        MeaCCText->setText(QApplication::translate("AUserInterfaceClass", "0", nullptr));
        ExportBox->setText(QApplication::translate("AUserInterfaceClass", "Export", nullptr));
        ScanDataClearButton->setText(QApplication::translate("AUserInterfaceClass", "Clear", nullptr));
        ScanBox->setText(QApplication::translate("AUserInterfaceClass", "Scan", nullptr));
        ScanProcessingButton->setText(QApplication::translate("AUserInterfaceClass", "Processing", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AUserInterfaceClass: public Ui_AUserInterfaceClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUSERINTERFACE_H
