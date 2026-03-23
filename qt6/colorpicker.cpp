#include <mainwindow.hpp>
#include <QtGui/QFont>
#include <QtWidgets/QApplication>

int main(int argc, char* argv[]) {
    QFont        lato { "Lato", 12, QFont::Weight::Medium }; // use Lato throughout the app
    QApplication colorpicker(argc, argv);
    colorpicker.setWindowIcon(QIcon(R"(.\icons\icon.png)"));
    colorpicker.setFont(lato);
    ::main_window window {};
    window.show();

    return colorpicker.exec();
}
