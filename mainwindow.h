#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QWheelEvent>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <QImage>

#include "reImagineLibrary.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void resized(); // Custom signal to indicate window resize

private slots:
    void syncScrollBars(int value);
    void exitApplication();
    void saveOutputImage();

private:
    reImagineLibrary ip; // Image Processor
    Ui::MainWindow *ui;
    void handleResize();
    void resizeEvent(QResizeEvent *event);
    void grabImages();
    void processImages();
    void updateDisplay();
    bool eventFilter(QObject *watched, QEvent *event);
    void zoomOutputImage(qreal scaleFactor);
    void setupOutputImageView();
    void syncImageViews(QGraphicsView *senderView);
    void syncView(QGraphicsView *view, const QRectF &viewportRect, const QTransform &transform);
    cv::Mat inputImage;
    cv::Mat textureImage;
    cv::Mat outputImage;
    // QImage mat2Q(cv::Mat &mat);
    // cv::Mat normalize(cv::Mat &texture);
    // cv::Mat matchSize(cv::Mat &texture, int width,int height);
    // cv::Mat addTexture(cv::Mat &img, cv::Mat &textureImage, float valMargin = 0.7,
    //                 float textureMidpoint = 0.96, float blendBalance = 0.2);
};
#endif // MAINWINDOW_H
