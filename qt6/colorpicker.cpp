#include <mainwindow.hpp>
#include <QtWidgets/QApplication>

int main(int argc, char* argv[]) {
    QApplication colorpicker(argc, argv);
    colorpicker.setWindowIcon(QIcon("../archived/resources/icon.ico"));
    ::main_window window {};
    window.show();

    return colorpicker.exec();
}
