#pragma once

#include <QWidget>

class QLineEdit;
class ImagesPoolWidget;
class CardBoard;
class QCheckBox;
class QLabel;

class MainWindow : public QWidget {
  Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    QLineEdit* m_leMediaLibraryPath = nullptr;
    QLineEdit* m_leCardBackgroundPath = nullptr;
    QLineEdit* m_leCardForegroundPath = nullptr;
    CardBoard* m_cardBoard = nullptr;
    QCheckBox* m_cbUseCardBackground = nullptr;
    QCheckBox* m_cbUseCardForeground = nullptr;

    ImagesPoolWidget* m_leftBoard = nullptr;
    QList<QString> m_imageFiles;
    QLabel* m_lStatusSave = nullptr;

    void __tryRestoreSession();
    QString __tryLoadExistedPath(const QString&) const;
    void __savePath(const QString&, const QString&) const;
    void __reloadMediaLibrary();
    void __updateColorBasedOnPathExistance(QWidget*, const QString&) const;
    void __collectImageFiles(const QString& path);
    void __processNewMediaLibraryPath(const QString& pathCandidate);
    void __processNewCardBackgroundPath(const QString& pathCandidate);
    void __processNewCardForegroundPath(const QString& pathCandidate);
    void __reloadCardBackground(QString, bool);
    void __reloadCardForeground(QString, bool);
    QList<QString> __getUniqueFileNames(int num=5);
};

