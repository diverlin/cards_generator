#pragma once

#include <QLabel>

class OverlayLabel : public QLabel {
    Q_OBJECT
public:
    OverlayLabel(QWidget* parent = nullptr);
    ~OverlayLabel();
};
