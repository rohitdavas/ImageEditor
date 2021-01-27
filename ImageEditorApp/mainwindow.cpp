#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QKeyEvent>
#include <QDebug>
#include <QLabel>
#include <QStringList>
#include <QIcon>
#include <mainwindow.h>
#include "opencv2/opencv.hpp"
#include <QPluginLoader>

//#include <QMap>
//#include "editor_plugin_interface.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    fileMenu(nullptr),
    viewMenu(nullptr),
    currentImage(nullptr)
{
    initUI();
    loadPlugins();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initUI()
{
//    this->setMinimumSize(500, 500);
    this->resize(800, 600);

    //initialise the menu bar
    fileMenu = menuBar()->addMenu( QIcon("Icons/file.png"), "&File");
    viewMenu = menuBar()->addMenu( QIcon("Icons/view.png"), "&View");
    editMenu = menuBar()->addMenu(QIcon("Icons/edit.png"), "&Edit");

    //setup toolbar
    fileToolBar = addToolBar("File");
    viewToolBar = addToolBar("View");
    editToolBar = addToolBar("Edit");

    //image display area
    imageScene = new QGraphicsScene(this);
    imageView = new QGraphicsView(imageScene);
    setCentralWidget(imageView);

    //setup status bar
    mainStatusBar = statusBar();
    mainStatusLabel = new QLabel(mainStatusBar);
    mainStatusBar->addWidget(mainStatusLabel);
    mainStatusLabel->setText("Image information will be here!");

    //create actions
    createActions();
}

void MainWindow::createActions()
{
    // ACTIONS
    //file menu actions : open, save as, exit
    // open action
    openAction = new QAction( QIcon("Icons/open.png"), "&Open", this);
    fileMenu->addAction(openAction);

    // save as action
    saveAsAction = new QAction(QIcon("Icons/saveas.png"), "&Save as", this);
    fileMenu->addAction(saveAsAction);

    //exit action
    exitAction = new QAction(QIcon("Icons/exit.png"),"&Exit", this);
    fileMenu->addAction(exitAction);

    // toolbar actions :zoom in, zoom out, prev, next
    // zoom in
    zoomInAction = new QAction(QIcon("Icons/zoomin.png"),"&Zoom in", this);
    viewMenu->addAction(zoomInAction);

    // zoom out
    zoomOutAction = new QAction(QIcon("Icons/zoomout.png"), "&Zoom out", this);
    viewMenu->addAction(zoomOutAction);

    //pre action
    prevAction = new QAction(QIcon("Icons/previous.png"),"&previous Image", this);
    viewMenu->addAction(prevAction);

    //next action
    nextAction = new QAction(QIcon("Icons/next.png"), "&next Image", this);
    viewMenu->addAction(nextAction);

    fitToWindowAction = new QAction("Fit to window", this);
    viewMenu->addAction(fitToWindowAction);

    fileToolBar->addAction(openAction);
    fileToolBar->addAction(zoomInAction);
    fileToolBar->addAction(zoomOutAction);
    viewToolBar->addAction(prevAction);
    viewToolBar->addAction(nextAction);
    viewToolBar->addAction(fitToWindowAction);

    //  connect to the slots
    connect(exitAction, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(quit()));
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(openImage()));
    connect(saveAsAction, SIGNAL(triggered(bool)), this, SLOT(saveAs()));
    connect(zoomInAction, SIGNAL(triggered(bool)), this, SLOT(zoomIn()));
    connect(zoomOutAction, SIGNAL(triggered(bool)), this, SLOT(zoomOut()));
    connect(prevAction, SIGNAL(triggered(bool)), this, SLOT(prevImage()));
    connect(nextAction, SIGNAL(triggered(bool)), this, SLOT(nextImage()));
    connect(fitToWindowAction, SIGNAL(triggered(bool)), this, SLOT(fitImageToWindow()));
    // shortcuts
    setupShortcuts();
}

void MainWindow::openImage() {
    QFileDialog dialog (this);
    dialog.setWindowTitle("Open Image");
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Images (*.png *.bmp *.jpg)"));
    QStringList filepaths;
    if (dialog.exec()){
        filepaths = dialog.selectedFiles();
        showImage(filepaths.at(0));
    }
}

void MainWindow::showImage(QString path) {
    // clear scene and reset matrix
    imageScene->clear();
    imageView->resetMatrix();
    // read image
    QPixmap image(path);
    // add to the scene and update
    currentImage = imageScene->addPixmap(image);
    imageScene->update();
    imageView->setSceneRect(image.rect());
    QString status = QString("%1, %2x%3 %4 Bytes").arg(path).arg(image.width()).arg(image.height()).arg(QFile(path).size());
    mainStatusLabel->setText(status);
    currentImagePath = path;

    // fit to window
    fitImageToWindow(image.width(), image.height());
}

void MainWindow::showImage(QPixmap &pixmap){
    imageScene->clear();
    imageView->resetMatrix();
    currentImage = imageScene->addPixmap(pixmap);
//    qDebug() << QString("image shape : %1 mode %2 ").arg( currentImage->shape() ).arg(currentImage->shapeMode());
    imageScene->update();
    imageView->setSceneRect(pixmap.rect());

    QString status = QString("edited image, %1x%2").arg(pixmap.width()).arg(pixmap.height());
    mainStatusLabel->setText(status);
}

void MainWindow::fitImageToWindow(int image_width, int image_height){

//    qDebug() << QString("image width = %1 x %2").arg(image_width).arg(image_height);

    float zoomLevel_W = (float)this->width()/image_width;
    float zoomLevel_H = (float)this->height()/image_height;

    float zoomLevel;
    if (zoomLevel_H > zoomLevel_W)
        zoomLevel = zoomLevel_W;
    else
        zoomLevel = zoomLevel_H;
    imageView->scale(zoomLevel, zoomLevel);
}

