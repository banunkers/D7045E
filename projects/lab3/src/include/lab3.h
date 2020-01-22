#pragma once

#include "config.h"
#include "core/app.h"
#include "render/window.h"

class Lab3 : public Core::App {
    public:
        Lab3();
        ~Lab3();

        bool Open();
        void Run();

    private:
        Display::Window* window;
};