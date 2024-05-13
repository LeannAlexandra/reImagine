#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reImagineLibrary.h"

#include <QImage>

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include <QMainWindow>
#include <QWheelEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGraphicsView>
#include <QSlider>
#include <QScrollBar>

/*
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create layout for the central widget
    QVBoxLayout *centralLayout = new QVBoxLayout(ui->centralwidget);
    centralLayout->setContentsMargins(0, 0, 0, 0); // Remove margins

    // Create top-level layout for central widget
    QHBoxLayout *mainLayout = new QHBoxLayout;
    centralLayout->addLayout(mainLayout);

    // Left side layout (top and bottom quarters)
    QVBoxLayout *leftLayout = new QVBoxLayout;
    mainLayout->addLayout(leftLayout, 1); // Left side occupies 1 part

    // Add input image widget to top-left quarter
    ui->input_image->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    leftLayout->addWidget(ui->input_image);

    // Add input texture widget to bottom-left quarter
    ui->input_texture->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    leftLayout->addWidget(ui->input_texture);

    // Right side layout (output image)
    QVBoxLayout *rightLayout = new QVBoxLayout;
    mainLayout->addLayout(rightLayout, 2); // Right side occupies 2 parts (double size)

    // Add output image widget to the right side (spanning two rows)
    ui->output_image->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rightLayout->addWidget(ui->output_image);

    // Set layout spacing
    mainLayout->setSpacing(2); // Adjust spacing between sections

    // Set central layout to the central widget
    ui->centralwidget->setLayout(centralLayout);
    // Set scroll bar ranges based on image size


    // Set up signal-slot connections for synchronization
    connect(ui->input_image->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(syncScrollBars(int)));
    connect(ui->input_image->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(syncScrollBars(int)));
    connect(ui->input_image->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(syncScrollBars(int)));

    connect(ui->input_texture->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(syncScrollBars(int)));
    connect(ui->input_texture->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(syncScrollBars(int)));

    connect(ui->output_image->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(syncScrollBars(int)));
    connect(ui->output_image->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(syncScrollBars(int)));


    // Create the menu bar
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // Create the "File" menu
    QMenu *fileMenu = menuBar->addMenu(tr("&File"));

    // Create the "Exit" action
    QAction *saveAction = new QAction(tr("&Save"), this);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveOutputImage);
    fileMenu->addAction(saveAction);

    // Create the "Exit" action
    QAction *exitAction = new QAction(tr("E&xit"), this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::exitApplication);
    fileMenu->addAction(exitAction);




    // ip = reImagineLibrary();
    // Load and process images
    grabImages();
    processImages();
   //setupOutputImageView();
    updateDisplay(); // Update the display after processing images
    // zoomOutputImage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//reads the images from disk. TODO: move all disk reads and writes to its own class - to ensure the disk only gets managed by one class. (allow for async too)
void MainWindow::grabImages(){
    //inputImage = cv::imread("test_images/inputimage.jpg", cv::IMREAD_COLOR); // 3 color image.
    //textureImage = cv::imread("test_images/canvas.jpg",cv::IMREAD_GRAYSCALE); //use grayscale for texture.
    cv::Mat image = cv::imread("test_images/inputImage.jpg", cv::IMREAD_COLOR);
    if (image.empty()) {
        //qDebug() << "Failed to load image!";
        return; // Handle error
    }
    cv::Mat texture = cv::imread("test_images/canvas.jpg", cv::IMREAD_GRAYSCALE);
    inputImage= image.clone();
    textureImage=texture.clone();
}

//processes Images assigned to the global variables cv::Mat inputImage and cv::Mat inputTexture and supplies result to cv::Mat outputImage
void MainWindow::processImages(){
    // textureImage
    cv::Mat newTexture = ip.matchSize(textureImage, inputImage.cols, inputImage.rows);
    cv::Mat normText = ip.normalize(newTexture);

    //overrideglobal
    textureImage=normText.clone();



    outputImage = ip.applyImageTexture(inputImage,textureImage).clone();
}

void MainWindow::updateDisplay(){

    // Display inputImage in input_image QGraphicsView
    QImage qImage = ip.mat2Q(inputImage);
    QGraphicsScene* scene1 = new QGraphicsScene(this);
    QGraphicsPixmapItem* item1 = new QGraphicsPixmapItem(QPixmap::fromImage(qImage));
    scene1->addItem(item1);
    ui->input_image->setScene(scene1);

    QSize imageSize = ui->input_image->scene()->sceneRect().size().toSize();
    ui->input_image->horizontalScrollBar()->setRange(0, imageSize.width());
    ui->input_image->verticalScrollBar()->setRange(0, imageSize.height());

    // Display textureImage in input_texture QGraphicsView
    QImage qImage2 = ip.mat2Q(textureImage);
    QGraphicsScene* scene2 = new QGraphicsScene(this);
    QGraphicsPixmapItem* item2 = new QGraphicsPixmapItem(QPixmap::fromImage(qImage2));
    scene2->addItem(item2);
    ui->input_texture->setScene(scene2);

    //QSize imageSize = ui->input_texture->scene()->sceneRect().size().toSize();
    ui->input_texture->horizontalScrollBar()->setRange(0, imageSize.width());
    ui->input_texture->verticalScrollBar()->setRange(0, imageSize.height());

    // Display textureImage in input_texture QGraphicsView
    QImage qImage3 = ip.mat2Q(outputImage);
    QGraphicsScene* scene3 = new QGraphicsScene(this);
    QGraphicsPixmapItem* item3 = new QGraphicsPixmapItem(QPixmap::fromImage(qImage3));
    scene3->addItem(item3);
    ui->output_image->setScene(scene3);

    //QSize imageSize = ui->input_image->scene()->sceneRect().size().toSize();
    ui->output_image->horizontalScrollBar()->setRange(0, imageSize.width());
    ui->output_image->verticalScrollBar()->setRange(0, imageSize.height());
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->output_image && event->type() == QEvent::Wheel)
    {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        if (wheelEvent->modifiers() & Qt::ControlModifier)
        {
            // Zoom based on Ctrl + mouse wheel
            qreal scaleFactor = wheelEvent->angleDelta().y() > 0 ? 1.1 : 0.9;
            zoomOutputImage(scaleFactor);
            return true;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}
void MainWindow::zoomOutputImage(qreal scaleFactor)
{
    QGraphicsView *outputView = ui->output_image;
    qreal newScale = outputView->transform().m11() * scaleFactor; // Get current scale factor

    // Limit zoom range if needed
    // Example: if (newScale < minScale) newScale = minScale;
    //          if (newScale > maxScale) newScale = maxScale;

    // Apply new scale factor to the view
    outputView->setTransform(QTransform().scale(newScale, newScale));
}
void MainWindow::setupOutputImageView()
{
    QGraphicsView *outputView = ui->output_image;

    // Adjust initial scale factor for output image view
    qreal initialScaleFactor = 2.0; // Example: double the size
    outputView->setTransform(QTransform().scale(initialScaleFactor, initialScaleFactor));

    // Install event filter to capture mouse wheel events
    outputView->installEventFilter(this);
}
void MainWindow::syncImageViews(QGraphicsView *senderView)
{
    // Get the current viewport and transformation of the sender view
    QRectF senderViewportRect = senderView->viewport()->rect();
    QTransform senderTransform = senderView->transform();

    // Synchronize the viewport and transformation of the input_image view
    syncView(ui->input_image, senderViewportRect, senderTransform);

    // Synchronize the viewport and transformation of the input_texture view
    syncView(ui->input_texture, senderViewportRect, senderTransform);

    // Synchronize the viewport and transformation of the output_image view
    syncView(ui->output_image, senderViewportRect, senderTransform);
}
void MainWindow::syncView(QGraphicsView *view, const QRectF &viewportRect, const QTransform &transform)
{
    // Set the viewport rectangle and transformation of the target view
    view->viewport()->setGeometry(viewportRect.toRect());
    view->setTransform(transform);
}
// Custom slot to handle window resize
void MainWindow::handleResize() {
    // Perform any necessary calculations or updates on resize
}
void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event); // Call base class implementation

    // Emit a signal indicating that the window has been resized
    emit resized();
}
void MainWindow::syncScrollBars(int value)
{
    QScrollBar *senderBar = qobject_cast<QScrollBar*>(sender());
    if (!senderBar)
        return;

    // Determine the sender of the signal
    QGraphicsView *senderView = nullptr;
    if (senderBar == ui->input_image->horizontalScrollBar() ||
        senderBar == ui->input_image->verticalScrollBar())
    {
        senderView = ui->input_image;
    }
    else if (senderBar == ui->input_texture->horizontalScrollBar() ||
             senderBar == ui->input_texture->verticalScrollBar())
    {
        senderView = ui->input_texture;
    }
    else if (senderBar == ui->output_image->horizontalScrollBar() ||
             senderBar == ui->output_image->verticalScrollBar())
    {
        senderView = ui->output_image;
    }

    if (!senderView)
        return;

    // Synchronize horizontal scroll bar if sender is horizontal
    if (senderBar == senderView->horizontalScrollBar())
    {
        ui->input_image->horizontalScrollBar()->setValue(senderBar->value());
        ui->input_texture->horizontalScrollBar()->setValue(senderBar->value());
        ui->output_image->horizontalScrollBar()->setValue(senderBar->value());
    }

    // Synchronize vertical scroll bar if sender is vertical
    if (senderBar == senderView->verticalScrollBar())
    {
        ui->input_image->verticalScrollBar()->setValue(senderBar->value());
        ui->input_texture->verticalScrollBar()->setValue(senderBar->value());
        ui->output_image->verticalScrollBar()->setValue(senderBar->value());
    }
}
void MainWindow::exitApplication(){
      QApplication::exit();  // Quit the application peacefully
}
void MainWindow::saveOutputImage(){
      // updateDisplay();
      //cv::Mat saveImage;
      // cv::cvtColor(outputImage,saveImage, cv::COLOR_BGR2BGRA);
      //updateDisplay();
      cv::imwrite("output.jpg", outputImage);
}



