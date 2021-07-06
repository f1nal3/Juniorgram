#pragma once
#include <QWidget>

class MenuItem : public QWidget
{
    Q_OBJECT
public:
    explicit MenuItem(const QString& text, QAction* action, QWidget* parent = nullptr);

protected:
    [[nodiscard]] QSize sizeHint() const override;
    void mouseMoveEvent(QMouseEvent* mouseEvent) override;
    void leaveEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* paintEvent) override;
    void mouseReleaseEvent(QMouseEvent* mouseEvent) override;

private:
    bool hit{};
    QString innerText;
    QAction* mAction;
};
