#include <mainwindow.hpp>
#include "imagespoolwidget.hpp"
#include "dragwidget.hpp"
#include "colors.hpp"
#include "utils.hpp"

#include <QApplication>
#include <QScrollArea>
#include <QStyle>
#include <QDesktopWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QStandardPaths>
#include <QSettings>
#include <QDebug>
#include <QFileDialog>
#include <QTimer>
#include <QCheckBox>

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    compactLayout(mainLayout);

    QWidget* wLeftPane = new QWidget;
    QWidget* wRightPane = new QWidget;

    mainLayout->addWidget(wLeftPane);
    mainLayout->addWidget(wRightPane);

    QVBoxLayout* leftPaneLayout = new QVBoxLayout(this);
    QVBoxLayout* rightPaneLayout = new QVBoxLayout(this);

    compactLayout(leftPaneLayout);
    compactLayout(rightPaneLayout);

    wLeftPane->setLayout(leftPaneLayout);
    wRightPane->setLayout(rightPaneLayout);

    QWidget* wLeftPaneControl = new QWidget;
    QWidget* wRightPaneControl = new QWidget;

    /// leftPaneControlLayout
    QVBoxLayout* leftPaneControlLayout = new QVBoxLayout(wLeftPaneControl);
    compactLayout(leftPaneControlLayout);

    QWidget* wLibrary = new QWidget;
    QHBoxLayout* libraryLayout = new QHBoxLayout;
    compactLayout(libraryLayout);
    wLibrary->setLayout(libraryLayout);

    QLabel* lLibrary = new QLabel(tr("Location to images:"));
    m_leMediaLibraryPath = new QLineEdit(wLeftPaneControl);
    m_leMediaLibraryPath->setObjectName("m_leMediaLibraryPath");
    QPushButton* bRealodLibrary = new QPushButton(tr("Reload"));
    QPushButton* bOpenMediaPath = new QPushButton(tr("Open"));

//    connect(m_mediaPathEdit, &QLineEdit::textEdited, this, [this](){
//        qInfo()<<"m_mediaPathEdit"<<m_mediaPathEdit->text();
//    });

    connect(bOpenMediaPath, &QPushButton::clicked, this, [this](){
        QString pathCandidate = QFileDialog::getExistingDirectory(this,
                                                             tr("Open Images Location"),
                                                             __tryLoadExistedPath(m_leMediaLibraryPath->objectName()));
        __processNewMediaLibraryPath(pathCandidate);
    });

    connect(bRealodLibrary, &QPushButton::clicked, this, [this](){
        QString pathCandidate = m_leMediaLibraryPath->text();
       __processNewMediaLibraryPath(pathCandidate);
    });

    libraryLayout->addWidget(lLibrary);
    libraryLayout->addWidget(m_leMediaLibraryPath);
    libraryLayout->addWidget(bOpenMediaPath);
    libraryLayout->addWidget(bRealodLibrary);

    leftPaneControlLayout->addWidget(wLibrary);
    /// leftPaneControlLayout

    /// controlRightLayout
    QVBoxLayout* controlRightLayout = new QVBoxLayout(wRightPaneControl);
    compactLayout(controlRightLayout);

    QFrame* wCardFrame = new QFrame;
    QHBoxLayout* cardFrameLayout = new QHBoxLayout;
    compactLayout(cardFrameLayout);
    wCardFrame->setLayout(cardFrameLayout);

    QCheckBox* cbUseFrame = new QCheckBox("use frame");
    cbUseFrame->setChecked(true);

    QLabel* lFrame = new QLabel(tr("Card frame:"));
    m_leCardFramePath = new QLineEdit(wRightPaneControl);
    m_leCardFramePath->setObjectName("m_leCardFramePath");
    QPushButton* bApplyFrame = new QPushButton(tr("Apply"));
    QPushButton* bOpenFrame = new QPushButton(tr("Open"));

    connect(cbUseFrame, &QCheckBox::stateChanged, [this, lFrame, bApplyFrame, bOpenFrame](bool state){
        QList<QWidget*> widgets = {lFrame, m_leCardFramePath, bApplyFrame, bOpenFrame};
        for (QWidget* widget: widgets) {
            widget->setEnabled(state);
        }
        __refreshCardFrame(m_leCardFramePath->text(), state);
    });

    connect(bOpenFrame, &QPushButton::clicked, this, [this](){
        QString pathCandidate = QFileDialog::getExistingDirectory(this,
                                                             tr("Select Card Frame Image"),
                                                             __tryLoadExistedPath(m_leCardFramePath->objectName()));
        __processNewMediaLibraryPath(pathCandidate);
    });

    cardFrameLayout->addWidget(cbUseFrame);
    cardFrameLayout->addWidget(lFrame);
    cardFrameLayout->addWidget(m_leCardFramePath);
    cardFrameLayout->addWidget(bOpenFrame);
    cardFrameLayout->addWidget(bApplyFrame);

    controlRightLayout->addWidget(wCardFrame);
    // controlRightLayout

    m_leftBoard = new ImagesPoolWidget;
    QWidget* wRightBoard = new QWidget;

    DragWidget* card = new DragWidget;
    QHBoxLayout* cardLayout = new QHBoxLayout;
    wRightBoard->setLayout(cardLayout);
    cardLayout->addWidget(card);

    card->setFixedSize(480, 640);
    card->setStyleSheet("background: yellow;");

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(m_leftBoard);

    leftPaneLayout->addWidget(wLeftPaneControl);
    leftPaneLayout->addWidget(scrollArea);

    wRightPaneControl->setStyleSheet("background: red");
    rightPaneLayout->addWidget(wRightPaneControl);
    rightPaneLayout->addWidget(wRightBoard);

    resize(QDesktopWidget().availableGeometry(this).size() * 0.8);

    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            size(),
            qApp->desktop()->availableGeometry()
        )
    );

    setWindowTitle(tr("Cards Generator"));

    leftPaneLayout->setStretch(0,0);
    leftPaneLayout->setStretch(1,1);

    rightPaneLayout->setStretch(0,0);
    rightPaneLayout->setStretch(1,1);

    mainLayout->setStretch(0,1);
    mainLayout->setStretch(1,1);

    show();
    __tryRestoreSession();
}

