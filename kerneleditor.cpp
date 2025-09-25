#include "kerneleditor.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>

KernelEditor::KernelEditor(QWidget *parent) : QWidget(parent){
    QVBoxLayout *mainLayout = new QVBoxLayout();

    m_filterComboBox = new QComboBox();
    m_filterComboBox->addItem("Personalizado");
    m_filterComboBox->addItem("Produzir borramento (Gaussiano)");  // Índice 0
    m_filterComboBox->addItem("Detectar Bordas (Laplaciano)");    // Índice 1
    m_filterComboBox->addItem("Detectar Bordas (intensivo)");  // Índice 2
    m_filterComboBox->addItem("Relevo Horizontal (Prewitt)");  // Índice 3
    m_filterComboBox->addItem("Relevo Horizontal (intensivo)");  // Índice 4
    m_filterComboBox->addItem("Relevo Vertical (Prewitt)");  // Índice 5
    m_filterComboBox->addItem("Relevo Vertical (intensivo)");  // Índice 6

    connect(m_filterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &KernelEditor::onFilterSelected);

    mainLayout->addWidget(new QLabel("Filtro Pré-definido:"));
    mainLayout->addWidget(m_filterComboBox);


    QGridLayout *matrixLayout = new QGridLayout();
    m_matrixCells.resize(9); // Reserva espaço para 9 ponteiros
    for (int i = 0; i < 9; ++i) {
        m_matrixCells[i] = new QDoubleSpinBox();
        m_matrixCells[i]->setRange(-100.0, 100.0); // Define uma faixa de valores
        m_matrixCells[i]->setDecimals(2);          // Número de casas decimais
        m_matrixCells[i]->setSingleStep(0.1);      // Incremento/decremento
        matrixLayout->addWidget(m_matrixCells[i], i / 3, i % 3); // Mapeia o índice linear (0-8) para a grade (0,0) a (2,2)
    }
    mainLayout->addLayout(matrixLayout);

    QPushButton *applyButton = new QPushButton("Aplicar Filtro");

    connect(applyButton, &QPushButton::clicked, [this]() {
        QVector<double> kernel;
        kernel.resize(9);

        for(int i = 0; i < 9; ++i) {
            kernel[i] = m_matrixCells[i]->value();
        }

        emit applyKernelClicked(kernel);
    });


    mainLayout->addWidget(applyButton);
    mainLayout->addStretch();
    this->setLayout(mainLayout);
}

void KernelEditor::onFilterSelected(int index)
{
    // Vetor para guardar os 9 valores do filtro
    QVector<double> values;

    switch(index) {
    case 1: // Gaussiano (Índice 1 no ComboBox)
        values = {0.0625f, 0.125f, 0.0625f,
                  0.125f,  0.25f,  0.125f,
                  0.0625f, 0.125f, 0.0625f};
        break;
    case 2: // Laplaciano
        values = { 0, -1,  0,
                  -1,  4, -1,
                  0, -1,  0};
        break;
    case 3: //
        values = {-1, -1, -1,
                  -1,  8, -1,
                  -1, -1, -1};
        break;
    case 4:
        values = {-1, 0, 1,
                  -1, 0, 1,
                  -1, 0, 1};
        break;
    case 5:
        values = {-1,  0, 1,
                  -2,  0, 2,
                  -1,  0, 1};

        break;
    case 6:
        values = {-1, -1, -1,
                  0,  0,  0,
                  1,  1,  1};
        break;
    case 7:
        values = {-1, -2, -1,
                  0,  0,  0,
                  1,  2,  1};
        break;
    case 0: // Personalizado (Índice 0)
    default:
        // Se for "Personalizado", não fazemos nada,
        // permitindo que o usuário edite os campos livremente.
        return;
    }
    setMatrixValues(values);
}

// Função auxiliar para preencher a matriz na tela
void KernelEditor::setMatrixValues(const QVector<double> &values)
{
    if (values.size() != 9) return;
    for(int i = 0; i < 9; ++i) {
        m_matrixCells[i]->setValue(values[i]);
    }
}
