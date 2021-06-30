#pragma once
#include <QWidget>

class LogoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogoWidget(QWidget* parent = nullptr);
    ~LogoWidget();

    [[nodiscard]] QSize bestFit() const { return logo->size(); }

protected:
    void paintEvent(QPaintEvent* paintEvent) override;

private:
    QPixmap* logo;
};
