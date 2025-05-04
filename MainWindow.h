#pragma once
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QSlider>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox> 
#include <vector>
#include "EffectManager.h"

class MainWindow : public QMainWindow 
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void onEnableEffect(int effectIndex, bool enabled);
        void onMoveEffectUp(int effectIndex);
        void onMoveEffectDown(int effectIndex);
        void onParameterChanged(int effectIndex, int paramIndex, int value);
        void onProcessingModeChanged(int mode);
        void onStartStop();

    private:
        void setupUI();
        void updateEffectBlocks();
        void createEffectBlock(int effectIndex);
        void clearEffectBlocks();

        EffectManager* effectManager_;
        QWidget* centralWidget_;
        QVBoxLayout* mainLayout_;
        QHBoxLayout* topLayout_;
        QVBoxLayout* effectsLayout_;

        QComboBox* processingModeCombo_;
        QPushButton* startStopButton_;

        struct EffectBlockWidgets 
        {
            QGroupBox* groupBox;
            QCheckBox* enableCheckBox;
            QPushButton* upButton;
            QPushButton* downButton;
            std::vector<QSlider*> parameterSliders;
            std::vector<QLabel*> parameterLabels;
            std::vector<QLabel*> parameterValues;
        };

        std::vector<EffectBlockWidgets> effectBlocks_;
        bool isRunning_ = false;
}; 