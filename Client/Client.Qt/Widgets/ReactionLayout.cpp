#include "ReactionLayout.hpp"

#include <QDebug>

#include "Widgets/Buttons.hpp"

namespace
{
    std::map<uint32_t, const Style::icon> _icons;
}


ReactionLayout::ReactionLayout(QWidget* parent, const uint32_t maxWidth, const uint32_t spacing, bool isMenu)
    : QWidget(parent), _maxWidth(maxWidth), _spacing(Style::valueDPIScale(isMenu ? 4 : spacing)), _isMenu(isMenu)
{
    if (isMenu)
    {
        for (const auto& item : _icons)
        {
            _reactionsModel.insert({item.first, 0});
        }
        remodelLayout();
    }
}

void ReactionLayout::recountSize()
{
    if (_reactions.empty())
    {
        _sizes = QSize(0, 0);
        return;
    }
    QSize    result;
    uint32_t width  = 0;
    uint32_t height = 0;
    for (const auto& reaction : _reactions)
    {
        height = reaction.second->height();
        if (width + reaction.second->minimumWidth() <= _maxWidth)
        {
            reaction.second->move(width, result.rheight());
            width += reaction.second->width() + _spacing;
        }
        else
        {
            width -= _spacing;
            result.rwidth() = std::max(width, uint32_t(result.width()));
            width           = 0;
            result.rheight() += height + _spacing;
        }
    }
    if (width != 0)
    {
        width -= _spacing;
        result.rwidth() = std::max(width, uint32_t(result.width()));
        result.rheight() += height;
    }

    if (result.width() >= 0) setMinimumSize(result);
    _sizes = result;
}

QSize ReactionLayout::layoutSize() const { return _sizes; }

bool ReactionLayout::addIcon(uint32_t id, const Style::icon icon)
{
    const auto [it, success] = _icons.insert({id, icon});
    return success;
}

void ReactionLayout::setReactionsModel(const std::map<uint32_t, uint32_t>& model)
{
    if (!_isMenu)
    {
        _reactionsModel = model;
        remodelLayout();
    }
}

void ReactionLayout::setZeroVisible(bool visible)
{
    _zeroVisible = visible;
    remodelLayout();
}

void ReactionLayout::remodelLayout()
{
    /// Item (First: reaction_id | Second: reacts)
    for (const auto& item : _reactionsModel)
    {
        const auto reaction = _reactions.find(item.first);
        if (reaction != _reactions.end())
        {
            if (!_zeroVisible && item.second == 0)
            {
                _reactions.erase(reaction);
            }
        }
        else
        {
            if (_isMenu)
            {
                _reactions[item.first] =
                    std::make_unique<IconButton>(this, QString(""), st::reactionIconButtonInMenu);

                _reactions[item.first]->setIcon(&_icons[item.first]);
                _reactions[item.first]->setClickCallback([item, this]() { this->clicked({ nullptr, y(), 0, true });
                                                                 emit this->onClick(item.first); });
            }

            else if (_zeroVisible || item.second > 0)
            {
                _reactions[item.first] = std::make_unique<IconButton>(this, QString("%1").arg(item.second), st::reactionIconButton);

                _reactions[item.first]->setIcon(&_icons[item.first]);
                _reactions[item.first]->setClickCallback([item, this]() { this->clicked({ nullptr, y(), 0, true }); 
                                                                 emit this->onClick(item.first); });
            }
        }
    }

    recountSize();
}
