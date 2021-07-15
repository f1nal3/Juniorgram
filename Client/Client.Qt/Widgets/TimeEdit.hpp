#pragma once

#include <QTimeEdit>
#include <memory>

/** @class TimeEdit
 *  @brief Custom style for TimeEdit object
 */
class TimeEdit : public QTimeEdit
{
public:
    TimeEdit(QWidget* parent = nullptr);
};
