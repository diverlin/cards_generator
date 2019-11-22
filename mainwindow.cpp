#include <mainwindow.hpp>
#include "imagespoolwidget.hpp"
#include "cardboard.hpp"
#include "colors.hpp"
#include "utils.hpp"
#include "overlaylabel.hpp"

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
#include <QSlider>
#include <QGroupBox>

#include <random>
#include "QDateTime"

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    // Create seed for the random
    // That is needed only once on application startup
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QWidget* wBody = new QWidget;

    mainLayout->addWidget(wBody);

    QHBoxLayout* mainBodyLayout = new QHBoxLayout(wBody);
    utils::compactLayout(mainLayout);

    QWidget* wLeftPane = new QWidget;
    QWidget* wRightPane = new QWidget;

    mainBodyLayout->addWidget(wLeftPane);
    mainBodyLayout->addWidget(wRightPane);

    QVBoxLayout* leftPaneLayout = new QVBoxLayout(this);
    QVBoxLayout* rightPaneLayout = new QVBoxLayout(this);

    utils::compactLayout(leftPaneLayout);
    utils::compactLayout(rightPaneLayout);

    wLeftPane->setLayout(leftPaneLayout);
    wRightPane->setLayout(rightPaneLayout);

    QWidget* wLeftPaneControl = new QWidget;
    QWidget* wRightPaneControl = new QWidget;

    /// leftPaneControlLayout
    QVBoxLayout* leftPaneControlLayout = new QVBoxLayout(wLeftPaneControl);
    utils::compactLayout(leftPaneControlLayout);

    QGroupBox* gbLibrary = new QGroupBox;
    gbLibrary->setTitle(tr("Media Library"));
    QHBoxLayout* libraryLayout = new QHBoxLayout;
    utils::compactLayout(libraryLayout);
    gbLibrary->setLayout(libraryLayout);

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

    leftPaneControlLayout->addWidget(gbLibrary);
    /// leftPaneControlLayout

    /// controlRightLayout
    QVBoxLayout* controlRightLayout = new QVBoxLayout(wRightPaneControl);
    utils::compactLayout(controlRightLayout);

    /// card background
    QGroupBox* gbControlCardBackground = new QGroupBox;
    gbControlCardBackground->setTitle(tr("Card Background"));
    QHBoxLayout* controlCardBackgroundLayout = new QHBoxLayout;
    utils::compactLayout(controlCardBackgroundLayout);
    gbControlCardBackground->setLayout(controlCardBackgroundLayout);

    m_cbUseCardBackground = new QCheckBox("Enabled");
    m_cbUseCardBackground->setChecked(true);

    QLabel* lBackground = new QLabel(tr("Path:"));
    m_leCardBackgroundPath = new QLineEdit(wRightPaneControl);
    m_leCardBackgroundPath->setObjectName("m_leCardBackgroundPath");
    QPushButton* bApplyBackground = new QPushButton(tr("Apply"));
    QPushButton* bOpenBackground = new QPushButton(tr("Open"));
    //

    QSlider* scaleSlider = new QSlider(Qt::Orientation::Horizontal, wRightPaneControl);
    scaleSlider->setMinimum(20);
    scaleSlider->setMaximum(500);
    scaleSlider->setValue(200);
    connect(scaleSlider, &QSlider::valueChanged, [this](int value){
        m_cardBoard->applyScaleFactor(value/100.0f);
    });

    connect(m_cbUseCardBackground, &QCheckBox::stateChanged, [this, lBackground, bApplyBackground, bOpenBackground](bool state){
        QList<QWidget*> widgets = {lBackground, m_leCardBackgroundPath, bApplyBackground, bOpenBackground};
        for (QWidget* widget: widgets) {
            widget->setEnabled(state);
        }
        __reloadCardBackground(m_leCardBackgroundPath->text(), state);
    });

    connect(bOpenBackground, &QPushButton::clicked, this, [this](){
        QString failBackPath = __tryLoadExistedPath(m_leCardBackgroundPath->objectName());
        failBackPath = failBackPath.replace(QFileInfo(failBackPath).baseName(), "");

        QString pathCandidate = QFileDialog::getOpenFileName(this, tr("Open Card Background Image"),
                                                             failBackPath,
                                                             tr("Images (*.png *.jpg *.jpeg *.bmp)"));

        __processNewCardBackgroundPath(pathCandidate);
    });

    controlCardBackgroundLayout->addWidget(m_cbUseCardBackground);
    controlCardBackgroundLayout->addWidget(lBackground);
    controlCardBackgroundLayout->addWidget(m_leCardBackgroundPath);
    controlCardBackgroundLayout->addWidget(bOpenBackground);
    controlCardBackgroundLayout->addWidget(bApplyBackground);
    ///


    /// card foreground
    QGroupBox* gbControlCardForeground = new QGroupBox;
    gbControlCardForeground->setTitle("Card Foreground");
    QHBoxLayout* controlCardForegroundLayout = new QHBoxLayout;
    utils::compactLayout(controlCardForegroundLayout);
    gbControlCardForeground->setLayout(controlCardForegroundLayout);

    m_cbUseCardForeground = new QCheckBox(tr("Enabled"));
    m_cbUseCardForeground->setChecked(true);

    QLabel* lForeground = new QLabel(tr("Path:"));
    m_leCardForegroundPath = new QLineEdit(wRightPaneControl);
    m_leCardForegroundPath->setObjectName("m_leCardForegroundPath");
    QPushButton* bApplyForeground = new QPushButton(tr("Apply"));
    QPushButton* bOpenForeground = new QPushButton(tr("Open"));
    //

    connect(m_cbUseCardForeground, &QCheckBox::stateChanged, [this, lForeground, bApplyForeground, bOpenForeground](bool state){
        QList<QWidget*> widgets = {lForeground, m_leCardForegroundPath, bApplyForeground, bOpenForeground};
        for (QWidget* widget: widgets) {
            widget->setEnabled(state);
        }
        __reloadCardForeground(m_leCardForegroundPath->text(), state);
    });

    connect(bOpenForeground, &QPushButton::clicked, this, [this](){
        QString failBackPath = __tryLoadExistedPath(m_leCardForegroundPath->objectName());
        failBackPath = failBackPath.replace(QFileInfo(failBackPath).baseName(), "");

        QString pathCandidate = QFileDialog::getOpenFileName(this, tr("Open Card Foreground Image"),
                                                             failBackPath,
                                                             tr("Images (*.png *.jpg *.jpeg *.bmp)"));
        __processNewCardForegroundPath(pathCandidate);
    });

    controlCardForegroundLayout->addWidget(m_cbUseCardForeground);
    controlCardForegroundLayout->addWidget(lForeground);
    controlCardForegroundLayout->addWidget(m_leCardForegroundPath);
    controlCardForegroundLayout->addWidget(bOpenForeground);
    controlCardForegroundLayout->addWidget(bApplyForeground);
    ///


    ///
    QWidget* wButtons = new QWidget;
    QHBoxLayout* buttonsLayout = new QHBoxLayout(wButtons);

    QPushButton* bDice = new QPushButton(tr("Dice"), this);
    connect(bDice, &QPushButton::clicked, [this](){
        QList<QString> fileNames = __getUniqueFileNames(5);
        m_cardBoard->gen(fileNames);
    });

    QPushButton* bClear = new QPushButton(tr("Clear"), this);
    connect(bClear, &QPushButton::clicked, [this](){
        m_cardBoard->reset();
    });

    QPushButton* bSave = new QPushButton(tr("Save"), this);
    connect(bSave, &QPushButton::clicked, [this](){
        m_cardBoard->save();
    });

    buttonsLayout->addWidget(bDice);
    buttonsLayout->addWidget(bClear);
    buttonsLayout->addWidget(bSave);
    ///

    controlRightLayout->addWidget(gbControlCardBackground);
    controlRightLayout->addWidget(gbControlCardForeground);
    controlRightLayout->addWidget(scaleSlider);
    controlRightLayout->addWidget(wButtons);
    // controlRightLayout

    m_leftBoard = new ImagesPoolWidget;
    QWidget* wRightBoard = new QWidget;
    wRightBoard->setObjectName("wRightBoard");

    QWidget* cardBoardWrapper = new QWidget;
    QVBoxLayout* cardBoardWrapperLayout = new QVBoxLayout(cardBoardWrapper);
    utils::superCompactLayout(cardBoardWrapperLayout);

    OverlayLabel* cardBoardOverlay = new OverlayLabel(cardBoardWrapper);

    m_cardBoard = new CardBoard(cardBoardOverlay, wRightBoard);
    QHBoxLayout* cardLayout = new QHBoxLayout;
    wRightBoard->setLayout(cardLayout);

    cardBoardWrapperLayout->addWidget(m_cardBoard);
    cardLayout->addSpacerItem( new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed) );
    cardLayout->addWidget(cardBoardWrapper);
    cardLayout->addSpacerItem( new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed) );

    m_lStatusSave = new QLabel;
    m_lStatusSave->setObjectName("m_lStatusSave");

    connect(m_cardBoard, &CardBoard::dirty, [this, wRightBoard](){
       m_lStatusSave->setText("card is not saved yet");
       utils::colorify(m_lStatusSave, COLOR_RED);
       utils::colorify(wRightBoard, COLOR_RED);
    });
    connect(m_cardBoard, &CardBoard::saved, [this, wRightBoard](const QString& path){
       m_lStatusSave->setText(QString("card saved to %1").arg(path));
       utils::colorify(m_lStatusSave, COLOR_GREEN);
       utils::colorify(wRightBoard, COLOR_GREEN);
    });

    m_cardBoard->setFixedSize(480, 640);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(m_leftBoard);

    leftPaneLayout->addWidget(wLeftPaneControl);
    leftPaneLayout->addWidget(scrollArea);

    rightPaneLayout->addWidget(wRightPaneControl);
    rightPaneLayout->addWidget(wRightBoard);

    mainLayout->addWidget(m_lStatusSave);

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

    mainBodyLayout->setStretch(0,1);
    mainBodyLayout->setStretch(1,1);

    m_cardBoard->applyScaleFactor(scaleSlider->value()/100.0f);
    cardBoardOverlay->raise();
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
        __reloadMediaLibrary();
    }
}

