#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QGraphicsPixmapItem>
#include <QStatusBar>
#include <QMap>
#include "editor_plugin_interface.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow( QWidget *parent=nullptr);
    ~MainWindow();

private:
    void initUI();
    void createActions();
    void showImage(QString);
    void showImage(QPixmap &);
    void setupShortcuts();
    void fitImageToWindow(int, int);
    void loadPlugins();

private slots:
    void openImage();
    void zoomIn();
    void zoomOut();
    void prevImage();
    void nextImage();
    void saveAs();
    void fitImageToWindow();
    void pluginPerform();

private:
    QMap<QString, EditorPluginInterface* > editplugins;
    bool fitted;
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *editMenu;

    QToolBar *fileToolBar;
    QToolBar *viewToolBar;
    QToolBar *editToolBar;

    QGraphicsScene *imageScene;
    QGraphicsView *imageView;
    QStatusBar *mainStatusBar;
    QLabel *mainStatusLabel;

    QAction *openAction;
    QAction *saveAsAction;
    QAction *exitAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *prevAction;
    QAction *nextAction;

    QAction *fitToWindowAction;
    // edit actions

    QString currentImagePath;
    QGraphicsPixmapItem *currentImage;
};

#endif // MAINWINDOW_H
