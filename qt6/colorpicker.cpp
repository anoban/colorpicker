// clang-format off
#include <mainwindow.hpp>
// clang-format on

#include <QtGui/QFont>
#include <QtWidgets/QApplication>

int main(int argc, char* argv[]) {
    QFont lato { "Lato", configs::FONTSIZE, QFont::Weight::Bold }; // use Lato throughout the app
    lato.setStyle(QFont::Style::StyleNormal);
    lato.setStretch(QFont::Stretch::Unstretched);

    QApplication colorpicker(argc, argv);
    // colorpicker.setWindowIcon(QIcon(R"(.\icons\colorpicker.png)"));
    colorpicker.setFont(lato);
    ::main_window window {};
    window.show();

    return colorpicker.exec();
}
