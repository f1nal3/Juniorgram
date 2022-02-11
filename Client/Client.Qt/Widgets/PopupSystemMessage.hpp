#pragma once

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>

class PopupSystemMessage : public QWidget
{
    Q_OBJECT

    // translucency property
    Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)

    private slots:
        void hideAnimation();
        void hide();

    protected:
        void paintEvent(QPaintEvent* event);

    public:
        explicit PopupSystemMessage(QWidget* parent = nullptr);

    public slots:
        void setPopupText(const QString& text);
        void show();

      private:
        QLabel             label;
        QGridLayout        layout;
        QPropertyAnimation animation;
        float              popupOpacity;
        QTimer*            timer;

        void  setPopupOpacity(float opacity);
        float getPopupOpacity() const;
};
