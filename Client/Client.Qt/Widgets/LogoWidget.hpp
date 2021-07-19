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
    /**
     * @brief Builds new logo widget
     * @param parent Parent widget
     */
    explicit LogoWidget(QWidget* parent = nullptr);

    /**
     * @brief Original size of logo
     * @return Original size of logo
     */
    [[nodiscard]] QSize bestFit() const { return _logo->size(); }
    /**
     * @brief Sets part for logo to take place in
     * @param percent Value in [0, 100]
     */
    void  setPart(int percent);
    /**
     * @brief Recalculates size of widget if parent widget geometry changed
     */
    void  recountSize();

protected:
    void paintEvent(QPaintEvent* paintEvent) override;

private:
    std::unique_ptr<QPixmap> _logo;
    int                      _partInPercent;
};
