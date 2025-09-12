#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <Qwidget>
#include <QPushButton>
#include <QSpinBox>

class Sidebar : public QWidget {
    Q_OBJECT

public:
    explicit Sidebar (QWidget *parent = nullptr);

private:
    QPushButton *verticalMirror;
    QPushButton *horizontalMirror;
    QPushButton *converterCiza;
    QPushButton *revertOriginal;
    QPushButton *quantify;
    QSpinBox *spinTons;

signals:
    void mirrorVertically();
    void mirrorHorizontally();
    void tornarTonsDeCinza();
    void applyQuantization(int num_shades);
    void revertToOriginal();
};

#endif // SIDEBAR_H
