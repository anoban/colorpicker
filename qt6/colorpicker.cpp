#include <mainwindow.hpp>
#include <QtWidgets/QApplication>

int main(int argc, char* argv[]) {
    QApplication colorpicker(argc, argv);
    colorpicker.setWindowIcon(QIcon(R"(.\icons\icon.png)"));
    ::main_window window {};
    window.show();

    return colorpicker.exec();
}
