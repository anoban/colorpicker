#pragma once

// title bars are handled by the OS and the desktop window managers (GNOME or KDE), hence Qt does not have much control over it
// to be able to customize it, we need first hide the OS dependent title bar and implement a pseudo title bar

#ifndef __TITLEBAR_HPP
    #define __TITLEBAR_HPP 1
#endif

#include <QtWidgets/QFrame>

class title_bar final : public QFrame {
        Q_OBJECT
        //

        
};
