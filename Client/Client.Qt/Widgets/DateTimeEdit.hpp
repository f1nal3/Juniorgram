#pragma once

#include <QDateTimeEdit>

/** @class QDateTimeEdit
 *  @brief Custom style for QDateTimeEdit object
 */
class DateTimeEdit : public QDateTimeEdit
{
public:
    explicit DateTimeEdit(QWidget* parent = nullptr);
};
