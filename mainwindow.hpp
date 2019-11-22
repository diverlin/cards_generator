#pragma once

#include <QWidget>

class QLineEdit;
class DragGridWidget;
class DragWidget;

class MainWindow : public QWidget {
  Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    QLineEdit* m_mediaPathEdit = nullptr;
    DragGridWidget* m_leftBoard = nullptr;
    DragWidget* m_rightBoard = nullptr;

    void __tryRestoreSession();
    QString __loadLibraryPath() const;
    void __saveLibraryPath(const QString&) const;
    void __reloadLibrary();
    void __updatePathColor(QString&) const;
    QList<QString> __getImageFiles(const QString& path) const;
};

