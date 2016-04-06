#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    int runningTime = 0;
    ~MainWindow();

private slots:
    void on_launcherStartButton_clicked();

    void on_launcherStopButton_clicked();

    void on_launcherSpeedControl_valueChanged(int value);

    void updateTime();

private:
    QTimer timer;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
