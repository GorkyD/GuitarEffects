#include "MainWindow.h"
#include "../effects/DelayEffect.h"
#include "../effects/ReverbEffect.h"
#include "../effects/OverdriveEffect.h"
#include <QMessageBox>
#include <QDebug>
#include <algorithm>
#include <QTimer>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) 
{
    qDebug() << "Starting MainWindow initialization...";
    
    setWindowIcon(QIcon(":/icons/app_icon.png"));
    
    setupUI();
    
    try {
        qDebug() << "Creating EffectManager...";
        effectManager_ = new EffectManager();
        
        qDebug() << "Adding effects...";

        effectManager_->clearEffects();
        effectManager_->addEffect(std::make_shared<DelayEffect>());
        effectManager_->addEffect(std::make_shared<ReverbEffect>());
        effectManager_->addEffect(std::make_shared<OverdriveEffect>());
        
        qDebug() << "Updating effect blocks...";
        updateEffectBlocks();
        
        qDebug() << "MainWindow initialization completed successfully";
    } catch (const std::exception& e) 
    {
        qDebug() << "Error during initialization:" << e.what();
        QMessageBox::critical(this, "Error", QString("Failed to initialize audio system: %1").arg(e.what()));
    }

    qDebug() << "MainWindow constructor finished";
}

MainWindow::~MainWindow() 
{
    if (isRunning_) 
    {
        effectManager_->stopStream();
    }
    delete effectManager_;
}

void MainWindow::setupUI() 
{
    setWindowTitle("Guitar Effects Processor");
    resize(900, 700);

    centralWidget_ = new QWidget(this);
    setCentralWidget(centralWidget_);
    mainLayout_ = new QVBoxLayout(centralWidget_);

    topLayout_ = new QHBoxLayout();
    mainLayout_->addLayout(topLayout_);

    processingModeCombo_ = new QComboBox();
    processingModeCombo_->addItem("Serial");
    processingModeCombo_->addItem("Parallel");
    topLayout_->addWidget(new QLabel("Processing Mode:"));
    topLayout_->addWidget(processingModeCombo_);

    startStopButton_ = new QPushButton("Start");
    topLayout_->addWidget(startStopButton_);

    effectsLayout_ = new QVBoxLayout();
    mainLayout_->addLayout(effectsLayout_);

    connect(processingModeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onProcessingModeChanged);
    connect(startStopButton_, &QPushButton::clicked, this, &MainWindow::onStartStop);
}

void MainWindow::updateEffectBlocks() 
{
    qDebug() << "updateEffectBlocks: start";
    clearEffectBlocks();
    effectBlocks_.clear();
    for (int i = 0; i < 3; ++i) 
    {
        qDebug() << "updateEffectBlocks: creating effect block" << i;
        createEffectBlock(i);
    }
    qDebug() << "updateEffectBlocks: end";
}

void MainWindow::clearEffectBlocks() 
{
    for (auto& block : effectBlocks_) 
    {
        if (block.groupBox) 
        {
            effectsLayout_->removeWidget(block.groupBox);
            block.groupBox->deleteLater();
        }
    }
    effectBlocks_.clear();
}

