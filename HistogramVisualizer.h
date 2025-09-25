#ifndef HISTOGRAMVISUALIZER_H
#define HISTOGRAMVISUALIZER_H

#include <QtCharts> // Inclui todos os cabeçalhos do Qt Charts
#include <QChartView>
#include <QBarSeries>


class HistogramVisualizer : public QChartView {
    Q_OBJECT

public:
    explicit HistogramVisualizer(QWidget *parent = nullptr);

public slots:
    void setHistogramData(const QVector<double> &data);
    void setColor(const QColor &color);
    void setTitle(const QString &title);

private:
    QChart *histogram_chart;
    QColor barColor;
};

#endif // HISTOGRAMVISUALIZER_H
