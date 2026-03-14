#pragma once
#ifndef __TRACKBAR_GROUP_HPP
    #define __TRACKBAR_GROUP_HPP 1
#endif

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

class trackbar_grid final : public QGroupBox {
        Q_OBJECT

    private:
        QSlider*    _slider_red;
        QSlider*    _slider_green;
        QSlider*    _slider_blue;
        QBoxLayout* _slider_layout;

    public:
        inline trackbar_grid(const char* const _title, QWidget* const _parent_window = nullptr) noexcept : QGroupBox(_title, _parent_window) { }
};
