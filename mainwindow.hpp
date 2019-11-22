#pragma once

#include <QWidget>

class QLineEdit;
class ImagesPoolWidget;
class DragWidget;

class MainWindow : public QWidget {
  Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    QLineEdit* m_mediaPathEdit = nullptr;
    ImagesPoolWidget* m_leftBoard = nullptr;
    DragWidget* m_rightBoard = nullptr;

    void __tryRestoreSession();
    QString __loadLibraryPath() const;
    void __saveLibraryPath(const QString&) const;
    void __reloadLibrary();
    void __updatePathColor(const QString&) const;
    QList<QString> __getImageFiles(const QString& path) const;
    void __processNewPath(const QString& pathCandidate);
};

