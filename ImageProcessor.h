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
    QImage turn90degrees (const QImage &source);
    QImage turn90degreesAnti (const QImage &source);

    QImage zoom2x (const QImage &source);
    QImage zoomOut(const QImage &source, double sx, double sy);

    QImage tonsDeCinza (const QImage &source);
    QImage quantizar (const QImage &source, int num_shades);

    void computeHistogram (const QImage &source, int histogramResult[256]);
    void cumulativeHistogramNormalized(int histogram[256]);
    QVector<QVector<int>> computeRGBHistograms(const QImage &source);
    QVector<double> HistogramInPercentages (const QImage &source, int histogramPercentages[256]);

    QImage equalizeHistogram(const QImage &source);
    QImage equalizeColorHistogram(const QImage &source);
    QImage histogramMatch (const QImage &source, const QImage &target);

    QImage biasChange(const QImage &source, int bias);
    QImage gainChange(const QImage &source, int gain);

    QImage negativeOfImage(const QImage &source);

    QImage applyConvolutionWithoutBorders(const QImage &source, const QVector<double> &kernel);


private:
    void cumulativeHistogram (int histogram[256], int result[256]);
    QVector<int> calculateEqualizationLUT(const QImage& source);

};

#endif // IMAGEPROCESSOR_H
