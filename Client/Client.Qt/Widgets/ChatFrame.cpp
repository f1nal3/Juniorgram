#include "ChatFrame.h"
ChatFrame::ChatFrame(QWidget* parent) : QWidget(parent) { textEdit = new TextEdit(this); }
ChatFrame::~ChatFrame() { delete textEdit; }
