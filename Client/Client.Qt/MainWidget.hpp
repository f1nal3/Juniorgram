#pragma once

#include <QWidget>
#include <memory>

#include "TitleWidget.hpp"
#include "Widgets/CaptionButton.hpp"

class QHBoxLayout;
class BioButton;

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget* parent = nullptr);
    int  addWidget(std::unique_ptr<QWidget> widget);
    void setCentralWidget(std::int32_t index);

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
        Right
    };

    MouseType checkResizableField(QMouseEvent* event);
    void      refreshTitleBar(BioButton* bio_button = nullptr);

protected:
#ifdef _WIN32
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

public:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    MouseType                  _lmbPos = None;
    std::unique_ptr<BioButton> _bioButton;
    std::int32_t               _current = -1;

    std::unique_ptr<QWidget>              _body;
    std::unique_ptr<TitleWidget>          _title;
    std::vector<std::unique_ptr<QWidget>> _widgets;
};
