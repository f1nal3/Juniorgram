#pragma once
#include <QWidget>
#include <memory>

/**
 * @brief Adds logo to main screens
 */
class LogoWidget : public QWidget
{
    Q_OBJECT
public:
    /// Constructor for logo widget
    explicit LogoWidget(QWidget* parent = nullptr);

    /**
     * @brief Sets part for logo to take place in
     * @param percent Value in [0, 100]
     */
    void setPart(double percent);
    /// Update size of logo
    void updateSize();

protected:
    /// Handle paint
    void paintEvent(QPaintEvent* paintEvent) override;

private:
    /// Original size of logo
    [[nodiscard]] QSize bestFit() const { return _logo->size(); }

    // variable field
    std::unique_ptr<QPixmap> _logo;
    double                   _part = 40;
};
