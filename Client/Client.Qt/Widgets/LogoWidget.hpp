#pragma once
#include <QWidget>
#include <memory>

class LogoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogoWidget(QWidget* parent = nullptr);

    [[nodiscard]] QSize bestFit() const { return logo->size(); }

protected:
    void paintEvent(QPaintEvent* paintEvent) override;

private:
    std::unique_ptr<QPixmap> logo;
};
