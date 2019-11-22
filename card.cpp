#include <card.hpp>
#include <QBitmap>

Card::Card(const QString& imagePath, QWidget* parent)
    :
      QLabel(parent)
    , m_imagePath(imagePath)
{
    scale(1.0f);
}

Card::~Card()
{

}

void Card::scale(float factor)
{
    QPixmap pixmapOrig(m_imagePath);
    float ratio = pixmapOrig.width()/float(pixmapOrig.height());
    m_size = QSize(factor*ratio*100, factor*100);

    QPixmap pixmapMini = pixmapOrig.scaled(m_size);
    setPixmap(pixmapMini);
    setFixedSize(m_size);

    applyMask();
}

void Card::applyMask()
{
    QPixmap mask("/workspace/projects/Qt_based/cards_generator/images/mask.png");
    setMask(mask.scaled(m_size));
}
