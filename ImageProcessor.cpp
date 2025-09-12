#include "ImageProcessor.h"
#include <QRgb>
#include <QColor>
#include <QtMath>

#include <cstring>
#define NUMBER_OF_CHANNELS 3

ImageProcessor::ImageProcessor (QObject *parent) : QObject(parent) {}

QImage ImageProcessor::espelharHorizontal(const QImage &source) {
    QImage resultado(source.size(), source.format());
    int width = source.width();
    int height = source.height();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QRgb origin = source.pixel(x, y);
            resultado.setPixel(width - x - 1, y, origin);
        }
    }

    return resultado;
}

QImage ImageProcessor::espelharVertical (const QImage &source){
    QImage resultado(source.size(), source.format());
    const int height = source.height();
    const int lineSizeInBytes = source.bytesPerLine();
    const uchar *imagePointer = source.constBits(); // retorna um ponteiro para o inicio da imagem
    uchar *resultadoPointer = resultado.bits();

    for (int y = 0; y < height; y++){
        const uchar *copied = imagePointer + y * lineSizeInBytes;
        uchar *pasted = resultadoPointer + (height- y - 1) * lineSizeInBytes;

        std::memcpy(pasted, copied, lineSizeInBytes);
    }

    return resultado;
}

QImage ImageProcessor::tonsDeCinza (const QImage &source){
    QImage resultado(source.size(), source.format());

    for (int y = 0; y < source.height(); y++){
        for (int x = 0; x < source.width(); x++){
            QColor toGrayShade = source.pixelColor(x, y);

            int red = toGrayShade.red();
            int green = toGrayShade.green();
            int blue = toGrayShade.blue();

            int luminancia = qRound(0.299 * red + 0.587 * green + 0.114 * blue);

            QRgb resultingShade = qRgb(luminancia, luminancia, luminancia);

            resultado.setPixel(x, y, resultingShade);
        }
    }
    return resultado;
}

// Input: image in gray shades
QImage ImageProcessor::quantizar (const QImage &source, int num_shades){
    int tmax = 0;
    int tmin = 255;

    // Get range in pixel intensity
    for (int y = 0; y < source.height(); y++){
        for (int x = 0; x < source.width(); x++){
            QColor originalGrayShade = source.pixelColor(x, y);
            int l = originalGrayShade.red();

            if (l > tmax) tmax = l;
            if (l < tmin) tmin = l;
        }
    }

    int total_shades = tmax - tmin + 1;

    if (num_shades >= total_shades){
        return source;
    }

    QImage resultado(source.size(), source.format());

    float bin_size = (float) total_shades / num_shades;

    for (int y = 0; y < source.height(); y++){
        for (int x = 0; x < source.width(); x++){
            QColor originalGrayShade = source.pixelColor(x, y);
            int l = originalGrayShade.red();

            int bin_index = (int) ((l - tmin) / bin_size);
            int bin_center = qRound (tmin + (bin_index * bin_size) + (bin_size/2.0));

            int finalL = qBound(0, bin_center, 255);

            QRgb resultingColor = qRgb(finalL, finalL, finalL);
            resultado.setPixel(x, y, resultingColor);
        }
    }

    return resultado;
}
