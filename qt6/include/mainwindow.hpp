#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

class trackbar_group final : public QGroupBox {
        Q_OBJECT

    private:
        QSlider*    _slider_red;
        QSlider*    _slider_green;
        QSlider*    _slider_blue;
        QBoxLayout* _slider_layout;

    public:
        inline trackbar_group(const char* const _title, QWidget* const _parent_window = nullptr) noexcept : QGroupBox(_title, _parent_window) { }
};

class window final : public QWidget {
        Q_OBJECT

    private:
        trackbar_group* _trackbar_group;

    public:
        inline window(QWidget* const _parent_window = nullptr) noexcept : QWidget(_parent_window) { }
};
