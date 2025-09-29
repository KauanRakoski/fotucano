#include "ImageProcessor.h"
#include <QRgb>
#include <QColor>
#include <QtMath>

#include <cstring>
#define NUMBER_OF_CHANNELS 3
#define KERNEL_SIZE 3

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

void ImageProcessor::computeHistogram (const QImage &source, int histogramResult[256]){
    for (int i = 0; i <= 255; i++){
        histogramResult[i] = 0;
    }

    int height = source.height();
    int width = source.width();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QColor GrayShade = source.pixelColor(x, y);
            int grayValue = GrayShade.red();

            histogramResult[grayValue] += 1;
        }
    }
}

QVector<QVector<int>> ImageProcessor::computeRGBHistograms(const QImage &source)
{
    QVector<QVector<int>> histograms(3, QVector<int>(256, 0));
    if (source.isNull() || source.isGrayscale()) return histograms;

    for (int y = 0; y < source.height(); ++y) {
        for (int x = 0; x < source.width(); ++x) {
            QRgb pixel = source.pixel(x, y);
            histograms[0][qRed(pixel)]++;   // Red
            histograms[1][qGreen(pixel)]++; // Green
            histograms[2][qBlue(pixel)]++;  // Blue
        }
    }
    return histograms;
}

QVector<double> ImageProcessor::HistogramInPercentages(const QImage &source, int hist[256]){
    this->computeHistogram(source, hist);

    QVector<double> histogramaNormalizado(256, 0.0);
    double totalPixels = source.width() * source.height();

    for (int i = 0; i < 256; ++i) {
        histogramaNormalizado[i] = ((double)hist[i] / totalPixels) * 100.0;
    }

    return histogramaNormalizado;
}

void ImageProcessor::cumulativeHistogram (int histogram[256], int result[256]){
    result[0] = histogram[0];

    for (int i = 1; i < 256; i++){
        result[i] = result[i-1] + histogram[i];
    }
}

void ImageProcessor::cumulativeHistogramNormalized(int histogram[256]){
    int max = histogram[255];

    for (int s = 0; s < 256; s++){
        histogram[s] = qRound(histogram[s] * 255.0f / max);
    }
}

QImage ImageProcessor::equalizeHistogram(const QImage &source){
    QImage resultado(source.size(), source.format());
    int height = source.height(), width = source.width();

    int hist[256] = {0}, cum_hist[256] = {0};

    this->computeHistogram(source, hist);
    this->cumulativeHistogram(hist, cum_hist);

    // Normalize histogram
    double totalPixels = width * height;

    for (int i = 0; i < 256; i++){
        cum_hist[i] = qRound( ( (double)cum_hist[i] / totalPixels) * 255.0 );
    }

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            int originalShade = source.pixelColor(x, y).red();
            int newShade = cum_hist[originalShade];

            QColor newShadeColor(newShade, newShade, newShade);
            resultado.setPixelColor(x, y, newShadeColor);
        }
    }

    return resultado;
}


QImage ImageProcessor::equalizeColorHistogram(const QImage &source)
{
    if (source.isNull() || source.isGrayscale()) {
        return source;
    }

    QImage luminanceImage = this->tonsDeCinza(source);

    int hist[256];
    this->computeHistogram(luminanceImage, hist);

    int cdf[256];
    this->cumulativeHistogram(hist, cdf);
    this->cumulativeHistogramNormalized(cdf);

    QImage result(source.size(), source.format());

    int height = source.height();
    int width = source.width();
    int bytesPerPixel = source.depth() / 8;

    const uchar *source_ptr = source.constBits();
    uchar *result_ptr = result.bits();

    long int total_bytes = width * height * bytesPerPixel;

    for (long int i = 0; i < total_bytes; i += bytesPerPixel) {
        result_ptr[i] = cdf[source_ptr[i]];
        result_ptr[i + 1] = cdf[source_ptr[i + 1]];
        result_ptr[i + 2] = cdf[source_ptr[i + 2]];

        if (bytesPerPixel == 4) {
            result_ptr[i + 3] = source_ptr[i + 3];
        }
    }

    return result;
}

QImage ImageProcessor::biasChange(const QImage &source, int bias){
    QImage resultado(source.size(), source.format());
    int height = source.height();
    int width = source.width();

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            QColor pixelColor = source.pixelColor(x, y);

            int red = pixelColor.red();
            int green = pixelColor.green();
            int blue = pixelColor.blue();

            red = qBound(0, red + bias, 255);
            green = qBound(0, green + bias, 255);
            blue = qBound(0, blue + bias, 255);

            QColor newColor(red, green, blue);
            resultado.setPixelColor(x, y, newColor);
        }
    }

    return resultado;
}

