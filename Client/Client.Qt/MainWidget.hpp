#pragma once

#include <QWidget>
#include <memory>

class QHBoxLayout;
class BioButton;
class CaptionButton;

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget* parent = nullptr);
    int  addWidget(std::unique_ptr<QWidget> widget);
    void setCentralWidget(std::int32_t index);

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
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

public:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    MouseType m_leftMouseButtonPressed;

    bool _mousePressed{};

    std::unique_ptr<CaptionButton> close_btn;
    std::unique_ptr<CaptionButton> maximize_btn;
    std::unique_ptr<CaptionButton> minimize_btn;

    std::int32_t _current = -1;

    std::unique_ptr<QWidget>              _body;
    std::unique_ptr<QHBoxLayout>          pTitleLayout;
    std::vector<std::unique_ptr<QWidget>> _widgets;
};
