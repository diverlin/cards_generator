#include <overlaylabel.hpp>

OverlayLabel::OverlayLabel(QWidget* parent)
    : QLabel(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

OverlayLabel::~OverlayLabel()
{

}

void OverlayLabel::moveEvent(QMoveEvent* event)
{
//    emit dirty();
//    QLabel::moveEvent(event);
}
