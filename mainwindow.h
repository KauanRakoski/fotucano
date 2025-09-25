#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

#include "sidebar.h"
#include "imageprocessor.h"

// O namespace Ui é usado pelo Qt Designer
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Pré-declarações
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void abrirImagem(); // Slot para a ação de abrir arquivo

    void onConvertGray();
    void onRevertToOriginal();
    void onMirrorVertically();
    void onTurn90Degrees();
    void onTurn90DegreesAnti();

    void onZoomIn();
    void onZoomOut(double sx, double sy);

    void onMirrorHorizontally();
    void onQuantify(int num_shades);

    void onBiasChange(int bias);
    void onGainChange(int gain);
    QImage applyGainAndBias (const QImage &source);

    void onInvertImage();
    void onEqualizeHistogram();
    void onHistMatch();

    void onApplyKernel(const QVector<double> &kernel);


    void salvarImagem();

private:
    bool isGray = false;
    bool fitToWindow = true;
    void criarMenus(); // Função auxiliar para manter o construtor limpo
    void setProcessedImage(QImage img);

    // Ponteiros para os widgets que criaremos via código
    QLabel *labelImagemOriginal;
    QLabel *labelImagemProcessada;

    // Objeto para armazenar os dados da imagem
    QImage imagemOriginal;
    QImage imagemProcessada;

    Sidebar *sidebar;
    ImageProcessor *processor;

    // Ponteiro para a interface criada no Qt Designer
    Ui::MainWindow *ui;

    int biasValue = 0;
    int gainValue = 1;
};
#endif // MAINWINDOW_H
