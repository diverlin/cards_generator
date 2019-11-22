#pragma once

#include <QLabel>

class ImageWidget : public QLabel {
    Q_OBJECT
public:
    ImageWidget(const QString& imagePath, QWidget* parent = nullptr);
    virtual ~ImageWidget();

    void scale(float factor);

private:
    QSize m_size;
    QString m_imagePath;
};

