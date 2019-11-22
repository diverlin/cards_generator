#include <utils.hpp>

#include <QLayout>
#include <QWidget>

namespace utils {

void superCompactLayout(QLayout* layout)
{
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
}

void compactLayout(QLayout* layout)
{
    layout->setSpacing(5);
    layout->setContentsMargins(5,5,5,5);
}

void colorify(QWidget* widget, const QColor& color)
{
    QString css = QString("#%1 {background: %2;}").arg(widget->objectName()).arg(color.name());
    widget->setStyleSheet(css);
}

} // namespace utils
