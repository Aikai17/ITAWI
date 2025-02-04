#ifndef ASCIIIMAGE_H
#define ASCIIIMAGE_H

#include <QImage>
#include <QPixmap>

struct InvertTask {
    int xFrom;
    int xTo;
    int yFrom;
    int yTo;
};

class AsciiImage
{
public:
    explicit AsciiImage();
    QString getAscii();
    void resize(int desiredWidth=100);
    void invert(int invertValue);
    void loadImage(const QImage& img,int desiredWidth=100);
    QPixmap getSource();
private:
    void invertChunk(const InvertTask&);
private:
    int lastInvert=0;
    float ratio;
    QImage mPreviewAscii;
    QImage mSourceAscii;
    QImage mScaledAscii;
};

#endif // ASCIIIMAGE_H
