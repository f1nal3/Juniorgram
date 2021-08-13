#pragma once
#include <QFrame>

#include "LoadingSpinner.hpp"

/**
 * @class Page
 * @brief Class for gui-pages
 */
class Page : public QFrame
{
    Q_OBJECT
public:
    /// Constructor for gui-page
    explicit Page(QWidget* parent = nullptr);
public slots:
    /// Resume widget
    void onResume();
    /// Pause widget/scrolling
    void onPause();
signals:
    /// Returns back on navigator stack
    void back();
    /// Add new page on navigator stack and goes into
    void navigateTo(const QString& route);
    /// Replace current page with a new one with saving stack back
    void replace(const QString& route);
    /// Destroys all stack with a new route
    void newRootPage(const QString& route);

protected:
    /// Handle resize
    void resizeEvent(QResizeEvent* event) override;

private:
    std::unique_ptr<LoadingSpinner> _loading;
};
