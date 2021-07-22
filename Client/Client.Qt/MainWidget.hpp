#pragma once

#include <QWidget>
#include <memory>

#include "Widgets/BioButton.hpp"
#include "Widgets/CaptionButton.hpp"
#include "Widgets/TitleWidget.hpp"

class QHBoxLayout;
class BioButton;

/**
 * @class MainWidget
 * @brief Custom window for JuniorGram
 */
class MainWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for MainWidget
     */
    explicit MainWidget();
    /**
     * @brief Adds widget to window
     * @param widget Own-pointer to widget
     * @return widget index
     */
    int  addWidget(std::unique_ptr<QWidget> widget);
    /**
     * @brief Hide old widget and show a new one
     * @param index index of new widget
     */
    void setCentralWidget(std::int32_t index);
    /**
     * @brief change icon of bio button
     * @param icon new icon
     * @return Success=true
     */
    bool setBioButtonIcon(const Style::icon* icon);


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
    void      refreshTitleBar(bool showBioButton);

protected:
#ifdef _WIN32
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    MouseType                  _lmbPos = None;
    std::unique_ptr<BioButton> _bioButton;
    std::int32_t               _current = -1;

    std::unique_ptr<QWidget>              _body;
    std::unique_ptr<TitleWidget>          _title;
    std::vector<std::unique_ptr<QWidget>> _widgets;
};
