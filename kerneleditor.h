#ifndef KERNELEDITOR_H
#define KERNELEDITOR_H

#include <QWidget>

class QDoubleSpinBox;
class QComboBox;

class KernelEditor : public QWidget{
    Q_OBJECT

public:
    explicit  KernelEditor(QWidget *parent = nullptr);

signals:
    void applyKernelClicked(const QVector<double> &kernel);

private:
    void setMatrixValues(const QVector<double> &values);
    void onFilterSelected(int index);

    QComboBox* m_filterComboBox;
    QVector<QDoubleSpinBox*> m_matrixCells;

};

#endif // KERNELEDITOR_H
