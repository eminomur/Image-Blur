#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "image.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_Me_triggered();

    void on_actionQuit_triggered();

    void on_apply_pushButton_clicked();

    void on_default_picture_pushButton_clicked();

    void on_actionAbout_Program_triggered();

    void on_change_picture_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Image *image;

    void image_initialization();
};

#endif // MAINWINDOW_H
