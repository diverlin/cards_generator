#include <imagewidget.hpp>
#include "colors.hpp"

#include <QBitmap>
#include <QVariant>
#include <QGraphicsDropShadowEffect>

ImageWidget::ImageWidget(const QString& imagePath, QWidget* parent)
    :
      QLabel(parent)
    , m_imagePath(imagePath)
{
    scale(1.0f);
    setStyleSheet(QString("border: 3x solid %1; border-radius: 8px;").arg(COLOR_DARK.name()));

    setProperty("imagePath", imagePath);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(15);
    effect->setXOffset(5);
    effect->setYOffset(5);
    effect->setColor(COLOR_DARK);

    setGraphicsEffect(effect);
}

ImageWidget::~ImageWidget()
{

}

void ImageWidget::scale(float factor)
{
    QPixmap pixmapOrig(m_imagePath);
    float ratio = pixmapOrig.width()/float(pixmapOrig.height());
    m_size = QSize(factor*ratio*100, factor*100);

    QPixmap pixmapMini = pixmapOrig.scaled(m_size);
    setPixmap(pixmapMini);
    setFixedSize(m_size);
}


