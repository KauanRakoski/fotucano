#include "SideBar.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QSlider>


Sidebar::Sidebar (QWidget *parent) : QWidget(parent) {
    QWidget *principais = new QWidget(this);
    QVBoxLayout *layoutLateral = new QVBoxLayout(principais);

    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->setMaximumWidth(400);

    QLabel *editLabel = new QLabel("Opções de Edição", this);
    editLabel->setObjectName("editLabel");

    QGroupBox *grupoEspelhamento = new QGroupBox("Espelhamento");
    QVBoxLayout *layoutEspelhamento = new QVBoxLayout();

    verticalMirror = new QPushButton("Vertical");
    horizontalMirror = new QPushButton("Horizontal");
    turn90degreesBtn = new QPushButton("Rotacionar Horário");
    turn90degreesAntiBtn = new QPushButton("Rotacionar Antihorário");
    layoutEspelhamento->addWidget(verticalMirror);
    layoutEspelhamento->addWidget(horizontalMirror);
    layoutEspelhamento->addWidget(turn90degreesBtn);
    layoutEspelhamento->addWidget(turn90degreesAntiBtn);

    grupoEspelhamento->setLayout(layoutEspelhamento);

    QGroupBox *grupoZoom = new QGroupBox("Zoom");
    QVBoxLayout *layoutZoom = new QVBoxLayout();

    zoomBtn = new QPushButton("Zoom 2x");
    zoomOutBtn = new QPushButton("Reduzir");

    QLabel *reduceFactor = new QLabel("Fator de redução:");

    QGridLayout *layoutReducao = new QGridLayout();

    QLabel *labelSx = new QLabel("Fator Sx:");
    outFactorX = new QDoubleSpinBox();

    QLabel *labelSy = new QLabel("Fator Sy:");
    outFactorY = new QDoubleSpinBox();

    outFactorX->setRange(1.0, 100.0);
    outFactorX->setSingleStep(0.1);
    outFactorX->setValue(2.0);
    outFactorX->setDecimals(2);

    outFactorY->setRange(1.0, 100.0);
    outFactorY->setSingleStep(0.1);
    outFactorY->setValue(2.0);
    outFactorY->setDecimals(2);

    layoutReducao->addWidget(labelSx, 0, 0);
    layoutReducao->addWidget(outFactorX, 0, 1);

    layoutReducao->addWidget(labelSy, 1, 0);
    layoutReducao->addWidget(outFactorY, 1, 1);

    layoutZoom->addWidget(zoomBtn);
    layoutZoom->addWidget(zoomOutBtn);
    layoutZoom->addWidget(reduceFactor);
    layoutZoom->addLayout(layoutReducao);

    grupoZoom->setLayout(layoutZoom);

    QGroupBox *grupoCinza = new QGroupBox("Tons de Cinza");
    QVBoxLayout *layoutCinza = new QVBoxLayout();
    converterCiza = new QPushButton("Converter para Cinza");

    layoutCinza->addWidget(converterCiza);
    grupoCinza->setLayout(layoutCinza);

    QGroupBox *grupoQuantizacao = new QGroupBox("Quantização");
    QGridLayout *layoutQuantizacao = new QGridLayout();
    layoutQuantizacao->addWidget(new QLabel("Nº de Tons:"), 0, 0);
    spinTons = new QSpinBox();
    spinTons->setRange(2, 256);
    spinTons->setValue(256);
    layoutQuantizacao->addWidget(spinTons, 0, 1);

    quantify = new QPushButton("Aplicar Quantização");
    layoutQuantizacao->addWidget(quantify, 1, 0, 1, 2);
    grupoQuantizacao->setLayout(layoutQuantizacao);

    revertOriginal = new QPushButton("Reverter para original");

    // Adiciona os grupos de operações à barra lateral
    layoutLateral->addWidget(editLabel);
    layoutLateral->addWidget(grupoEspelhamento);
    layoutLateral->addWidget(grupoZoom);
    layoutLateral->addWidget(grupoCinza);
    layoutLateral->addWidget(grupoQuantizacao);
    layoutLateral->addWidget(revertOriginal);
    layoutLateral->addStretch();


    QWidget *tons = new QWidget();
    QVBoxLayout *tonsLayout = new QVBoxLayout(tons);
    QGroupBox *grupoAjustes = new QGroupBox("Ajustes");
    QGridLayout *layoutAjustes = new QGridLayout();

    biasSlider = new QSlider(Qt::Horizontal);
    biasSlider->setRange(-255, 255);
    biasSlider->setValue(0);
    biasLabel = new QLabel("0");
    biasLabel->setMinimumWidth(30);

    gainSlider = new QSlider(Qt::Horizontal);
    gainSlider->setRange(0, 255);
    gainSlider->setValue(1);
    gainLabel = new QLabel("0");
    gainLabel->setMinimumWidth(30);

    // Adiciona os componentes ao layout em grade
    layoutAjustes->addWidget(new QLabel("Brilho:"), 0, 0);
    layoutAjustes->addWidget(biasSlider, 0, 1);
    layoutAjustes->addWidget(biasLabel, 0, 2);

    layoutAjustes->addWidget(new QLabel("Contraste:"), 1, 0);
    layoutAjustes->addWidget(gainSlider, 1, 1);
    layoutAjustes->addWidget(gainLabel, 1, 2);

    histogramsContainer = new HistogramsContainer(this);
    histogramsContainer->setMaximumSize(400, 400);


    equalizeBtn = new QPushButton("Equalizar Histograma");
    histMatchBtn = new QPushButton("Match histograma");
    invertImageBtn = new QPushButton("Inverter imagem");

    grupoAjustes->setLayout(layoutAjustes);
    tonsLayout->addWidget(grupoAjustes);
    tonsLayout->addWidget(histogramsContainer);
    tonsLayout->addWidget(equalizeBtn);
    tonsLayout->addWidget(histMatchBtn);
    tonsLayout->addWidget(invertImageBtn);
    tonsLayout->addStretch();

    tabWidget->addTab(principais, "Básicas");
    tabWidget->addTab(tons, "Tons");

    kernelEditor = new KernelEditor(this);
    tabWidget->addTab(kernelEditor, "Filtros");

    // Conecta as ações definidas no .h aos respectivos botões
    connect(revertOriginal, &QPushButton::clicked, this, &Sidebar::revertToOriginal);

    connect(verticalMirror, &QPushButton::clicked, this, &Sidebar::mirrorVertically);
    connect(horizontalMirror, &QPushButton::clicked, this, &Sidebar::mirrorHorizontally);
    connect(turn90degreesBtn, &QPushButton::clicked, this, &Sidebar::turn90degrees);
    connect(turn90degreesAntiBtn, &QPushButton::clicked, this, &Sidebar::turn90degreesAnti);

    connect(zoomBtn, &QPushButton::clicked, this, &Sidebar::zoomIn);

    connect(converterCiza, &QPushButton::clicked, this, &Sidebar::tornarTonsDeCinza);
    connect(quantify, &QPushButton::clicked, this, [this](){
        int num_shades = spinTons->value();

        emit applyQuantization(num_shades);
    });

    connect(biasSlider, &QSlider::valueChanged, [this](int value){
        biasLabel->setText(QString::number(value));

        emit biasChanged(value);
    });

    connect(gainSlider, &QSlider::valueChanged, [this](int value){
        gainLabel->setText(QString::number(value));

        emit gainChanged(value);
    });

    connect(equalizeBtn, &QPushButton::clicked, this, &Sidebar::equalizeHistogram);
    connect(invertImageBtn, &QPushButton::clicked, this, &Sidebar::invertImage);
    connect(kernelEditor, &KernelEditor::applyKernelClicked, this, &Sidebar::requestKernelApply);

    connect(zoomOutBtn, &QPushButton::clicked, [this]() {
        double sx = outFactorX->value();
        double sy = outFactorY->value();
        emit requestZoomOut(sx, sy);
    });

    connect(histMatchBtn, &QPushButton::clicked, this, &Sidebar::histogramMatchRequest);
}

void Sidebar::updateHistogram(QVector<double> &data){
    //histogram->setHistogramData(data);
}
