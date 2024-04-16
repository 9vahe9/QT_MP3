#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSlider>
#include <QLabel>
#include <QString>
#include <QResizeEvent>
#include <QFileDialog>
#include <QStringList>
#include <QVideoWidget>
#include <QPixmap>
#include "playlistupgrade.h"

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

private slots:
    void datachanged();
    void resizeEvent(QResizeEvent* e) override;

    void handle_next();
    void handle_prev();
    void handleCurrentChanged(qint64 curr);

private:
    // Ui::MainWindow *ui;
    bool play_flag;

    bool lock;

    QPushButton* media_player_button;
    QPushButton* media_stop_button;
    QPushButton* media_forward_button;
    QPushButton* media_back_button;
    QPushButton* next_song_button;
    QPushButton* prev_song_button;
    QPushButton* add_folder_songs;
    QMediaPlayer* media_player;
    QAudioOutput* audio_output;

    QSlider* slider_time;
    QSlider* slider_volume;

    QLabel* time_label;
    QLabel* name_label;

    QLabel* image_lbl;

    PlayListUpgrade* playlist;

    QVideoWidget* video_widget;

private:
    double p(int val, double p);
};
#endif // MAINWINDOW_H
