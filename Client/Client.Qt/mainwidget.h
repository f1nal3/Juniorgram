//
// Created by Stanislav on 17.02.2021.
//

#ifndef JUNIORGRAMUI_MAINWIDGET_H
#define JUNIORGRAMUI_MAINWIDGET_H

#include "Widgets/caption_button.h"
#include "pch.h"

class MainWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QPoint previousPosition MEMBER m_previousPosition)
public:
    explicit MainWidget(QWidget* parent = nullptr);
#ifdef _WIN32

    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;

#endif

    enum MouseType
    {
        None = 0,
        Top,
        TopLeft,
        TopRight,
        Bottom,
        BottomLeft,
        BottomRight,
        Left,
        Right,
        Move
    };

    MouseType checkResizableField(QMouseEvent* event);

protected:
    void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

public:
    ~MainWidget() override
    {
        delete close_btn;
        delete maximize_btn;
        delete minimize_btn;
        delete pTitleLayout;
        delete body;
    }

    void setCentralWidget(QWidget* widget = nullptr);

private:
    MouseType m_leftMouseButtonPressed;
    QPoint m_previousPosition;

    bool _mousePressed{};

    CaptionButton* close_btn;
    CaptionButton* maximize_btn;
    CaptionButton* minimize_btn;

    QWidget* body;
    QHBoxLayout* pTitleLayout;
};

#endif  // JUNIORGRAMUI_MAINWIDGET_H
