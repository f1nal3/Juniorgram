#pragma once

#include <QVBoxLayout>
#include <QWidget>
#include <Style/Styles.hpp>
#include <memory>

#include "Widgets/TitleWidget.hpp"

class QHBoxLayout;

/**
 * @class MainWidget
 * @brief Custom window for JuniorGram
 */
class MainWidget : public QWidget
{
    Q_OBJECT
public:
    /// Constructor for main widget
    explicit MainWidget();
    /**
     * @brief Adds widget to window
     * @param widget Own-pointer to widget
     * @return widget index
     */
    int addWidget(std::unique_ptr<QWidget> widget);
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

    /// Returns edge from global position
    Qt::Edges edgesFromPos(const QPoint& pos);

    /**
     * @brief Re-layout title-bar
     * @param showBioButton Show bio button?
     */
    void refreshTitleBar(bool showBioButton);

protected:
#ifdef _WIN32
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif
    /// Handle paint
    void paintEvent(QPaintEvent* event) override;
    /// Handle resize
    void resizeEvent(QResizeEvent* event) override;
    /// Handle window events
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void updateCursor(const Qt::Edges edges);
    void restoreCursor();

private:
    std::int32_t _current        = -1;
    bool         _cursorOverride = false;

    std::unique_ptr<QVBoxLayout> _grid;

    std::unique_ptr<TitleWidget>          _title;
    std::unique_ptr<QWidget>              _body;
    std::vector<std::unique_ptr<QWidget>> _widgets;
};
