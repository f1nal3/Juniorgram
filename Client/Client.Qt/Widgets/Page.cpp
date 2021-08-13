#include "Page.hpp"

#include <ConnectionManager.hpp>
#include <QDebug>

Page::Page(QWidget* parent) : QFrame(parent)
{
    _loading = std::make_unique<LoadingSpinner>(this);
    /// TODO: disconnect notification
    connect(ReceiverManager::instance(), &ReceiverManager::onDisconnect, this, &Page::onResume);
}

void Page::onResume() { _loading->stop(); }

void Page::onPause() { _loading->start(); }

void Page::resizeEvent(QResizeEvent*) { _loading->resize(size()); }
