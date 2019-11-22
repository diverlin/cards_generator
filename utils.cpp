#include <utils.hpp>

#include <QLayout>

void compactLayout(QLayout* layout)
{
    layout->setSpacing(5);
    layout->setContentsMargins(5,5,5,5);
}
