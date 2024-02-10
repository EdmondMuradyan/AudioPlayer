
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConnected();
    void readMusicData();
    void on_playButton_clicked();
    void on_pauseButton_clicked();
    void on_stopButton_clicked();
    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QMediaPlayer *M_Player;
    QAudioOutput *AudioOutput;
    QByteArray buffer;
};

#endif // MAINWINDOW_H