MainWindow::~MainWindow()
{}


void MainWindow::__processNewMediaLibraryPath(const QString& pathCandidate)
{
    __updateColorBasedOnPathExistance(m_leMediaLibraryPath, pathCandidate);
    if (QFileInfo(pathCandidate).exists()) {
        if (m_leMediaLibraryPath->text() != pathCandidate) {
            m_leMediaLibraryPath->setText(pathCandidate);
        }
        __savePath(m_leMediaLibraryPath->objectName(), pathCandidate);
        __reloadLibrary();
    }
}

void MainWindow::__processNewCardFramePath(const QString& pathCandidate)
{
    __updateColorBasedOnPathExistance(m_leCardFramePath, pathCandidate);
    if (QFileInfo(pathCandidate).exists()) {
        if (m_leCardFramePath->text() != pathCandidate) {
            m_leCardFramePath->setText(pathCandidate);
        }
        __savePath(m_leCardFramePath->objectName(), pathCandidate);
//        __reloadCardFrame();
    }
}

QString MainWindow::__tryLoadExistedPath(const QString& key) const
{
    QSettings settings;
    QString path = settings.value(key).toString();
    if (!QFileInfo(path).exists()) {
        path = "";
    }
    return path;
}

void MainWindow::__savePath(const QString& key, const QString& path) const
{
    QSettings settings;
    settings.setValue(key, path);
}

void MainWindow::__tryRestoreSession()
{
    QString path = __tryLoadExistedPath(m_leMediaLibraryPath->objectName());
    if (!QFileInfo(path).exists()) {
       QStringList pathes = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
       if (!pathes.isEmpty()) {
           path = pathes.first();
       }
    }
    __updateColorBasedOnPathExistance(m_leMediaLibraryPath, path);
    m_leMediaLibraryPath->setText(path);
    __reloadLibrary();
}

void MainWindow::__reloadLibrary()
{
    QString path = m_leMediaLibraryPath->text();
    qInfo()<<"load images from"<<path;
    QList<QString> files = __getImageFiles(path);
    qInfo() << files;
    m_leftBoard->fill(files);
}

void MainWindow::__updateColorBasedOnPathExistance(QWidget* widget, const QString& path) const
{
    if (QFileInfo(path).exists()) {
        widget->setStyleSheet(QString("background: %1;").arg(COLOR_GREEN.name()));
    } else {
        widget->setStyleSheet(QString("background: %1;").arg(COLOR_RED.name()));
    }
}

QList<QString>
MainWindow::__getImageFiles(const QString& path) const
{
    QStringList files;

    QStringList filters;
    filters << "*.png";
    filters << "*.jpg";
    filters << "*.bmp";

    QDirIterator it(path, filters, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        files << it.next();
    }
    return std::move(files);
}

void MainWindow::__refreshCardFrame(const QString&, bool state)
{
    qInfo()<<"__refreshCardFrame"<<state;
}
