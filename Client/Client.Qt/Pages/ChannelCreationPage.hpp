#pragma once

#include <Network/Primitives.hpp>
#include <QVBoxLayout>
#include <QWidget>

#include "Widgets/InputFields.hpp"
#include "Widgets/ListWidget.hpp"
#include "Widgets/Page.hpp"

class FlatButton;

/**
 * @class ChannelCreationPage
 * @brief Page to create channel
 */
class ChannelCreationPage : public Page
{
public:
    /// Default constructor
    explicit ChannelCreationPage(QWidget* parent = nullptr);

private:
    void updateLayout();

private:
    std::unique_ptr<FlatButton> _createButton;
    std::unique_ptr<FlatInput>  _channelNameInput;
};