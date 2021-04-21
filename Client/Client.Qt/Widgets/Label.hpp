#pragma once

#include "pch.hpp"

/** @class Label
 *  @brief Custom style for Label
 */
class Label : public QLabel
{
public:
    Label(const QString &text = QString(), QWidget *parent = nullptr);
};
