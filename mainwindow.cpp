#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    srand(0);
    ui->setupUi(this);
    ui->lWindowImage->setScaledContents(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//loading image
void MainWindow::on_pbLoad_clicked()
{
    ui->statusBar->showMessage("loading");
    fileName = QFileDialog::getOpenFileName(this,"choose a file","/home/pavlenty/QtProgects/GIIT_1/img/","*.jpg");
    if(!fileName.isNull()){
        loadedImage = cv::imread(fileName.toStdString());
        cv::imshow("loaded image", loadedImage);
        //ui->firstImage->setPixmap(QPixmap::fromImage(QImage((unsigned char*) inputImage.data, inputImage.cols, inputImage.rows, QImage::Format_RGB888)));
    }
    ui->statusBar->showMessage("");
}
//processing of noised image
void MainWindow::on_pbProcess_clicked()
{
    if(noisedImage.empty()){
        ui->statusBar->showMessage("noised image isn't exist!",1000);
        return;
    }

    ui->statusBar->showMessage("processing");
    setCursor(Qt::BusyCursor);

    int windowNumbersWidth = ui->lWindowWidth->text().toInt();
    int windowNumbersHeight = ui->lWindowHeight->text().toInt();
    int windowWidthNumber = ui->lWindowWidthNumber->text().toInt();
    int windowHeightNumber = ui->lWindowHeightNumber->text().toInt();
    int windowXKoordinate = ui->sbXKoordinate->value();
    int windowYKoordinate = ui->sbYKoordinate->value();

    int dX = windowXKoordinate - (windowWidthNumber - 1);
    int dY = windowYKoordinate - (windowHeightNumber - 1);


    //creating processed image
    int cols = noisedImage.cols;
    int rows = noisedImage.rows;
    noisedImage.copyTo(processedImage);
    processedImage.rows = rows;
    processedImage.cols = cols;

    int sequence[windowNumbersWidth + windowNumbersHeight][3];
    //creating counters
    int k = 0;
    int i = 0;
    int j = 0;
    //creating rgb for two pixels
    int red1, green1, blue1, red2, green2, blue2;
    //core of our processing
    for(int x = 0; x < rows; x++)
        for(int y = 0; y < cols; y++){
            k = 0;
            //take horisontal pixels from window in sequence
            j = y - dY;
            for(i = x - windowXKoordinate; i < x - windowXKoordinate + windowNumbersWidth; i++, k++)
                if(i >= rows || i <= -1 || j >= cols || j <= -1){
                    sequence[k][0] = noisedImage.at<Vec3b>(x,y)[0];
                    sequence[k][1] = noisedImage.at<Vec3b>(x,y)[1];
                    sequence[k][2] = noisedImage.at<Vec3b>(x,y)[2];
                }
                else{
                    sequence[k][0] = noisedImage.at<Vec3b>(i,j)[0];
                    sequence[k][1] = noisedImage.at<Vec3b>(i,j)[1];
                    sequence[k][2] = noisedImage.at<Vec3b>(i,j)[2];
                }
            //take vertical pixels from window in sequence
            i = x - dX;
            for(j = y - windowYKoordinate; j < y - windowYKoordinate + windowNumbersHeight; j++, k++)
                if(i >= rows || i <= -1 || j >= cols || j <= -1){
                    sequence[k][0] = noisedImage.at<Vec3b>(x,y)[0];
                    sequence[k][1] = noisedImage.at<Vec3b>(x,y)[1];
                    sequence[k][2] = noisedImage.at<Vec3b>(x,y)[2];
                }
                else{
                    sequence[k][0] = noisedImage.at<Vec3b>(i,j)[0];
                    sequence[k][1] = noisedImage.at<Vec3b>(i,j)[1];
                    sequence[k][2] = noisedImage.at<Vec3b>(i,j)[2];
                }
            //sorting sequence with booble method
            for(i = 1; i < windowNumbersWidth + windowNumbersHeight; i++)
                for(j = 1; j < windowNumbersWidth + windowNumbersHeight; j++){

                    red1    = sequence[j-1][0];
                    green1  = sequence[j-1][1];
                    blue1   = sequence[j-1][2];
                    red2    = sequence[j][0];
                    green2  = sequence[j][1];
                    blue2   = sequence[j][2];

                    if(red1 + green1 + blue1 > red2 + green2 + blue2){
                        sequence[j-1][0]    = red2;
                        sequence[j-1][1]    = green2;
                        sequence[j-1][2]    = blue2;
                        sequence[j][0]      = red1;
                        sequence[j][1]      = green1;
                        sequence[j][2]      = blue1;
                    }
                }

            processedImage.at<Vec3b>(x,y)[0] = sequence[(windowNumbersWidth + windowNumbersHeight) / 2][0];
            processedImage.at<Vec3b>(x,y)[1] = sequence[(windowNumbersWidth + windowNumbersHeight) / 2][1];
            processedImage.at<Vec3b>(x,y)[2] = sequence[(windowNumbersWidth + windowNumbersHeight) / 2][2];
        }
    cv::imshow("processed image", processedImage);
    setCursor(Qt::ArrowCursor);
    ui->statusBar->showMessage("");
}
//add noise pixels in loaded image
void MainWindow::on_pbNoise_clicked()
{
    if(loadedImage.empty()){
        ui->statusBar->showMessage("loaded image isn't exist!",1000);
        return;
    }
    ui->statusBar->showMessage("noising");
    int num = ui->verticalSlider->value();
    int cols = loadedImage.cols;
    int rows = loadedImage.rows;
    loadedImage.copyTo(noisedImage);

    int sb1 = ui->spinBox->value();

    int x = 0, y = 0;

    for(int i = 0; i < num; i++){

        x = rand()%rows;
        y = rand()%cols;
        for(int x_t = 0; x_t < sb1; x_t++)
            for(int y_t = 0; y_t < sb1; y_t++){
                if(x + x_t >= rows || x + x_t <= -1 || y + y_t >= cols || y + y_t <= -1)
                    continue;
                noisedImage.at<Vec3b>(x + x_t, y + y_t)[0] = 255;
                noisedImage.at<Vec3b>(x + x_t, y + y_t)[1] = 255;
                noisedImage.at<Vec3b>(x + x_t, y + y_t)[2] = 255;
        }
    }
    cv::imshow("noised image", noisedImage);
    ui->statusBar->showMessage("");
}
//status bar updating
void MainWindow::on_verticalSlider_sliderPressed()
{
    int val = ui->verticalSlider->value();
    ui->statusBar->showMessage("slider value: " + QString::number(val));
}
//status bar updating
void MainWindow::on_verticalSlider_sliderReleased()
{
    ui->statusBar->showMessage("");
}
//status bar updating
void MainWindow::on_verticalSlider_sliderMoved(int position)
{
    ui->statusBar->showMessage("slider value: " + QString::number(position));
}
//status bar, label and limits updating
void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    int windowImageHeight = 2 * arg1 - 1;
    ui->lWindowHeight->setText(QString::number(windowImageHeight));
    ui->statusBar->showMessage("window height: " + QString::number(windowImageHeight), 1000);
    ui->vsWindowNumberHeight->setMaximum(windowImageHeight);
    ui->sbYKoordinate->setMaximum(windowImageHeight - 1);
}

