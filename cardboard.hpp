#pragma once

#include <QLabel>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
class QLabel;
QT_END_NAMESPACE

class ImageWidget;
class OverlayLabel;

class CardBoard : public QLabel
{
    Q_OBJECT
public:
    explicit CardBoard(OverlayLabel* overlay, QWidget *parent);
    virtual ~CardBoard();

    void reset();
    void save();
    void gen(const QList<QString>& pack);
    void applyScaleFactor(float factor);
    void applyBackground(const QString& path);
    void removeBackground();

    void applyForeground(const QString& path);
    void removeForeground();

signals:
    void dirty();
    void saved(const QString&);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    float m_scaleFactor = 1.0f;
    QString m_foregroundPath;

    OverlayLabel* m_overlay = nullptr;

    void __markDirty();
};


