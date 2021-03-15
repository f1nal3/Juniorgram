//
// Created by Stanislav on 10.03.2021.
//

#include "style.h"

namespace Style {

    namespace {

        int ScaleValue = defDPI;

    }

    void setDpiScale(int factor) {
        if (factor < minDPI) factor = minDPI;
        if (factor > maxDPI) factor = maxDPI;
        ScaleValue = factor;
    }

    int getDpiScale() {
        return ScaleValue;
    }

    int WindowsScaleDPIValue(int value) {
        double res = static_cast<double>(value) * static_cast<double>(ScaleValue) / 100.0;
        return static_cast<int>(res + (fmod(res, 10.0) >= 5.0 ? 0.5 : 0.0));
    }
}