#pragma once

#include <QWidget>

class QLineEdit;

class MainWindow : public QWidget {
  Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    QLineEdit* m_mediaPathEdit = nullptr;

    void __tryRestoreSession();
    QString __loadLibraryPath() const;
    void __saveLibraryPath(const QString&) const;
    void __reloadLibrary();
    void __updatePathColor(QString&) const;
};

