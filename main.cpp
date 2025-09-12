#include "mainwindow.h"

#include <QApplication>
#include <QString>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;

    QString styleSheet = R"(
    QWidget {
        background-color: #383838;
    }
    QMenuBar {
        border-bottom: 1px solid #282828;
    }
    QLabel {
        font-size: 15px;
    }
    QGroupBox {
        border: 1px solid #5A5A5A;
        border-radius: 5px;
        width: 200px;
        margin-top: 1ex;
        padding: 10px 0 2px 0;
    }
    QGroupBox::title {
        font-size: 15px;
        font-weight: bold;
        subcontrol-origin: margin;
        padding: 0 3px;
    }
    QPushButton {
        background-color: #161821;
        color: #DFE1E2;
        border-radius: 4px;
        padding: 5px;
        font-size: 12px;
        outline: none;
        border: none;
    }
    QPushButton::hover {
        background-color: #000000;
    }
    QLabel#processedImageLabel {
        background-color: #282828;
    }
    QLabel#originalImageLabel {
        background-color: #282828;
    }
    QLabel#editLabel {
        font-size: 20px;
        font-weight: 600;
        margin-bottom: 15px;
    }
)";

    app.setStyleSheet(styleSheet);

    window.setWindowTitle("Fotucano");
    window.resize(1200, 800);
    window.show();
    return app.exec();
}
