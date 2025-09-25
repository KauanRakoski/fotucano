#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SideBar.h"
#include "ImageProcessor.h"

#include <QPushButton> // Widget de botão
#include <QLabel>      // Widget de rótulo (texto)
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QPixmap>
#include <QWidget>
#include <QMessageBox>
#include <QScrollArea>

#define IMG_DISPLAY_HEIGTH 750
#define IMG_DISPLAY_WIDTH 450

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/logo.png"));

    QWidget *centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    QHBoxLayout *container = new QHBoxLayout(centralWidget);
    container->setProperty("class", "sidebar");

    labelImagemOriginal = new QLabel("Carregue uma imagem em 'Arquivo > Abrir...'", this);
    labelImagemOriginal->setFrameStyle(QFrame::Panel);
    labelImagemOriginal->setAlignment(Qt::AlignCenter);

    labelImagemProcessada = new QLabel("A imagem processada aparecerá aqui", this);
    labelImagemProcessada->setFrameStyle(QFrame::Panel);
    labelImagemProcessada->setAlignment(Qt::AlignCenter);

    QScrollArea *scrollAreaOriginal = new QScrollArea;
    scrollAreaOriginal->setBackgroundRole(QPalette::Dark);
    scrollAreaOriginal->setWidget(labelImagemOriginal);
    scrollAreaOriginal->setWidgetResizable(true);

    QScrollArea *scrollAreaProcessada = new QScrollArea;
    scrollAreaProcessada->setBackgroundRole(QPalette::Dark);
    scrollAreaProcessada->setWidget(labelImagemProcessada);
    scrollAreaProcessada->setWidgetResizable(true);

    container->addWidget(scrollAreaOriginal, 1);
    container->addWidget(scrollAreaProcessada, 1);

    labelImagemProcessada->setObjectName("processedImageLabel");
    labelImagemOriginal->setObjectName("originalImageLabel");

    sidebar = new Sidebar();
    sidebar->setMaximumWidth(400);

    connect(sidebar, &Sidebar::tornarTonsDeCinza, this, &MainWindow::onConvertGray);
    connect(sidebar, &Sidebar::revertToOriginal, this, &MainWindow::onRevertToOriginal);
    connect(sidebar, &Sidebar::mirrorVertically, this, &MainWindow::onMirrorVertically);
    connect(sidebar, &Sidebar::mirrorHorizontally, this, &MainWindow::onMirrorHorizontally);
    connect(sidebar, &Sidebar::applyQuantization, this, &MainWindow::onQuantify);
    connect(sidebar, &Sidebar::biasChanged, this, &MainWindow::onBiasChange);
    connect(sidebar, &Sidebar::gainChanged, this, &MainWindow::onGainChange);
    connect(sidebar, &Sidebar::invertImage, this, &MainWindow::onInvertImage);
    connect(sidebar, &Sidebar::equalizeHistogram, this, &MainWindow::onEqualizeHistogram);
    connect(sidebar, &Sidebar::requestKernelApply, this, &MainWindow::onApplyKernel);
    connect(sidebar, &Sidebar::turn90degrees, this, &MainWindow::onTurn90Degrees);
    connect(sidebar, &Sidebar::turn90degreesAnti, this, &MainWindow::onTurn90DegreesAnti);
    connect(sidebar, &Sidebar::zoomIn, this, &MainWindow::onZoomIn);
    connect(sidebar, &Sidebar::requestZoomOut, this, &MainWindow::onZoomOut);
    connect(sidebar, &Sidebar::histogramMatchRequest, this, &MainWindow::onHistMatch);

    container->addWidget(sidebar, 1);
    criarMenus();
}

