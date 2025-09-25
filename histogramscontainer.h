#ifndef HISTOGRAMSCONTAINER_H
#define HISTOGRAMSCONTAINER_H

#include <QWidget>
#include "histogramvisualizer.h"

class QStackedWidget;

class HistogramsContainer : public QWidget {
    Q_OBJECT
public:
    explicit HistogramsContainer(QWidget *parent = nullptr);

public slots:
    void displayGrayscaleResults(const QVector<double>& histBefore, const QVector<double>& histAfter);
    void displayColorResults(const QVector<double>& histR, const QVector<double>& histG, const QVector<double>& histB);
    void clear();

private:
    QStackedWidget* stackedWidget;

    HistogramVisualizer* grayscaleHistBefore;
    HistogramVisualizer* grayscaleHistAfter;

    HistogramVisualizer* colorHistR;
    HistogramVisualizer* colorHistG;
    HistogramVisualizer* colorHistB;
};

#endif // HISTOGRAMSCONTAINER_H
