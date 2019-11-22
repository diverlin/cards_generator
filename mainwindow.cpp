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

    QVBoxLayout* leftPaneControlLayout = new QVBoxLayout(wLeftPaneControl);
    compactLayout(leftPaneControlLayout);

    QWidget* wLibrary = new QWidget;
    QHBoxLayout* libraryLayout = new QHBoxLayout;
    compactLayout(libraryLayout);
    wLibrary->setLayout(libraryLayout);

    QLabel* lLibrary = new QLabel("Location to images:");
    m_mediaPathEdit = new QLineEdit(wLeftPaneControl);
    QPushButton* bRefresh = new QPushButton("reload");
    QPushButton* bOpen = new QPushButton("open");

//    connect(m_mediaPathEdit, &QLineEdit::textEdited, this, [this](){
//        qInfo()<<"m_mediaPathEdit"<<m_mediaPathEdit->text();
//    });

    connect(bOpen, &QPushButton::clicked, this, [this](){
        QString pathCandidate = QFileDialog::getExistingDirectory(this,
                                                             tr("Open Images Location"),
                                                             __loadLibraryPath());
        __processNewPath(pathCandidate);
    });

    connect(bRefresh, &QPushButton::clicked, this, [this](){
        QString pathCandidate = m_mediaPathEdit->text();
       __processNewPath(pathCandidate);
    });

    libraryLayout->addWidget(lLibrary);
    libraryLayout->addWidget(m_mediaPathEdit);
    libraryLayout->addWidget(bOpen);
    libraryLayout->addWidget(bRefresh);

    leftPaneControlLayout->addWidget(wLibrary);

    // controlRightLayout
    QVBoxLayout* controlRightLayout = new QVBoxLayout(wRightPaneControl);
    compactLayout(controlRightLayout);

    m_leftBoard = new ImagesPoolWidget;
    m_rightBoard = new DragWidget;

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(m_leftBoard);

    leftPaneLayout->addWidget(wLeftPaneControl);
    leftPaneLayout->addWidget(scrollArea);

    rightPaneLayout->addWidget(wRightPaneControl);
    rightPaneLayout->addWidget(m_rightBoard);

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


void MainWindow::__processNewPath(const QString& pathCandidate)
{
    __updatePathColor(pathCandidate);
    if (QFileInfo(pathCandidate).exists()) {
        if (m_mediaPathEdit->text() != pathCandidate) {
            m_mediaPathEdit->setText(pathCandidate);
        }
        __saveLibraryPath(pathCandidate);
        __reloadLibrary();
    }
}

QString MainWindow::__loadLibraryPath() const
{
    QSettings settings;
    QString path = settings.value("libraryLocation").toString();
    if (!QFileInfo(path).exists()) {
        path = "";
    }
    return path;
}

void MainWindow::__saveLibraryPath(const QString& path) const
{
    QSettings settings;
    settings.setValue("libraryLocation", path);
}

void MainWindow::__tryRestoreSession()
{
    QString path = __loadLibraryPath();
    if (!QFileInfo(path).exists()) {
       QStringList pathes = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
       if (!pathes.isEmpty()) {
           path = pathes.first();
       }
    }
    __updatePathColor(path);
    m_mediaPathEdit->setText(path);
    __reloadLibrary();
}

void MainWindow::__reloadLibrary()
{
    QString path = m_mediaPathEdit->text();
    qInfo()<<"load images from"<<path;
    QList<QString> files = __getImageFiles(path);
    qInfo() << files;
    m_leftBoard->fill(files);
}

void MainWindow::__updatePathColor(const QString& path) const
{
    if (QFileInfo(path).exists()) {
        m_mediaPathEdit->setStyleSheet(QString("background: %1;").arg(COLOR_GREEN.name()));
    } else {
        m_mediaPathEdit->setStyleSheet(QString("background: %1;").arg(COLOR_RED.name()));
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
