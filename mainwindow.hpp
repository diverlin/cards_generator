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
    QLineEdit* m_leMediaLibraryPath = nullptr;
    QLineEdit* m_leCardFramePath = nullptr;

    ImagesPoolWidget* m_leftBoard = nullptr;

    void __tryRestoreSession();
    QString __tryLoadExistedPath(const QString&) const;
    void __savePath(const QString&, const QString&) const;
    void __reloadLibrary();
    void __updateColorBasedOnPathExistance(QWidget*, const QString&) const;
    QList<QString> __getImageFiles(const QString& path) const;
    void __processNewMediaLibraryPath(const QString& pathCandidate);
    void __processNewCardFramePath(const QString& pathCandidate);
    void __refreshCardFrame(const QString&, bool);
};