//status bar, label and limits updating
void MainWindow::on_spinBox_3_valueChanged(int arg1)
{
    int windowImageWidth = 2 * arg1 - 1;
    ui->lWindowWidth->setText(QString::number(windowImageWidth));
    ui->statusBar->showMessage("window height: " + QString::number(windowImageWidth), 1000);
    ui->hsWindowNumberWidth->setMaximum(windowImageWidth);
    ui->sbXKoordinate->setMaximum(windowImageWidth - 1);
}
//show skelet of the window
void MainWindow::on_pbFreshImage_clicked()
{
    int windowImageWidth  = 100;
    int windowImageHeight = 100;
    int windowNumbersWidth  = ui->lWindowWidth->text().toInt();
    int windowNumbersHeight = ui->lWindowHeight->text().toInt();
    int windowWidthNumber   = ui->lWindowWidthNumber->text().toInt();
    int windowHeightNumber  = ui->lWindowHeightNumber->text().toInt();
    int windowXKoordinate   = ui->sbXKoordinate->value();
    int windowYKoordinate   = ui->sbYKoordinate->value();

    windowImage = new QImage(windowImageWidth, windowImageHeight, QImage::Format_RGBA8888);
    //pixels drawing
    for(int i = 0 ; i < windowImageWidth; i++)
        for(int j = 0; j < windowImageHeight; j++)
            if(i > windowXKoordinate       *   windowImageWidth  / windowNumbersWidth  &&
                    i < (windowXKoordinate + 1) *   windowImageWidth  / windowNumbersWidth  &&
                    j > windowYKoordinate       *   windowImageHeight / windowNumbersHeight &&
                    j < (windowYKoordinate + 1) *   windowImageHeight / windowNumbersHeight)
                windowImage->setPixel(i,j,QColor(255,0,0).rgb());
            else if(i > (windowWidthNumber - 1) *   windowImageWidth  / windowNumbersWidth  &&
                    i < windowWidthNumber       *   windowImageWidth  / windowNumbersWidth)
                windowImage->setPixel(i,j,QColor(0,255,0).rgb());
            else if(j >(windowHeightNumber - 1) *   windowImageHeight / windowNumbersHeight &&
                    j < windowHeightNumber      *   windowImageHeight / windowNumbersHeight)
                windowImage->setPixel(i,j,QColor(0,255,0).rgb());
            else
                windowImage->setPixel(i,j,QColor(255,255,255).rgb());
    //grid drawing
    for(int i = 0; i < windowImageWidth; i++)
        for(int j = 0 ; j < windowNumbersHeight; j++)
            windowImage->setPixel(i,j * windowImageHeight / windowNumbersHeight,QColor(0,0,0).rgb());
    for(int j = 0; j < windowImageHeight; j++)
        for(int i = 0 ; i < windowNumbersWidth; i++)
            windowImage->setPixel(i * windowImageWidth  / windowNumbersWidth,j,QColor(0,0,0).rgb());
    ui->lWindowImage->setPixmap(QPixmap::fromImage(*windowImage));
}
// status bar and label updating
void MainWindow::on_hsWindowNumberWidth_valueChanged(int value)
{
    ui->statusBar->showMessage("window width num: " + QString::number(value), 1000);
    ui->lWindowWidthNumber->setText(QString::number(value));
}
// status bar and label updating
void MainWindow::on_vsWindowNumberHeight_valueChanged(int value)
{
    ui->statusBar->showMessage("window height num: " + QString::number(value), 1000);
    ui->lWindowHeightNumber->setText(QString::number(value));
}
