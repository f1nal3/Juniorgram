#pragma once
#include <QWidget>
#include <memory>

class LogoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogoWidget(QWidget* parent = nullptr);

    QSize bestFit() const { return logo->size(); }
    void  setPart(int percent);
    void  recountSize();

protected:
    void paintEvent(QPaintEvent* paintEvent) override;

private:
    std::unique_ptr<QPixmap> logo;
    int                      part;
};
