#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QLabel>
#include <QRadioButton>
#include <QSlider>
#include <QPushButton>
#include "EdaScene.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onAlgorithmChanged();
    void onObjectCountChanged(int value);
    void updateStats();

private:
    EdaScene *m_scene;
    QGraphicsView *m_view;

    QLabel *m_statusLabel;
    QLabel *m_timeLabel;
    QLabel *m_countLabel;

    QRadioButton *m_spatialRadio;
    QRadioButton *m_bruteRadio;
    QSlider *m_densitySlider;
};

#endif // MAINWINDOW_H