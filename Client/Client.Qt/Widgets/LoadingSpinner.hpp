#pragma once
#include <QColor>
#include <QTimer>
#include <QWidget>
#include <Style/StyleBasic.hpp>

/**
 * @class Loading spinner
 * @brief Widget which blocks page with spinner animation
 */
class LoadingSpinner : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for loading spinner
     * @param parent Page
     * @param disableParentWhenSpinning Disable parent widget?
     * @param st Styles
     */
    explicit LoadingSpinner(QWidget* parent = nullptr, bool disableParentWhenSpinning = true,
                            const Style::LoadingSpinner& st = st::defaultLoadingSpinner);

    /// Is running?
    [[nodiscard]] bool isSpinning() const;

public slots:
    /// Blocks page and shows spinner animation
    void start();
    /// Unblocks page and hides animation
    void stop();

protected:
    /// Handle painting
    void paintEvent(QPaintEvent* paintEvent) override;

private slots:
    /// Rotate spinner
    void rotate();

private:
    /// Fade the color
    [[nodiscard]] QColor currentLineColor(int distance, int numOfLines, qreal trailFadePerc, QColor color) const;

    /// Update size
    void updateSize();
    /// Put on center of page
    void updatePosition();

private:
    const Style::LoadingSpinner& _st;
    std::unique_ptr<QTimer>      _timer;
    bool                         _disableParentWhenSpinning;
    int32_t                      _currentCounter;
    bool                         _isSpinning;
};
