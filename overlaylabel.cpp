#include <overlaylabel.hpp>

OverlayLabel::OverlayLabel(QWidget* parent)
    : QLabel(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

OverlayLabel::~OverlayLabel()
{

}

