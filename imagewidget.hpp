#pragma once

#include <QLabel>

class ImageWidget : public QLabel {
    Q_OBJECT
public:
    ImageWidget(const QString& imagePath, float scale = 1.0f, QWidget* parent = nullptr);
    virtual ~ImageWidget();

    void applyScale(float factor);

private:
    QSize m_size;
    QString m_imagePath;
};

