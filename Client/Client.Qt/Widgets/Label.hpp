#pragma once

#include "pch.hpp"

class Label : public QLabel
{
public:
    Label(const QString &text = QString(), QWidget *parent = nullptr);
};
