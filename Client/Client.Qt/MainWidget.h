#pragma once

#include <Widgets/BioButton.hpp>

#include "Widgets/CaptionButton.hpp"
#include "pch.hpp"

class MainWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QPoint previousPosition MEMBER m_previousPosition)
public:
    explicit MainWidget(QWidget* parent = nullptr);
    void setCentralWidget(QWidget* widget = nullptr);
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
    void refreshTitleBar(BioButton* bio_button = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void showEvent(QShowEvent* event) override;

public:
    bool eventFilter(QObject* watched, QEvent* event) override;

public:
    ~MainWidget() override
    {
        delete close_btn;
        delete maximize_btn;
        delete minimize_btn;
        delete pTitleLayout;
        delete body;
    }

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