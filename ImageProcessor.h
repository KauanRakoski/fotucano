#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <QImage>

class ImageProcessor : public QObject {
    Q_OBJECT

public:
    explicit ImageProcessor (QObject *parent = nullptr);

    QImage espelharHorizontal (const QImage &source);
    QImage espelharVertical (const QImage &source);
    QImage tonsDeCinza (const QImage &source);
    QImage quantizar (const QImage &source, int num_shades);
};

#endif // IMAGEPROCESSOR_H
