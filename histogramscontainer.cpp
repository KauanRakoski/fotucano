#include "histogramscontainer.h"
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>

HistogramsContainer::HistogramsContainer(QWidget *parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    stackedWidget = new QStackedWidget();

    QWidget* grayscalePage = new QWidget();
    QVBoxLayout* grayscaleLayout = new QVBoxLayout(grayscalePage);
    grayscaleHistBefore = new HistogramVisualizer();
    grayscaleHistBefore->setTitle("Histograma Original");
    grayscaleHistAfter = new HistogramVisualizer();
    grayscaleHistAfter->setTitle("Histograma Processado");
    grayscaleLayout->addWidget(grayscaleHistBefore);
    grayscaleLayout->addWidget(grayscaleHistAfter);

    QWidget* colorPage = new QWidget();
    QVBoxLayout* colorLayout = new QVBoxLayout(colorPage);
    colorHistR = new HistogramVisualizer();
    colorHistR->setTitle("Canal Vermelho");
    colorHistR->setColor(Qt::red);
    colorHistG = new HistogramVisualizer();
    colorHistG->setTitle("Canal Verde");
    colorHistG->setColor(Qt::green);
    colorHistB = new HistogramVisualizer();
    colorHistB->setTitle("Canal Azul");
    colorHistB->setColor(Qt::blue);
    colorLayout->addWidget(colorHistR);
    colorLayout->addWidget(colorHistG);
    colorLayout->addWidget(colorHistB);

    stackedWidget->addWidget(grayscalePage);
    stackedWidget->addWidget(colorPage);

    mainLayout->addWidget(stackedWidget);
}

void HistogramsContainer::displayGrayscaleResults(const QVector<double>& histBefore, const QVector<double>& histAfter)
{
    stackedWidget->setCurrentIndex(0);
    grayscaleHistBefore->setHistogramData(histBefore);
    grayscaleHistAfter->setHistogramData(histAfter);
}

void HistogramsContainer::displayColorResults(const QVector<double>& histR, const QVector<double>& histG, const QVector<double>& histB)
{
    stackedWidget->setCurrentIndex(1);
    colorHistR->setHistogramData(histR);
    colorHistG->setHistogramData(histG);
    colorHistB->setHistogramData(histB);
}

void HistogramsContainer::clear()
{
    QVector<double> emptyData(256, 0.0);
    grayscaleHistBefore->setHistogramData(emptyData);
    grayscaleHistAfter->setHistogramData(emptyData);
    colorHistR->setHistogramData(emptyData);
    colorHistG->setHistogramData(emptyData);
    colorHistB->setHistogramData(emptyData);
}
