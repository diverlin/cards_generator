#pragma once

#include <QFrame>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

class ImageWidget;

class ImagesPoolWidget : public QFrame
{
public:
    explicit ImagesPoolWidget(QWidget *parent = nullptr);
    void fill(const QList<QString>&);

protected:
    QSize sizeHint() const override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    std::vector<ImageWidget*> m_cards;
    int m_widthMax = 0;
    int m_heightMax = 0;

    void __reset();
};


