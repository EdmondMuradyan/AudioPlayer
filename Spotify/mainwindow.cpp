#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    M_Player = new QMediaPlayer(this);
    AudioOutput = new QAudioOutput(this);
    M_Player->setAudioOutput(AudioOutput);
    AudioOutput->setVolume(50);


    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readMusicData);


    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::on_playButton_clicked);
    connect(ui->pauseButton, &QPushButton::clicked, this, &MainWindow::on_pauseButton_clicked);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::on_stopButton_clicked);


    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::on_horizontalSlider_valueChanged);


    socket->connectToHost("localhost", 12345);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete socket;
    delete AudioOutput;
}

void MainWindow::onConnected()
{
    qDebug() << "Connected to server";
}

void MainWindow::readMusicData()
{
    QByteArray musicData = socket->readAll();
    buffer.append(musicData);
    if (buffer.isEmpty()) {
        qDebug() << "No music data received";
        return;
    }

    QString fileName = QCoreApplication::applicationDirPath() + "/received_music.mp3";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file for writing";
        return;
    }
    file.write(buffer);
    file.close();

    M_Player->setSource(QUrl::fromLocalFile(fileName));
}

void MainWindow::on_playButton_clicked()
{

    M_Player->play();

}

void MainWindow::on_pauseButton_clicked()
{
    M_Player->pause();
}

void MainWindow::on_stopButton_clicked()
{
    M_Player->stop();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    AudioOutput->setVolume(value);
}
