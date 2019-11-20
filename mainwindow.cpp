/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <mainwindow.hpp>
#include "dragwidget.h"

#include <QApplication>
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

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QWidget* wControl = new QWidget;

    QWidget* wBoard = new QWidget;

    QVBoxLayout* controlLayout = new QVBoxLayout(wControl);

    QWidget* wLibrary = new QWidget;
    QHBoxLayout* libraryLayout = new QHBoxLayout;
    wLibrary->setLayout(libraryLayout);

    QLabel* lLibrary = new QLabel("Location to images:");
    m_mediaPathEdit = new QLineEdit(wControl);
    QPushButton* bRefresh = new QPushButton("reload");

    //    connect(m_mediaPathEdit, &QLineEdit::textEdited, this, [this](){
    //        qInfo()<<"m_mediaPathEdit"<<m_mediaPathEdit->text();
    //    });

    connect(bRefresh, &QPushButton::clicked, this, [this](){
        QString pathCandidate = m_mediaPathEdit->text();
        __updatePathColor(pathCandidate);
        if (QFileInfo(pathCandidate).exists()) {
            __saveLibraryPath(pathCandidate);
            __reloadLibrary();
        }
    });

    libraryLayout->addWidget(lLibrary);
    libraryLayout->addWidget(m_mediaPathEdit);
    libraryLayout->addWidget(bRefresh);

    controlLayout->addWidget(wLibrary);

    QHBoxLayout* boardLayout = new QHBoxLayout(wBoard);

    m_leftBoard = new DragWidget;
    m_rightBoard = new DragWidget;

    boardLayout->addWidget(m_leftBoard);
    boardLayout->addWidget(m_rightBoard);

    mainLayout->addWidget(wControl);
    mainLayout->addWidget(wBoard);

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

    mainLayout->setStretch(0,0);
    mainLayout->setStretch(1,1);

    __tryRestoreSession();
}

MainWindow::~MainWindow()
{}

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
}

void MainWindow::__updatePathColor(QString& path) const
{
    if (QFileInfo(path).exists()) {
        m_mediaPathEdit->setStyleSheet("background: #41c3aa;");
    } else {
        m_mediaPathEdit->setStyleSheet("background: #f37366;");
    }
}

QList<QString>
MainWindow::__getImageFiles(const QString& path) const
{
    QStringList files;

    QStringList filters;
    filters << "*.png";
    filters << "*.jpg";

    QDirIterator it(path, filters, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        files << it.next();
    }
    return std::move(files);
}
