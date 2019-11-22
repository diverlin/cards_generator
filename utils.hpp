#pragma once

class QLayout;
class QWidget;
class QColor;

namespace utils {

void superCompactLayout(QLayout* layout);
void compactLayout(QLayout* layout);
void colorify(QWidget* widget, const QColor& color);

} // namespace utils
