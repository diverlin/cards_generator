#include <imagewidget.hpp>
#include <QBitmap>
#include <QVariant>

ImageWidget::ImageWidget(const QString& imagePath, QWidget* parent)
    :
      QLabel(parent)
    , m_imagePath(imagePath)
{
    scale(1.0f);
    setStyleSheet("border: 6px solid #505050; border-radius: 8px;");

    setProperty("imagePath", imagePath);
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


