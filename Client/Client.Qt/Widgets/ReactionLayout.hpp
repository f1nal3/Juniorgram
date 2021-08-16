#pragma once

#include <QWidget>
#include <QWidgetAction>
#include <Styles/StyleBasic.hpp>
#include <vector>

#include "Widgets/MenuItem.hpp"

class IconButton;

/**
 * @class ReactionLayout
 * @brief Layout reactions in message widget
 */
class ReactionLayout : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for reaction layout
     * @param parent Parent widget
     */
    explicit ReactionLayout(QWidget* parent, uint32_t maxWidth = -1, uint32_t spacing = 8, bool isMenu = false);

    /**
     * @brief Remodels layout
     */
    void remodelLayout();

    void setInfo(uint64_t channelId, uint64_t userId, uint64_t msgId)
    {
        _channelId = channelId;
        _userId    = userId;
        _msgId     = msgId;
    }

    /**
     * @brief Do we need show reactions with zero counts
     */
    void setZeroVisible(bool visible = true);

    void setMaxWidth(uint32_t maxWidth)
    {
        _maxWidth = maxWidth;
        recountSize();
    }

    /**
     * @brief Sets reactions model
     * @param model Model (First: reaction_id | Second: reacts)
     */
    void setReactionsModel(const std::map<uint32_t, uint32_t>& model);

    QSize layoutSize() const;

    /**
     * @brief Recalculates size of layout
     */
    void recountSize();

    static bool addIcon(uint32_t id, const Style::icon icon);

signals:
    void clicked(const CallbackData& data_);

private:
    uint64_t _channelId, _userId, _msgId;
    /// First: reaction_id | Second: reaction_button
    std::map<uint32_t, std::unique_ptr<IconButton>> _reactions;

    /// First: reaction_id | Second: reacts
    std::map<uint32_t, uint32_t> _reactionsModel;

    uint32_t _maxWidth;
    uint32_t _spacing;

    bool  _zeroVisible = false;
    bool  _isMenu      = false;
    QSize _sizes;
};
