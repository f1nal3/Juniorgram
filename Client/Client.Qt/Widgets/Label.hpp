#pragma once

#include <QLabel>

/** @class Label
 *  @brief Custom style for Label
 */
class Label : public QLabel
{
public:
    /**
     *  @brief Setting the style for Label objects
     */
    explicit Label(const QString &text = QString(), QWidget *parent = nullptr);
};