void MainWindow::createEffectBlock(int effectIndex) 
{
    qDebug() << "createEffectBlock: start for index" << effectIndex;
    auto effect = effectManager_->getEffect(effectIndex);

    if (!effect) 
    {
        qDebug() << "createEffectBlock: effect is nullptr for index" << effectIndex;
        return;
    }

    EffectBlockWidgets block;
    block.groupBox = new QGroupBox(QString::fromUtf8(effect->getName().c_str()), this);

    QVBoxLayout* vbox = new QVBoxLayout(block.groupBox);
    QHBoxLayout* top = new QHBoxLayout();

    block.enableCheckBox = new QCheckBox("Enable");
    block.enableCheckBox->setChecked(effect->isEnabled());

    connect(block.enableCheckBox, &QCheckBox::toggled, [this, effectIndex](bool checked){onEnableEffect(effectIndex, checked);});
    top->addWidget(block.enableCheckBox);

    block.upButton = new QPushButton("↑");
    block.downButton = new QPushButton("↓");
    block.upButton->setEnabled(effectIndex > 0);
    block.downButton->setEnabled(effectIndex < 2);

    connect(block.upButton, &QPushButton::clicked, [this, effectIndex](){onMoveEffectUp(effectIndex);});
    connect(block.downButton, &QPushButton::clicked, [this, effectIndex](){onMoveEffectDown(effectIndex);});

    top->addWidget(block.upButton);
    top->addWidget(block.downButton);

    vbox->addLayout(top);

    auto paramNames = effect->getParameterNames();
    qDebug() << "paramNames.size() =" << paramNames.size();

    for (size_t p = 0; p < paramNames.size(); ++p) 
    {
        if (paramNames[p].empty()) continue;
        QString paramNameQt = QString::fromUtf8(paramNames[p].c_str());
        QHBoxLayout* paramLayout = new QHBoxLayout();
        QLabel* label = new QLabel(paramNameQt);
        block.parameterLabels.push_back(label);
        paramLayout->addWidget(label);

        QSlider* slider = new QSlider(Qt::Horizontal);
        auto range = effect->getParameterRange(paramNames[p]);
        slider->setRange(range.first * 100, range.second * 100);
        float paramValue = effect->getParameter(paramNames[p]);
        slider->setValue(paramValue * 100);
        block.parameterSliders.push_back(slider);
        paramLayout->addWidget(slider);

        QLabel* value = new QLabel(QString::number(paramValue, 'f', 2));
        block.parameterValues.push_back(value);
        paramLayout->addWidget(value);

        connect(slider, &QSlider::valueChanged, [this, effectIndex, p](int val){onParameterChanged(effectIndex, p, val);});
        vbox->addLayout(paramLayout);
    }

    effectsLayout_->addWidget(block.groupBox);
    effectBlocks_.push_back(block);
    qDebug() << "createEffectBlock: end for index" << effectIndex;
}

void MainWindow::onEnableEffect(int effectIndex, bool enabled) 
{
    auto effect = effectManager_->getEffect(effectIndex);
    if (effect) effect->setEnabled(enabled);
}

void MainWindow::onMoveEffectUp(int effectIndex) 
{
    effectManager_->moveEffectUp(effectIndex);
    updateEffectBlocks();
}

void MainWindow::onMoveEffectDown(int effectIndex) 
{
    effectManager_->moveEffectDown(effectIndex);
    updateEffectBlocks();
}

void MainWindow::onParameterChanged(int effectIndex, int paramIndex, int value) 
{
    auto effect = effectManager_->getEffect(effectIndex);
    if (!effect) return;
    auto paramNames = effect->getParameterNames();
    if (paramIndex >= 0 && paramIndex < (int)paramNames.size()) 
    {
        float fval = value / 100.0f;
        effect->setParameter(paramNames[paramIndex], fval);
        effectBlocks_[effectIndex].parameterValues[paramIndex]->setText(QString::number(fval, 'f', 2));
    }
}

void MainWindow::onProcessingModeChanged(int mode) 
{
    effectManager_->setProcessingMode(
        mode == 0 ? EffectManager::ProcessingMode::Serial 
                  : EffectManager::ProcessingMode::Parallel);
}

void MainWindow::onStartStop() 
{
    if (!isRunning_) 
    {
        if (effectManager_->startStream()) 
        {
            isRunning_ = true;
            startStopButton_->setText("Stop");
        } else 
        {
            QMessageBox::critical(this, "Error", "Failed to start audio stream");
        }
    } else 
    {
        effectManager_->stopStream();
        isRunning_ = false;
        startStopButton_->setText("Start");
    }
} 