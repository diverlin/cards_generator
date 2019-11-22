#include <imagewidget.hpp>
#include "colors.hpp"

#include <QBitmap>
#include <QVariant>
#include <QGraphicsDropShadowEffect>

ImageWidget::ImageWidget(const QString& imagePath, float scale, QWidget* parent)
    :
      QLabel(parent)
    , m_imagePath(imagePath)
{
    applyScale(scale);
    setStyleSheet(QString("border: 3px solid %1; border-radius: 8px;").arg(COLOR_DARK.name()));

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

void ImageWidget::applyScale(float factor)
{
    QPixmap pixmapOrig(m_imagePath);
    float ratio = pixmapOrig.width()/float(pixmapOrig.height());
    QSize prevSize = m_size;
    m_size = QSize(factor*ratio*100, factor*100);

    QPixmap pixmapMini = pixmapOrig.scaled(m_size);
    setPixmap(pixmapMini);
    setFixedSize(m_size);

    QSize deltaSize = prevSize - m_size;
    QPoint pos(x(), y());
    pos.setX(pos.x() + deltaSize.width()/2);
    pos.setY(pos.y() + deltaSize.height()/2);
    move(pos);
}


