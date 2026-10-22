#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("EDA Լեյաութի Window Query Սիմուլյատոր (Qt/C++)");
    resize(1050, 680);

    m_scene = new EdaScene(this);
    m_view = new QGraphicsView(m_scene, this);
    m_view->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *controlsLayout = new QVBoxLayout();

    QGroupBox *algGroup = new QGroupBox("Ալգորիթմ", this);
    QVBoxLayout *algLayout = new QVBoxLayout();
    m_spatialRadio = new QRadioButton("R-Tree / Spatial Index (O(log N + K))", this);
    m_bruteRadio = new QRadioButton("Brute Force (O(N))", this);
    m_spatialRadio->setChecked(true);
    algLayout->addWidget(m_spatialRadio);
    algLayout->addWidget(m_bruteRadio);
    algGroup->setLayout(algLayout);

    QGroupBox *densityGroup = new QGroupBox("Օբյեկտների քանակ", this);
    QVBoxLayout *densityLayout = new QVBoxLayout();
    m_densitySlider = new QSlider(Qt::Horizontal, this);
    m_densitySlider->setRange(50, 400);
    m_densitySlider->setValue(150);
    m_countLabel = new QLabel("Քանակ: 150", this);
    densityLayout->addWidget(m_countLabel);
    densityLayout->addWidget(m_densitySlider);
    densityGroup->setLayout(densityLayout);

    QGroupBox *statsGroup = new QGroupBox("Վիճակագրություն", this);
    QVBoxLayout *statsLayout = new QVBoxLayout();
    m_statusLabel = new QLabel("Ընտրված օբյեկտներ: 0", this);
    m_timeLabel = new QLabel("Հարցման ժամանակ: 0.00 ms", this);
    m_timeLabel->setStyleSheet("font-weight: bold; color: #0066cc;");
    statsLayout->addWidget(m_statusLabel);
    statsLayout->addWidget(m_timeLabel);
    statsGroup->setLayout(statsLayout);

    controlsLayout->addWidget(algGroup);
    controlsLayout->addWidget(densityGroup);
    controlsLayout->addWidget(statsGroup);
    controlsLayout->addStretch();

    QWidget *rightPanel = new QWidget(this);
    rightPanel->setLayout(controlsLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(m_view, 1);
    mainLayout->addWidget(rightPanel, 0);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    connect(m_spatialRadio, &QRadioButton::toggled, this, &MainWindow::onAlgorithmChanged);
    connect(m_densitySlider, &QSlider::valueChanged, this, &MainWindow::onObjectCountChanged);
    connect(m_scene, &EdaScene::queryCompleted, this, &MainWindow::updateStats);

    m_scene->generateLayoutObjects(150);
}

void MainWindow::onAlgorithmChanged() {
    m_scene->setUseSpatialIndex(m_spatialRadio->isChecked());
}

void MainWindow::onObjectCountChanged(int value) {
    m_countLabel->setText(QString("Քանակ: %1").arg(value));
    m_scene->generateLayoutObjects(value);
}

void MainWindow::updateStats() {
    m_statusLabel->setText(QString("Ընտրված օբյեկտներ: %1").arg(m_scene->getSelectedCount()));
    m_timeLabel->setText(QString("Հարցման ժամանակ: %1 ms").arg(QString::number(m_scene->getLastQueryTimeMs(), 'f', 3)));
}