void MainWindow::abrirImagem (){
    QString nomeArquivo = QFileDialog::getOpenFileName(this,
                                                       "Abrir Imagem", "", "Arquivos de Imagem (*.png *.jpg *.jpeg *.bmp)");

    if (!nomeArquivo.isEmpty())
    {
        if (imagemOriginal.load(nomeArquivo))
        {
            QPixmap pixmapOriginal = QPixmap::fromImage(imagemOriginal);
            QSize targetSize(IMG_DISPLAY_WIDTH, IMG_DISPLAY_HEIGTH);
            QPixmap toDisplay = pixmapOriginal.scaled(targetSize,
                                                      Qt::KeepAspectRatio,
                                                      Qt::SmoothTransformation);

            imagemProcessada = imagemOriginal;

            labelImagemOriginal->setPixmap(toDisplay);
            this->setProcessedImage(imagemOriginal);
        }
    }
}

void MainWindow::salvarImagem(){
    if (imagemProcessada.isNull()){
        QMessageBox::warning(this, "Aviso", "Não há nenhuma imagem salvar.");
        return;
    }

    QString nomeArquivo = QFileDialog::getSaveFileName(this, "Salvar imagem", "imagem_editada", "Imagem JPEG (*.jpg)");

    if (nomeArquivo.isEmpty()) {
        return;
    }

    imagemProcessada = this->applyGainAndBias(imagemProcessada);

    if (!imagemProcessada.save(nomeArquivo, "JPG", 95)) {
        QMessageBox::critical(this, "Erro", "Não foi possível salvar a imagem no local especificado.");
    }
}

QImage MainWindow::applyGainAndBias (const QImage &source){
    QImage res(source.size(), source.format());

    res = processor->biasChange(source, biasValue);
    res = processor->gainChange(res, gainValue);

    return res;
}

void MainWindow::criarMenus(){
    QMenu *menuArquivo = menuBar()->addMenu("&Arquivo");

    QAction *acaoAbrir = new QAction("&Abrir...", this);
    acaoAbrir->setShortcut(QKeySequence::Open);

    menuArquivo->addSeparator();

    QAction *acaoSalvar = new QAction("&Salvar", this);
    acaoSalvar->setShortcut(QKeySequence::Save);

    connect(acaoAbrir, &QAction::triggered, this, &MainWindow::abrirImagem);
    connect(acaoSalvar, &QAction::triggered, this, &MainWindow::salvarImagem);

    menuArquivo->addAction(acaoAbrir);
    menuArquivo->addAction(acaoSalvar);
}

void MainWindow::onConvertGray(){
    if (imagemProcessada.isNull()) return;
    isGray = true;

    imagemProcessada = processor->tonsDeCinza(imagemProcessada);
    this->setProcessedImage(imagemProcessada);
}

void MainWindow::onRevertToOriginal(){
    if (imagemProcessada.isNull()) return;

    isGray = false;
    fitToWindow = true;
    biasValue = 0;
    gainValue = 1;

    imagemProcessada = imagemOriginal;
    this->setProcessedImage(imagemProcessada);
}

void MainWindow::onMirrorVertically(){
    if (imagemProcessada.isNull()) return;

    imagemProcessada = processor->espelharVertical(imagemProcessada);
    this->setProcessedImage(imagemProcessada);
}

void MainWindow::onMirrorHorizontally(){
    if (imagemProcessada.isNull()) return;

    imagemProcessada = processor->espelharHorizontal(imagemProcessada);
    this->setProcessedImage(imagemProcessada);
}

// Only applied to gray shade images
void MainWindow::onQuantify (int num_shades){
    if (imagemProcessada.isNull()) return;

    if (!isGray) this->onConvertGray();
    isGray = true;

    imagemProcessada = processor->quantizar(imagemProcessada, num_shades);
    this->setProcessedImage(imagemProcessada);
}

