#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QSplitter>
#include <QPushButton>
#include <QSlider>
#include <QObject>
#include <QScrollBar>
#include <QScrollArea>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include "asciiimage.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    AsciiImage asciiImage;
    QLabel* mSourceLabel;
    QLabel* mPreview;
    QLabel* mFSizeLabel;
    QLabel* mFSizeValueLabel;
    QLabel* mScaleLabel;
    QLabel* mScaleValueLabel;
    QLabel* mInvertLabel;
    QLabel* mInvertValueLabel;
    QSplitter* mSplitter;
    QSlider* mFSizeSlider;
    QSlider* mInvertSlider;
    QSlider* mScaleSlider;
    QScrollArea* mScrollArea;
    QScrollBar* mHorizontalBar;
    QScrollBar* mVerticalBar;

private:
    QMenu* fileMenu;
    QMenu* editMenu;
    QAction* openAct;
    QAction* saveAct;
    QAction* exitAct;
    QAction* copyAct;
private:
    void createActions();
    void createMenus();
private slots:
    void onFSizeChanged(int value);
    void onInvertChanged(int value);
    void onScaleChanged(int value);
    void saveAscii();
    void copyAscii();
    void openImage();
};
#endif // MAINWINDOW_H