void MainWindow::fitImageToWindow(){
    if (currentImage == nullptr){
        QMessageBox::information(this, "Information", "No image to fit");
        return;
    }

    QPixmap pixmap = currentImage->pixmap();
    showImage(pixmap);
    QImage image = pixmap.toImage();
    fitImageToWindow(image.width(), image.height());

}

void MainWindow::zoomIn() {
    imageView->scale(1.2, 1.2);
}

void MainWindow::zoomOut() {
    imageView->scale(1/1.2, 1/1.2);
}

void MainWindow::prevImage() {
    QFileInfo current(currentImagePath);
    QDir dir = current.absoluteDir();
    QStringList nameFilters = {"*.png", "*.bmp", "*.jpg"};
//    nameFilters <<"*.png" <<"*.bmp" <<"*.jpg";
    QStringList fileNames = dir.entryList(nameFilters, QDir::Files, QDir::Name);
    int idx = fileNames.indexOf(QRegExp(QRegExp::escape(current.fileName())));
    if (idx > 0){
        showImage(dir.absoluteFilePath(fileNames.at(idx-1)));
    }
//    else{
//        QMessageBox::information(this, "Information", "Current Image is the first one");
//    }
}

void MainWindow::nextImage() {
    QFileInfo current(currentImagePath);
    QDir dir = current.absoluteDir();
    QStringList nameFilters;
    nameFilters << "*.png" << "*.bmp" << "*.jpg";
    QStringList fileNames = dir.entryList(nameFilters, QDir::Files, QDir::Name);
    int idx = fileNames.indexOf(QRegExp(QRegExp::escape(current.fileName())));
    if (idx < fileNames.size() - 1){
        showImage(dir.absoluteFilePath(fileNames.at(idx+1)));
    }
//    else{
//        QMessageBox::information(this, "Information", "Current Image is the last one");
//    }
}

void MainWindow::saveAs() {
    if (currentImage == nullptr ){
        QMessageBox::information(this, "Information", "Nothing to save");
        return ;
    }

    QFileDialog dialog(this);
    dialog.setWindowTitle("Save Image As ...");
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(tr("Images (*.png *.jpg *.bmp"));
    QStringList filenames;
    if (dialog.exec()){
        filenames = dialog.selectedFiles();
        if(QRegExp(".+\\.(png|bmp|jpg)").exactMatch(filenames.at(0))){
            currentImage->pixmap().save(filenames.at(0));
        }
        else{
            QMessageBox::information(this, "Information", "Save error : bad format or filename.");
        }
    }
}

void MainWindow::setupShortcuts() {
    QList<QKeySequence> shortcuts;
    // zoom in
    shortcuts << Qt::Key_Plus << Qt::Key_Equal;
    zoomInAction->setShortcuts(shortcuts);
    shortcuts.clear();

    //zoom out
    shortcuts<<Qt::Key_Minus << Qt::Key_Underscore;
    zoomOutAction->setShortcuts(shortcuts);

    //previous
    shortcuts.clear();
    shortcuts << Qt::Key_Up << Qt::Key_Left;
    prevAction->setShortcuts(shortcuts);

    // next image
    shortcuts.clear();
    shortcuts << Qt::Key_Down << Qt::Key_Right;
    nextAction->setShortcuts(shortcuts);
}

void MainWindow::loadPlugins(){
    QDir pluginsDir(QApplication::instance()->applicationDirPath() + "/plugins");
    QStringList namefilters = {"*.so", "*.dylib", "*.dll"};
    QFileInfoList plugins = pluginsDir.entryInfoList(namefilters, QDir::NoDotAndDotDot|QDir::Files, QDir::Name);

    foreach ( QFileInfo plugin, plugins){
        QPluginLoader pluginLoader(plugin.absoluteFilePath(), this);
        EditorPluginInterface *plugin_ptr = dynamic_cast<EditorPluginInterface*>(pluginLoader.instance());

        if (plugin_ptr){
            QAction *action = new QAction(plugin_ptr->name());
            editMenu->addAction(action);
            editToolBar->addAction(action);
            editplugins[plugin_ptr->name()] = plugin_ptr;
            connect(action, SIGNAL(triggered(bool)), this, SLOT(pluginPerform()) );

        }

        else{
            qDebug()<<"bad plugin: " <<plugin.absoluteFilePath();
        }
    }

}

void MainWindow::pluginPerform(){

    if (currentImage==nullptr){
        QMessageBox::information(this, "Information", "No image to edit.");
        return;
    }

    QAction *active_action = qobject_cast<QAction*>(sender());
    EditorPluginInterface *plugin_ptr = editplugins[active_action->text()];
    if(!plugin_ptr){
        QMessageBox::information(this, "Information", "No plugin is found.");
        return;
    }

    QPixmap pixmap = currentImage->pixmap();
    QImage image = pixmap.toImage();
    image = image.convertToFormat(QImage::Format_RGB888);

    cv::Mat mat = cv::Mat(image.height(),
                      image.width(),
                      CV_8UC3,
                      image.bits(),
                      image.bytesPerLine()
                      );

    plugin_ptr->edit(mat, mat);

    QImage image_edited(mat.data,
                        mat.cols,
                        mat.rows,
                        mat.step,
                        QImage::Format_RGB888);
    pixmap = QPixmap::fromImage(image_edited);
    showImage(pixmap);
    fitImageToWindow(image_edited.width(), image_edited.height());

}


























