#pragma once

#include <QHBoxLayout>
#include <QWidget>
#include <memory>

#include "Widgets/BioButton.hpp"
#include "Widgets/CaptionButton.hpp"

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
    void      refreshTitleBar(BioButton* bio_button = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void showEvent(QShowEvent* event) override;

public:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    MouseType m_leftMouseButtonPressed;
    QPoint    m_previousPosition;

    bool _mousePressed{};

    std::unique_ptr<CaptionButton> close_btn;
    std::unique_ptr<CaptionButton> maximize_btn;
    std::unique_ptr<CaptionButton> minimize_btn;

    std::unique_ptr<QWidget> body;
    std::unique_ptr <QHBoxLayout> pTitleLayout;
};
