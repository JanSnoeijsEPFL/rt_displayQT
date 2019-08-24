#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    int readNewFile();
    void parseTargetMessage(char* message);
    void computeConfidence(int sequence);
    void parseTargets(char* line);
    ~MainWindow();

public slots:
    void realtimeDataSlot();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
