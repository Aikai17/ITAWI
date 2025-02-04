#include "asciiimage.h"
#include <QFile>
#include <QTextStream>
#include <QtMath>
#include <algorithm>
#include <QVector>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

const QString characters = "@@##%%**==,,..";
const int THREAD_COUNT=QThread::idealThreadCount();

AsciiImage::AsciiImage()
{
    QImage nImg(QSize(500,500),QImage::Format_RGB32);
    nImg.fill("black");
    loadImage(nImg,20);
}


QString AsciiImage::getAscii(){
    if(mSourceAscii.isNull()){
        return QString();
    }
    QString textedImage;
    for(int y = 0;y<mPreviewAscii.height();y++){
        const QRgb* line=reinterpret_cast<const QRgb*>(mPreviewAscii.scanLine(y));
        for(int x = 0;x<mPreviewAscii.width();x++){
            const QRgb& pixel=line[x];
            int gray=qGray(pixel);
            int charIndex=gray/((float)255/(characters.size()-1));
            textedImage+=characters[charIndex];
        }
        textedImage+='\n';

    }
    return textedImage;
}
void AsciiImage::invertChunk(const InvertTask& task){
    for(int y =task.yFrom;y<task.yTo;y++){
        const QRgb* line=reinterpret_cast<const QRgb*>(mScaledAscii.scanLine(y));
        QRgb* iLine=reinterpret_cast<QRgb*>(mPreviewAscii.scanLine(y)); //iLine - i stands for inverted
        for(int x=task.xFrom;x<task.xTo;x++){
            const QRgb& pixel=line[x];
            int intervalR=255-qRed(pixel);
            int intervalG=255-qGreen(pixel);
            int intervalB=255-qBlue(pixel);
            int percentR=intervalR/100==0?1 : intervalR/100;
            int percentG=intervalG/100==0?1 : intervalG/100;
            int percentB=intervalB/100==0?1 : intervalB/100;
            int iRed=qRed(pixel)+percentR*lastInvert;
            int iGreen=qGreen(pixel)+percentG*lastInvert;
            int iBlue=qBlue(pixel)+percentB*lastInvert;
            iLine[x]=qRgb(iRed,iGreen,iBlue);
        }
    }
}
void AsciiImage::invert(int invertValue)
{
    if(mSourceAscii.isNull()){
        return;
    }
    lastInvert=invertValue;
    int y = 0;
    int dy=mScaledAscii.height()/THREAD_COUNT==0?1 : mScaledAscii.height()/THREAD_COUNT;
    QVector<InvertTask> tasks;
    for(;y<mScaledAscii.height()-dy;y+=dy){
        tasks<<InvertTask{0,mScaledAscii.width(),y,y+dy};
    }
    QFuture<void> future=QtConcurrent::map(tasks,[this] (const InvertTask& task){
        invertChunk(task);
    });
    invertChunk(InvertTask{0,mScaledAscii.width(),y,mScaledAscii.height()});
    future.waitForFinished();
}

void AsciiImage::loadImage(const QImage &img, int desiredWidth)
{
    mSourceAscii=img;
    mScaledAscii=mSourceAscii;
    mPreviewAscii=mScaledAscii;
    int w=mSourceAscii.width();
    int h=mSourceAscii.height();
    ratio=(float)w/h;
    resize(desiredWidth);
}

QPixmap AsciiImage::getSource()
{
    return QPixmap::fromImage(mSourceAscii);
}

void AsciiImage::resize(int desiredWidth)
{
    if(mSourceAscii.isNull()){
        return;
    }
    int desiredHeight=(desiredWidth/(ratio*2));
    mScaledAscii=mSourceAscii.scaled(desiredWidth,desiredHeight);
    mPreviewAscii=mScaledAscii;
    invert(lastInvert);
}
