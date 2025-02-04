#include "mainwindow.h"
#include <QFile>
#include <QScrollArea>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    mSourceLabel=new QLabel(this);
    mSourceLabel->setPixmap(asciiImage.getSource());
    mSourceLabel->setScaledContents(true);
    mSourceLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

    mPreview=new QLabel(this);
    mPreview->setWordWrap(false);
    mPreview->setFont(QFont("Courier",5));
    mPreview->setAlignment(Qt::AlignLeft|Qt::AlignTop);

    mScrollArea=new QScrollArea(this);
    mScrollArea->setWidget(mPreview);
    mScrollArea->setWidgetResizable(true);

    mSplitter = new QSplitter(this);
    mSplitter->addWidget(mSourceLabel);
    mSplitter->addWidget(mScrollArea);
    mSplitter->setSizes({50,50});

    mInvertSlider=new QSlider(this);
    mInvertSlider->setOrientation(Qt::Horizontal);
    mInvertSlider->setRange(0,100);
    mInvertSlider->setMinimumSize(50,30);
    mInvertSlider->setSingleStep(1);
    connect(mInvertSlider,&QSlider::valueChanged,this,&MainWindow::onInvertChanged);

    mFSizeSlider=new QSlider(this);
    mFSizeSlider->setOrientation(Qt::Horizontal);
    mFSizeSlider->setRange(5,30);
    mFSizeSlider->setMinimumSize(50,30);
    mFSizeSlider->setSingleStep(1);
    connect(mFSizeSlider,&QSlider::valueChanged,this,&MainWindow::onFSizeChanged);

    mScaleSlider=new QSlider(this);
    mScaleSlider->setOrientation(Qt::Horizontal);
    mScaleSlider->setRange(20,300);
    mScaleSlider->setMinimumSize(50,30);
    mScaleSlider->setSingleStep(2);
    connect(mScaleSlider,&QSlider::valueChanged,this,&MainWindow::onScaleChanged);

    mScaleLabel=new QLabel(this);
    mScaleLabel->setMinimumSize(50,30);
    mScaleLabel->setText("SCALE ");

    mScaleValueLabel=new QLabel(this);
    mScaleValueLabel->setMinimumSize(50,30);
    mScaleValueLabel->setText(QString::number(mScaleSlider->value()));
    connect(mScaleSlider,&QSlider::valueChanged,this,&MainWindow::onScaleChanged);

    mInvertLabel=new QLabel(this);
    mInvertLabel->setMinimumSize(50,30);
    mInvertLabel->setText("INVERT ");

    mInvertValueLabel=new QLabel(this);
    mInvertValueLabel->setMinimumSize(50,30);
    mInvertValueLabel->setText(QString::number(mInvertSlider->value()));
    connect(mInvertSlider,&QSlider::valueChanged,this,&MainWindow::onInvertChanged);

    mFSizeLabel=new QLabel(this);
    mFSizeLabel->setMinimumSize(50,30);
    mFSizeLabel->setText("FONT ");

    mFSizeValueLabel=new QLabel(this);
    mFSizeValueLabel->setMinimumSize(50,30);
    mFSizeValueLabel->setText(QString::number(mFSizeSlider->value()));
    connect(mFSizeSlider,&QSlider::valueChanged,this,&MainWindow::onFSizeChanged);

    QVBoxLayout* layout=new QVBoxLayout;
    QHBoxLayout* sliderLayout=new QHBoxLayout;
    sliderLayout->addWidget(mScaleLabel);
    sliderLayout->addWidget(mScaleSlider);
    sliderLayout->addWidget(mScaleValueLabel);
    sliderLayout->addWidget(mFSizeLabel);
    sliderLayout->addWidget(mFSizeSlider);
    sliderLayout->addWidget(mFSizeValueLabel);
    sliderLayout->addWidget(mInvertLabel);
    sliderLayout->addWidget(mInvertSlider);
    sliderLayout->addWidget(mInvertValueLabel);

    QWidget* container=new QWidget(this);
    layout->addWidget(mSplitter);
    layout->addLayout(sliderLayout);
    layout->setStretch(0,1);
    layout->setStretch(1,0);
    container->setLayout(layout);

    createActions();
    createMenus();

    mPreview->setText(asciiImage.getAscii());
    setCentralWidget(container);
}

void MainWindow::onInvertChanged(int value)
{
    mInvertValueLabel->setText(QString::number(value));
    asciiImage.invert(value);
    mPreview->setText(asciiImage.getAscii());
}

void MainWindow::onScaleChanged(int value)
{
    mScaleValueLabel->setText(QString::number(value));
    asciiImage.resize(value);
    mPreview->setText(asciiImage.getAscii());
}

void MainWindow::openImage()
{
    QImage img(QFileDialog::getOpenFileUrl(this,"Open Image",QStandardPaths::standardLocations(QStandardPaths::PicturesLocation)[0]).path());
    if(img.isNull()){
        QMessageBox::critical(this,"Error","Can't open image");
        return;
    }
    asciiImage.loadImage(img,mScaleSlider->minimum());
    mSourceLabel->setPixmap(QPixmap::fromImage(img));

    //resize ascii due to sliders' value
    onInvertChanged(mInvertSlider->value());
    onScaleChanged(mScaleSlider->value());
}
void MainWindow::saveAscii(){
    QFile asciiFile(QFileDialog::getSaveFileUrl(this,"Save",QStandardPaths::standardLocations(QStandardPaths::HomeLocation)[0]).path());
    if(!asciiFile.open(QFile::WriteOnly)){
        QMessageBox::critical(this,"Error","Can't save ASCII");
        return;
    }
    asciiFile.write(mPreview->text().toLatin1().data(),mPreview->text().size());
}

void MainWindow::copyAscii()
{
    QClipboard* clipboard=qApp->clipboard();
    clipboard->setText(mPreview->text());
}

void MainWindow::createActions()
{
    openAct=new QAction(QIcon::fromTheme("DocumentOpen"),
                          "&Open...",this);
    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip("Open a new image");
    connect(openAct,&QAction::triggered,this,&MainWindow::openImage);

    saveAct=new QAction(QIcon::fromTheme("DocumentSave"),
                          "&Save...",this);
    saveAct->setShortcut(QKeySequence::Save);
    saveAct->setStatusTip("Save the ASCII");
    connect(saveAct,&QAction::triggered,this,&MainWindow::saveAscii);

    exitAct=new QAction(QIcon::fromTheme("ApplicationExit"),
                          "&Exit",this);
    exitAct->setShortcut(QKeySequence::Quit);
    connect(exitAct,&QAction::triggered,this,&QWidget::close);

    copyAct=new QAction(QIcon::fromTheme("EditCopy"),
                          "&Copy",this);
    copyAct->setShortcut(QKeySequence::Copy);
    connect(copyAct,&QAction::triggered,this,&MainWindow::copyAscii);
}
void MainWindow::createMenus()
{
    fileMenu=menuBar()->addMenu("&File");
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu=menuBar()->addMenu("&Edit");
    editMenu->addAction(copyAct);
}

void MainWindow::onFSizeChanged(int value)
{
    mPreview->setFont(QFont("Courier",value));
    mFSizeValueLabel->setText(QString::number(value));
}

MainWindow::~MainWindow() {}

