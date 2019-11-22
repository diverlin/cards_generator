#include <imagespoolwidget.hpp>

#include <imagewidget.hpp>

#include <QtWidgets>

ImagesPoolWidget::ImagesPoolWidget(QWidget *parent)
    : QFrame(parent)
{
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
}

void ImagesPoolWidget::__reset()
{
    for (ImageWidget* card: m_cards) {
        card->deleteLater();
    }
    m_cards.clear();
}

QSize ImagesPoolWidget::sizeHint() const
{
    QSize size(std::max(width(), m_widthMax), std::max(height(), m_heightMax));
    qInfo()<<"sizeHint="<<size;
    return size;
}

void ImagesPoolWidget::fill(const QList<QString>& files)
{
    __reset();

    const int border = 5;
    QPoint pos(border, border);
    int row_height = 0;
    ImageWidget* prevCardInRow = nullptr;

    for (const QString& file: files) {
        ImageWidget* card = new ImageWidget(file, 1.0f, this);

        int card_width = card->width();
        int card_height = card->height();

        row_height = std::max(row_height, card_height);
        int counter = m_cards.size();
        if (pos.x() <= (width() - 1.5*card_width)) {
            if (prevCardInRow) {
                pos = QPoint(pos.x() + prevCardInRow->width() + 2*border, pos.y());
                m_widthMax = std::max(m_widthMax, pos.x()+2*border+card->width());
            }
        } else {
            pos = QPoint(border, pos.y()+row_height+2*border);
            row_height = 0;
            prevCardInRow = nullptr;
        }
        card->move(pos);

        prevCardInRow = card;
        card->show();

        m_cards.push_back(card);
    }

    m_heightMax = pos.y();
    setFixedSize(m_widthMax, m_heightMax);
}

void ImagesPoolWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void ImagesPoolWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

//! [1]
void ImagesPoolWidget::mousePressEvent(QMouseEvent *event)
{
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if (!child)
        return;

    QPixmap pixmap = *child->pixmap();
    QString imagePath = child->property("imagePath").toString();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pixmap << QPoint(event->pos() - child->pos()) << imagePath;
//! [1]

//! [2]
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);
//! [2]

//! [3]
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos() - child->pos());
//! [3]

    QPixmap tempPixmap = pixmap;
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
    painter.end();

    child->setPixmap(tempPixmap);

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
        child->close();
    } else {
        child->show();
        child->setPixmap(pixmap);
    }
}
