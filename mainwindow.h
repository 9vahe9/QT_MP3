#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSlider>
#include <QLabel>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // Ui::MainWindow *ui;
    bool play_flag;

    QPushButton* media_player_button;
    QPushButton* media_stop_button;

    QString path;
    QMediaPlayer* media_player;
    QAudioOutput* audio_output;

    QSlider* slider1;
    QSlider* slider2;

    QLabel* time_label;

    QString song_name;
    QLabel* name_label;
};
#endif // MAINWINDOW_H