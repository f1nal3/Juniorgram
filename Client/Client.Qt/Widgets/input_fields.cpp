//
// Created by Stanislav on 11.03.2021.
//

#include "input_fields.h"

FlatInput::FlatInput(QWidget* parent) : QLineEdit(parent)
{
    setFont(QFont("Noto Sans", Style::WindowsScaleDPIValue(13)));
    // This is not not good
    QColor qg(0x2f, 0x32, 0x41);
    auto gg               = qg.lighter(175);
    auto* regexpvalidator = new QRegExpValidator;
    regexpvalidator->setRegExp(QRegExp("[a-zA-Z0-9]{32}"));
    setValidator(regexpvalidator);
    setStyleSheet(QString("QLineEdit { "
                          "border: 0px;"
                          "border-radius: %7px;"
                          "selection-background-color: rgb(%1, %2, %3);"
                          "background: rgb(%4,%5,%6);"
                          "padding:%7px;"
                          "color:white;"
                          "}")
                      .arg(gg.red())
                      .arg(gg.green())
                      .arg(gg.blue())
                      .arg(qg.red())
                      .arg(qg.green())
                      .arg(qg.blue())
                      .arg(Style::WindowsScaleDPIValue(5)));
}
