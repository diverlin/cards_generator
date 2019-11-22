#include "cardboard.hpp"
#include "imagewidget.hpp"
#include "overlaylabel.hpp"

#include <QtWidgets>

CardBoard::CardBoard(OverlayLabel* overlay, QWidget *parent)
    : QLabel(parent)
    , m_overlay(overlay)
{
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAcceptDrops(true);
}

CardBoard::~CardBoard()
{

}

void CardBoard::reset()
{
    for (QObject* child: children()) {
        ImageWidget *image = static_cast<ImageWidget*>(child);
        if (!image) {
            continue;
        }
        image->deleteLater();
    }
}

void CardBoard::save()
{
    QPixmap pixmap = grab(QRect(QPoint(0, 0), size()));
    QString path = qApp->applicationDirPath() + "/result_cards/";
    if (!QDir(path).exists()) {
        QDir().mkdir(path);
    }

    QDateTime now = QDateTime::currentDateTime();
    QString timestamp = now.toString(QLatin1String("yyyyMMddhhmmss"));

    path += QString("card_%1.png").arg(timestamp);

    if(pixmap.toImage().save(path)) {
        emit saved(path);
    } else {
        qCritical("Save Failed!");
    }
}

void CardBoard::gen(const QList<QString>& pack)
{
    reset();

    QPointF topLeft(0.25, 0.25);
    QPointF topRight(0.75, 0.25);
    QPointF middle(0.5, 0.5);
    QPointF bottomLeft(0.25, 0.75);
    QPointF bottomRight(0.75, 0.75);
    QList<QPointF> points;
    points = {
        topLeft
        , topRight
        , middle
        , bottomLeft
        , bottomRight
    };

    int i = 0;
    for (const QString& filePath: pack) {
        if (i>=5) {
            break;
        }
        ImageWidget* card = new ImageWidget(filePath, m_scaleFactor, this);
        QPointF p = points[i];
        p.setX(p.x()*width()-card->width()/2);
        p.setY(p.y()*height()-card->height()/2);
        card->move(p.x(), p.y());
        card->show();
        ++i;
    }

    emit dirty();
}

void CardBoard::applyScaleFactor(float factor)
{
    m_scaleFactor = factor;
    for (QObject* child: children()) {
        ImageWidget *image = static_cast<ImageWidget*>(child);
        if (!image) {
            continue;
        }
        image->applyScale(factor);
    }

    __markDirty();
}

void CardBoard::applyBackground(const QString& path)
{
    QPixmap pixmapOrig(path);
    QPixmap scaledPixmap = pixmapOrig.scaled(size());
    setPixmap(scaledPixmap);

    __markDirty();
}

void CardBoard::removeBackground()
{
    setPixmap(QPixmap(""));
    __markDirty();
}

void CardBoard::dragEnterEvent(QDragEnterEvent *event)
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

void CardBoard::applyForeground(const QString& path)
{
    m_foregroundPath = path;

    QPixmap pixmapOrig(path);
    QPixmap scaledPixmap = pixmapOrig.scaled(size());

    m_overlay->setGeometry(geometry());
    m_overlay->setPixmap(scaledPixmap);

    m_overlay->setVisible(true);

    __markDirty();
}

void CardBoard::removeForeground()
{
    m_overlay->setVisible(false);
//    m_overlay->setPixmap(QPixmap(""));
    __markDirty();
}


void CardBoard::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }

        emit dirty();

    } else {
        event->ignore();
    }
}

void CardBoard::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        qInfo() << event->source()->property("imagePath").toString();

        QPixmap pixmap;
        QPoint offset;
        QString imagePath;
        dataStream >> pixmap >> offset >> imagePath;

        ImageWidget *newIcon = new ImageWidget(imagePath, m_scaleFactor, this);
        newIcon->setPixmap(pixmap);
        newIcon->move(event->pos() - offset);
        newIcon->applyScale(m_scaleFactor);
        newIcon->show();
        newIcon->setAttribute(Qt::WA_DeleteOnClose);

        emit dirty();

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
void CardBoard::mousePressEvent(QMouseEvent *event)
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

void CardBoard::__markDirty()
{
    bool atLeastOneCardExists = false;
    for (QObject* child: children()) {
        ImageWidget *image = static_cast<ImageWidget*>(child);
        if (!image) {
            continue;
        }
        atLeastOneCardExists = true;
        break;
    }
    if (atLeastOneCardExists) {
        emit dirty();
    }
}
