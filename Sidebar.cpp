#include "SideBar.h"

#include <Qwidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

Sidebar::Sidebar (QWidget *parent) : QWidget(parent) {
    QWidget *barraLateral = new QWidget(this);
    QVBoxLayout *layoutLateral = new QVBoxLayout(barraLateral);

    QLabel *editLabel = new QLabel("Opções de Edição", this);
    editLabel->setObjectName("editLabel");

    QGroupBox *grupoEspelhamento = new QGroupBox("Espelhamento");
    QVBoxLayout *layoutEspelhamento = new QVBoxLayout();

    verticalMirror = new QPushButton("Vertical");
    horizontalMirror = new QPushButton("Horizontal");
    layoutEspelhamento->addWidget(verticalMirror);
    layoutEspelhamento->addWidget(horizontalMirror);
    grupoEspelhamento->setLayout(layoutEspelhamento);

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
    layoutLateral->addWidget(grupoCinza);
    layoutLateral->addWidget(grupoQuantizacao);
    layoutLateral->addWidget(revertOriginal);
    layoutLateral->addStretch();

    // Conecta as ações definidas no .h aos respectivos botões
    connect(converterCiza, &QPushButton::clicked, this, &Sidebar::tornarTonsDeCinza);
    connect(revertOriginal, &QPushButton::clicked, this, &Sidebar::revertToOriginal);
    connect(verticalMirror, &QPushButton::clicked, this, &Sidebar::mirrorVertically);
    connect(horizontalMirror, &QPushButton::clicked, this, &Sidebar::mirrorHorizontally);
    connect(quantify, &QPushButton::clicked, this, [this](){
        int num_shades = spinTons->value();

        emit applyQuantization(num_shades);
    });
}
