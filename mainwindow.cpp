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

    container->addWidget(labelImagemOriginal);
    container->addWidget(labelImagemProcessada);

    labelImagemProcessada->setObjectName("processedImageLabel");
    labelImagemOriginal->setObjectName("originalImageLabel");

    sidebar = new Sidebar();
    sidebar->setMaximumWidth(250);

    connect(sidebar, &Sidebar::tornarTonsDeCinza, this, &MainWindow::onConvertGray);
    connect(sidebar, &Sidebar::revertToOriginal, this, &MainWindow::onRevertToOriginal);
    connect(sidebar, &Sidebar::mirrorVertically, this, &MainWindow::onMirrorVertically);
    connect(sidebar, &Sidebar::mirrorHorizontally, this, &MainWindow::onMirrorHorizontally);
    connect(sidebar, &Sidebar::applyQuantization, this, &MainWindow::onQuantify);

    container->addWidget(sidebar);
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
            labelImagemProcessada->setPixmap(toDisplay);
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

    if (!imagemProcessada.save(nomeArquivo, "JPG", 95)) {
        QMessageBox::critical(this, "Erro", "Não foi possível salvar a imagem no local especificado.");
    }
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
    if (!isGray) this->onConvertGray();
    isGray = true;

    imagemProcessada = processor->quantizar(imagemProcessada, num_shades);
    this->setProcessedImage(imagemProcessada);
}

void MainWindow::setProcessedImage (QImage img){
    QPixmap imageDisplay = QPixmap::fromImage(img);

    QSize targetSize(IMG_DISPLAY_WIDTH, IMG_DISPLAY_HEIGTH);
    QPixmap toDisplay = imageDisplay.scaled(targetSize,
                              Qt::KeepAspectRatio,
                              Qt::SmoothTransformation);

    labelImagemProcessada->setPixmap(toDisplay);
}
MainWindow::~MainWindow()
{
    delete ui;
}