QImage ImageProcessor::gainChange(const QImage &source, int gain){
    QImage resultado(source.size(), source.format());
    int height = source.height();
    int width = source.width();

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            QColor pixelColor = source.pixelColor(x, y);

            int red = pixelColor.red();
            int green = pixelColor.green();
            int blue = pixelColor.blue();

            red = qBound(0, red * gain, 255);
            green = qBound(0, green * gain, 255);
            blue = qBound(0, blue * gain, 255);

            QColor newColor(red, green, blue);
            resultado.setPixelColor(x, y, newColor);
        }
    }

    return resultado;
}

QImage ImageProcessor::negativeOfImage(const QImage &source){
    QImage resultado(source.size(), source.format());
    int height = source.height(), width = source.width();

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            QColor pixelColor = source.pixelColor(x, y);

            int red = 255 - pixelColor.red();
            int green = 255 - pixelColor.green();
            int blue = 255 - pixelColor.blue();

            QColor newColor(red, green, blue);
            resultado.setPixelColor(x, y, newColor);
        }
    }

    return resultado;
}

QImage ImageProcessor::applyConvolutionWithoutBorders(const QImage &source, const QVector<double> &kernel){
    QImage resultado = source;
    int height = source.height(), width = source.width();


    // 1 for ignoring borders
    for (int y = 1; y < height - 1; y++){
        for (int x = 1; x < width -1; x++){
            double sumR = 0.0, sumG = 0.0, sumB = 0.0;

            // Calculating the convolution
            for (int ky = -1; ky <= 1; ky++){
                for (int kx = -1; kx <=1; kx++){

                    QRgb neighborPixel = source.pixel(x + kx, y + ky);

                    int kernelIndex = (ky + 1) * KERNEL_SIZE + (kx + 1); // We use a vector instead of a matrix, so it needs to be converted
                    double kernelValue = kernel[kernelIndex];

                    sumR += qRed(neighborPixel) * kernelValue;
                    sumG += qGreen(neighborPixel) * kernelValue;
                    sumB += qBlue(neighborPixel) * kernelValue;
                }
            }

            sumR = qBound(0, qRound(sumR), 255);
            sumG = qBound(0, qRound(sumG), 255);
            sumB = qBound(0, qRound(sumB), 255);

            resultado.setPixel(x,y, qRgb(sumR, sumG, sumB));
        }
    }

    return resultado;
}

QImage ImageProcessor::turn90degrees (const QImage &source){
    int originalWidth = source.width(), originalHeight = source.height();

    QImage resultado(originalHeight, originalWidth, source.format());

    for (int y = 0; y < originalHeight; y++){
        for (int x = 0; x < originalWidth; x++){
            QColor originalColor = source.pixelColor(x, y);

            int destination_x = originalHeight - y - 1;
            int destination_y = x;

            resultado.setPixelColor(destination_x, destination_y, originalColor);
        }
    }

    return resultado;
}

QImage ImageProcessor::turn90degreesAnti (const QImage &source){
    int originalWidth = source.width(), originalHeight = source.height();

    QImage resultado(originalHeight, originalWidth, source.format());

    for (int y = 0; y < originalHeight; y++){
        for (int x = 0; x < originalWidth; x++){
            QColor originalColor = source.pixelColor(x, y);

            int destination_x = y;
            int destination_y = originalWidth - x - 1;

            resultado.setPixelColor(destination_x, destination_y, originalColor);
        }
    }

    return resultado;
}

QImage ImageProcessor::zoom2x(const QImage &source){
    int oldWidth = source.width();
    int oldHeight = source.height();
    int newWidth = oldWidth * 2 - 1;
    int newHeight = oldHeight * 2 - 1;

    QImage resultado(newWidth, newHeight, source.format());

    for (int y = 0; y < oldHeight; ++y) {
        for (int x = 0; x < oldWidth; ++x) {
            resultado.setPixel(x * 2, y * 2, source.pixel(x, y));
        }
    }

    // Horizontal interpolation
    for (int y = 0; y < newHeight; y += 2){
        for (int x = 1; x < newWidth; x += 2){
            QRgb leftColor = resultado.pixel(x - 1, y);
            QRgb rightColor = resultado.pixel(x + 1, y);

            int red = (qRed(leftColor) + qRed(rightColor)) / 2;
            int green = (qGreen(leftColor) + qGreen(rightColor)) / 2;
            int blue = (qBlue(leftColor) + qBlue(rightColor)) / 2;

            resultado.setPixel(x, y, qRgb(red, green, blue));
        }
    }

    // Vertical interpolation
    for (int x = 0; x < newWidth; x++){
        for (int y = 1; y < newHeight; y += 2){
            QRgb leftColor = resultado.pixel(x, y - 1);
            QRgb rightColor = resultado.pixel(x, y + 1);

            int red = (qRed(leftColor) + qRed(rightColor)) / 2;
            int green = (qGreen(leftColor) + qGreen(rightColor)) / 2;
            int blue = (qBlue(leftColor) + qBlue(rightColor)) / 2;

            resultado.setPixel(x, y, qRgb(red, green, blue));
        }
    }

    return resultado;
}