void MainWindow::setProcessedImage (QImage img){
    QImage toDisplay = this->applyGainAndBias(img);
    QPixmap imageDisplay = QPixmap::fromImage(toDisplay);

    QSize targetSize(IMG_DISPLAY_WIDTH, IMG_DISPLAY_HEIGTH);

    if (fitToWindow){
        imageDisplay = imageDisplay.scaled(targetSize,
                                                Qt::KeepAspectRatio,
                                                Qt::SmoothTransformation);
    }

    labelImagemProcessada->setPixmap(imageDisplay);

    if (imagemProcessada.isGrayscale()){
        int histBefore[256], histAfter[256];
        QVector<double> histBeforePerc = processor->HistogramInPercentages(processor->tonsDeCinza(imagemOriginal), histBefore);
        QVector<double> histAfterPerc = processor->HistogramInPercentages(this->applyGainAndBias(imagemProcessada), histAfter);
        sidebar->histogramsContainer->displayGrayscaleResults(histBeforePerc, histAfterPerc);
    }else {

        QVector<QVector<int>> histsRGB = processor->computeRGBHistograms(this->applyGainAndBias(imagemProcessada));

        double totalPixels = imagemProcessada.width() * imagemProcessada.height();
        QVector<double> rPerc(256), gPerc(256), bPerc(256);
        if (totalPixels > 0) {
            for(int i=0; i<256; ++i) rPerc[i] = (histsRGB[0][i] / totalPixels) * 100.0;
            for(int i=0; i<256; ++i) gPerc[i] = (histsRGB[1][i] / totalPixels) * 100.0;
            for(int i=0; i<256; ++i) bPerc[i] = (histsRGB[2][i] / totalPixels) * 100.0;
        }

        sidebar->histogramsContainer->displayColorResults(rPerc, gPerc, bPerc);
    }
}

void MainWindow::onBiasChange(int bias){
    if (imagemProcessada.isNull()) return;

    biasValue = bias;
    this->setProcessedImage(imagemProcessada);
}

void MainWindow::onGainChange(int gain){
    if (imagemProcessada.isNull()) return;

    gainValue = gain;
    this->setProcessedImage(imagemProcessada);
}

void MainWindow::onInvertImage(){
    if (imagemProcessada.isNull()) return;

    imagemProcessada = processor->negativeOfImage(imagemProcessada);
    this->setProcessedImage(imagemProcessada);
}

void MainWindow::onEqualizeHistogram(){
    if (imagemProcessada.isNull()) return;

    if (!isGray) {
        imagemProcessada = processor->equalizeColorHistogram(imagemProcessada);
    } else {
        imagemProcessada = processor->equalizeHistogram(imagemProcessada);
    }

    this->setProcessedImage(imagemProcessada);
}

void MainWindow::onApplyKernel(const QVector<double> &kernel){
    if (imagemProcessada.isNull()) return;

    imagemProcessada = processor->applyConvolutionWithoutBorders(imagemProcessada, kernel);
    this->setProcessedImage(imagemProcessada);
}

void MainWindow::onTurn90Degrees(){
    if (imagemProcessada.isNull()) return;

    imagemProcessada = processor->turn90degrees(imagemProcessada);
    this->setProcessedImage(imagemProcessada);
}

void MainWindow::onTurn90DegreesAnti(){
    if (imagemProcessada.isNull()) return;

    imagemProcessada = processor->turn90degreesAnti(imagemProcessada);
    this->setProcessedImage(imagemProcessada);
}

void MainWindow::onZoomIn(){
    if (imagemProcessada.isNull()) return;
    fitToWindow = false;

    imagemProcessada = processor->zoom2x(imagemProcessada);
    this->setProcessedImage(imagemProcessada);
}

void MainWindow::onZoomOut(double sx, double sy){
    if (imagemProcessada.isNull()) return;
    fitToWindow = false;

    imagemProcessada = processor->zoomOut(imagemProcessada, sx, sy);
    this->setProcessedImage(imagemProcessada);
}

void MainWindow::onHistMatch(){
    if (imagemProcessada.isNull()) return;

    if (!isGray) this->onConvertGray();
    isGray = true;

    QString nomeArquivo = QFileDialog::getOpenFileName(this,
                                                       "Abrir Imagem", "", "Arquivos de Imagem (*.png *.jpg *.jpeg *.bmp)");

    QImage targetImage;

    if (!nomeArquivo.isEmpty() && targetImage.load(nomeArquivo)){
        imagemProcessada = processor->histogramMatch(imagemProcessada, processor->tonsDeCinza(targetImage));
        this->setProcessedImage(imagemProcessada);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
