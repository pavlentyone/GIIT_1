#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QPixmap>
#include <QImage>
#include <QFileDialog>
#include <QColor>
#include <opencv2/opencv.hpp>
#include <cv.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_pbLoad_clicked();

    void on_pbProcess_clicked();

    void on_pbNoise_clicked();

    void on_verticalSlider_sliderPressed();

    void on_verticalSlider_sliderReleased();

    void on_verticalSlider_sliderMoved(int position);

    void on_spinBox_2_valueChanged(int arg1);

    void on_spinBox_3_valueChanged(int arg1);

    void on_pbFreshImage_clicked();

    void on_hsWindowNumberWidth_valueChanged(int value);

    void on_vsWindowNumberHeight_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QString fileName;
    cv::Mat loadedImage;
    cv::Mat noisedImage;
    cv::Mat processedImage;
    QImage *windowImage;
};

#endif // MAINWINDOW_H
