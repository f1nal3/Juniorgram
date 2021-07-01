#pragma once

#include <QLabel>

/** @class Label
 *  @brief Custom style for Label
 */
class Label : public QLabel
{
public:
    explicit Label(const QString &text = QString(), QWidget *parent = nullptr);
};
