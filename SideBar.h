#ifndef SIDEBAR_H
#define SIDEBAR_H

#include "histogramscontainer.h"
#include "kerneleditor.h"

#include <Qwidget>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QLabel>

class Sidebar : public QWidget {
    Q_OBJECT

public:
    explicit Sidebar (QWidget *parent = nullptr);
    HistogramsContainer* histogramsContainer;

public slots:
    void updateHistogram(QVector<double> &data);

private:
    QPushButton *verticalMirror;
    QPushButton *horizontalMirror;
    QPushButton *turn90degreesBtn;
    QPushButton *turn90degreesAntiBtn;

    QPushButton *zoomBtn;
    QDoubleSpinBox *outFactorX;
    QDoubleSpinBox *outFactorY;
    QPushButton *zoomOutBtn;

    QPushButton *converterCiza;
    QPushButton *revertOriginal;
    QPushButton *quantify;
    QSpinBox *spinTons;

    QSlider *biasSlider;
    QSlider *gainSlider;
    QLabel *biasLabel;
    QLabel *gainLabel;

    QPushButton *equalizeBtn;
    QPushButton *histMatchBtn;
    QPushButton *invertImageBtn;

    KernelEditor *kernelEditor;

signals:
    void mirrorVertically();
    void mirrorHorizontally();
    void turn90degrees();
    void turn90degreesAnti();

    void zoomIn();

    void tornarTonsDeCinza();
    void applyQuantization(int num_shades);
    void revertToOriginal();

    void biasChanged(int value);
    void gainChanged(int value);
    void equalizeHistogram();
    void histogramMatchRequest();
    void invertImage();

    void requestKernelApply(const QVector<double> &kernel);
    void requestZoomOut(double sx, double sy);
};

#endif // SIDEBAR_H