QImage ImageProcessor::zoomOut (const QImage &source, double sx, double sy){
    int oldHeight = source.height();
    int oldWidth = source.width();

    int newWidth = qFloor(oldWidth / sx);
    int newHeight = qFloor(oldHeight / sy);

    // Force stop if image gets too small
    if (newWidth == 0 || newHeight == 0) return source;

    QImage resultado(newWidth, newHeight, source.format());

    for (int py = 0; py < newHeight; py++){
        for (int px = 0; px < newWidth; px++){
            int startX = qFloor(px * sx);
            int startY = qFloor(py * sy);
            int endX = qFloor((px + 1) * sx);
            int endY = qFloor((py + 1) * sy);

            // Grant that doesn't pass borders
            endX = qMin(endX, oldWidth);
            endY = qMin(endY, oldHeight);

            double sumR = 0, sumG = 0, sumB = 0;
            int pixelCount = 0;

            for (int y = startY; y < endY; y++) {
                for (int x = startX; x < endX; x++) {
                    QRgb pixel = source.pixel(x, y);
                    sumR += qRed(pixel);
                    sumG += qGreen(pixel);
                    sumB += qBlue(pixel);
                    pixelCount++;
                }
            }

            if (pixelCount > 0) {
                int avgR = qBound(0, qRound(sumR / pixelCount), 255);
                int avgG = qBound(0, qRound(sumG / pixelCount), 255);
                int avgB = qBound(0, qRound(sumB / pixelCount), 255);
                resultado.setPixel(px, py, qRgb(avgR, avgG, avgB));
            }
        }
    }

    return resultado;
}


int findClosestIntensity(int source_cum_value, const int target_cum_hist[256], int original_intensity) {
    int closest_intensity = 0;
    int min_diff = 2147483647; // Valor máximo de um inteiro

    for (int target_intensity = 0; target_intensity < 256; ++target_intensity) {
        int diff = abs(source_cum_value - target_cum_hist[target_intensity]);

        if (diff < min_diff) {
            min_diff = diff;
            closest_intensity = target_intensity;
        } else if (diff == min_diff) {
            if (abs(target_intensity - original_intensity) < abs(closest_intensity - original_intensity)) {
                closest_intensity = target_intensity;
            }
        }
    }
    return closest_intensity;
}

QImage ImageProcessor::histogramMatch(const QImage &source, const QImage &target){
    QImage resultado(source.size(), source.format());

    int hist_original[256] = {0}, hist_target[256] = {0};
    int cum_hist_or[256] = {0}, cum_hist_target[256] = {0};
    int hm[256];

    this->computeHistogram(source, hist_original);
    this->computeHistogram(target, hist_target);

    this->cumulativeHistogram(hist_original, cum_hist_or);
    this->cumulativeHistogram(hist_target, cum_hist_target);

    int norm_cum_hist_or[256] = {0};
    int norm_cum_hist_target[256] = {0};

    const float total_pixels_source = source.width() * source.height();
    const float total_pixels_target = target.width() * target.height();

    for (int i = 0; i < 256; ++i) {
        norm_cum_hist_or[i] = qRound((cum_hist_or[i] / total_pixels_source) * 255.0f);
        norm_cum_hist_target[i] = qRound((cum_hist_target[i] / total_pixels_target) * 255.0f);
    }

    for (int s = 0; s < 256; s++){
        hm[s] = findClosestIntensity(norm_cum_hist_or[s], norm_cum_hist_target, s);
    }

    int height = source.height();
    int width = source.width();

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            int originalShade = source.pixelColor(x, y).red();
            int targetShade = hm[originalShade];

            resultado.setPixelColor(x, y, qRgb(targetShade, targetShade, targetShade));
        }
    }

    return resultado;
}