void MainWindow::__processNewCardBackgroundPath(const QString& pathCandidate)
{
    __updateColorBasedOnPathExistance(m_leCardBackgroundPath, pathCandidate);
    if (QFileInfo(pathCandidate).exists()) {
        if (m_leCardBackgroundPath->text() != pathCandidate) {
            m_leCardBackgroundPath->setText(pathCandidate);
        }
        __savePath(m_leCardBackgroundPath->objectName(), pathCandidate);
        __reloadCardBackground(pathCandidate, m_cbUseCardBackground->isChecked());
    }
}

void MainWindow::__processNewCardForegroundPath(const QString& pathCandidate)
{
    __updateColorBasedOnPathExistance(m_leCardForegroundPath, pathCandidate);
    if (QFileInfo(pathCandidate).exists()) {
        if (m_leCardForegroundPath->text() != pathCandidate) {
            m_leCardForegroundPath->setText(pathCandidate);
        }
        __savePath(m_leCardForegroundPath->objectName(), pathCandidate);
        __reloadCardForeground(pathCandidate, m_cbUseCardForeground->isChecked());
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
    __reloadMediaLibrary();

    __reloadCardBackground(m_leCardBackgroundPath->text(), m_cbUseCardBackground->isChecked());
    __reloadCardForeground(m_leCardForegroundPath->text(), m_cbUseCardForeground->isChecked());
}

void MainWindow::__reloadMediaLibrary()
{
    QString path = m_leMediaLibraryPath->text();
    qInfo()<<"load images from"<<path;
    __collectImageFiles(path);
    qInfo() << m_imageFiles;
    m_leftBoard->fill(m_imageFiles);
}

void MainWindow::__updateColorBasedOnPathExistance(QWidget* widget, const QString& path) const
{
    if (QFileInfo(path).exists()) {
        widget->setStyleSheet(QString("background: %1;").arg(COLOR_GREEN.name()));
    } else {
        widget->setStyleSheet(QString("background: %1;").arg(COLOR_RED.name()));
    }
}

void MainWindow::__collectImageFiles(const QString& path)
{
    m_imageFiles.clear();

    QStringList filters;
    filters << "*.png";
    filters << "*.jpg";
    filters << "*.jpeg";
    filters << "*.bmp";

    QDirIterator it(path, filters, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        m_imageFiles << it.next();
    }
}

void MainWindow::__reloadCardBackground(QString path, bool state)
{
    qInfo()<<"__reloadCardBackground"<<path<<state;
    if (state) {
        if (!QFileInfo(path).exists()) {
            path = ":/images/bg.jpeg";
        }
        m_cardBoard->applyBackground(path);
    } else {
        m_cardBoard->removeBackground();
    }
}

void MainWindow::__reloadCardForeground(QString path, bool state)
{
    qInfo()<<"__reloadCardForeground"<<path<<state;
    if (state) {
        if (!QFileInfo(path).exists()) {
            path = ":/images/fg.png";
        }
        m_cardBoard->applyForeground(path);
    } else {
        m_cardBoard->removeForeground();
    }
}

int randInt(int low, int high)
{
// Random number between low and high
return qrand() % ((high + 1) - low) + low;
}

QList<QString>
MainWindow::__getUniqueFileNames(int num)
{
    QList<QString> result;
    num = std::min(num, m_imageFiles.size());
    int counter = 0;
    for (int i=0; i<num; ++i) {
        int randIndex = randInt(0, m_imageFiles.size()-1);
        qInfo()<<randIndex;
        QString candidate = m_imageFiles.at(randIndex);
        if (result.contains(candidate)) {
            --i;
        } else {
            result << candidate;
        }
        counter++;
        if (counter >= 100) {
            qCritical() << "overflow random attempt, probably it's due small images set num="<<m_imageFiles.size();
        }
    }
    return result;
}
