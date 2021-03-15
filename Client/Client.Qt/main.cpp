#include <QApplication>
#include "mainwidget.h"
#include "pch.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWidget w;
    w.show();
    w.windowHandle()->handle();
#if _WIN32
    HWND handle = reinterpret_cast<HWND>(w->winId());
    LONG style  = ::GetWindowLong(handle, GWL_STYLE);
    ::SetWindowLong(handle, GWL_STYLE, style | WS_CAPTION | WS_MAXIMIZEBOX | WS_THICKFRAME);
    ::SetWindowPos(handle, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
    const MARGINS shadow{9, 9, 9, 9};
    //::DwmExtendFrameIntoClientArea(handle,&shadow);
    // w.showNormal();
#endif

    return app.exec();
}
