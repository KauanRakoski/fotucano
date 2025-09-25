#include "HistogramVisualizer.h"

HistogramVisualizer::HistogramVisualizer(QWidget *parent) : QChartView(parent){
    histogram_chart = new QChart();
    this->setChart(histogram_chart);

    barColor = QColor("#00AEEF");

    histogram_chart->setTitle("Histograma de Tons de Cinza");
    histogram_chart->setAnimationOptions(QChart::SeriesAnimations);
    this->setRenderHint(QPainter::Antialiasing);
    histogram_chart->setTheme(QChart::ChartThemeDark);
    histogram_chart->setMargins(QMargins(0, 0, 0, 0));
    histogram_chart->legend()->hide();

    // garantir que o widget tem espaço razoável para desenhar
    this->setMinimumSize(300, 150);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

void HistogramVisualizer::setTitle(const QString &title)
{
    if (histogram_chart) {
        histogram_chart->setTitle(title);
    }
}

void HistogramVisualizer::setColor(const QColor &color)
{
    this->barColor = color;
}

void HistogramVisualizer::setHistogramData(const QVector<double> &data){
    histogram_chart->removeAllSeries();
    for (QAbstractAxis *axis : histogram_chart->axes()) {
        histogram_chart->removeAxis(axis);
        axis->deleteLater();
    }

    QBarSet *set = new QBarSet("Intensidades");
    set->setColor(barColor);
    set->setBorderColor(barColor.lighter());

    for (int i = 0; i < 256; ++i) {
        *set << data[i];
    }
    QBarSeries *series = new QBarSeries();
    series->append(set);
    histogram_chart->addSeries(series);

    QStringList categories;
    categories.reserve(256);
    for (int i = 0; i < 256; ++i) categories << QString::number(i);

    QFont axisFont("Segoe UI", 8);

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, 255);
    axisX->setTitleText("Intensidade (0-255)");
    axisX->setLabelFormat("%d"); // Garante que os números não tenham casas decimais
    histogram_chart->addAxis(axisX, Qt::AlignBottom);
    axisX->setLabelsFont(axisFont);
    series->attachAxis(axisX);

    auto maxIterator = std::max_element(data.constBegin(), data.constEnd());
    double valorMaximo = *maxIterator;

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, valorMaximo);
    axisY->setTitleText("% Pixels");
    histogram_chart->addAxis(axisY, Qt::AlignLeft);
    axisY->setLabelsFont(axisFont);
    series->attachAxis(axisY);


    histogram_chart->legend()->hide();
    this->setRenderHint(QPainter::Antialiasing);
}
