#pragma once

#include <QLabel>

class OverlayLabel : public QLabel {
    Q_OBJECT
public:
    OverlayLabel(QWidget* parent = nullptr);
    ~OverlayLabel();

signals:
    void dirty();

protected:
    void moveEvent(QMoveEvent*) override final;
};
