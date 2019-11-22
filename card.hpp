#pragma once

#include <QLabel>

class Card : public QLabel {
    Q_OBJECT
public:
    Card(const QString& imagePath, QWidget* parent = nullptr);
    virtual ~Card();

    void scale(float factor);
    void applyMask();

private:
    QSize m_size;
    QString m_imagePath;
};

