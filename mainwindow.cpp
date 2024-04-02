#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , play_flag(false)
    , lock(false)
    , media_player_button(new QPushButton(this))
    , media_stop_button(new QPushButton(this))
    , media_player(new QMediaPlayer(this))
    , audio_output(new QAudioOutput(this))
    , slider_volume(new QSlider(Qt::Orientation::Horizontal, this))
    , slider_time(new QSlider(Qt::Orientation::Horizontal, this))
    , time_label(new QLabel("", this))
    , name_label(new QLabel("", this))
    , open_music_button(new QPushButton(this))
{
    this->setGeometry(100, 200, 800, 200);
    this->setFixedSize(800, 200);

    media_player_button->setGeometry(50, 50, 100, 50);
    media_player_button->setText("▶");
    media_player_button->setStyleSheet("background-color: dimGray;");

    media_stop_button->setGeometry(200, 50, 100, 50);
    media_stop_button->setStyleSheet("background-color: dimGray;");
    media_stop_button->setText("⏮");

    slider_volume->setGeometry(600, 100, 150, 50);
    slider_volume->setMaximum(100);

    slider_time->setGeometry(50, 140, 600, 50);

    time_label->setGeometry(675, 145, 100, 50);
    time_label->setStyleSheet("background-color: dimGray;");
    time_label->setAlignment(Qt::AlignCenter);
    time_label->setText("00:00");

    name_label->setGeometry(350, 50, 200, 50);
    name_label->setStyleSheet("background-color: dimGray;");

    //Open Music window
    open_music_button->setGeometry(600, 50, 150, 50);
    open_music_button->setStyleSheet("background-color: dimGray;");
    open_music_button->setText("Open Songs List");

    media_player->setAudioOutput(audio_output);

    path = "/Users/vahe/Downloads/price-of-freedom.mp3";

    media_player->setSource(QUrl(path));

    connect(media_player, &QMediaPlayer::metaDataChanged, this, &MainWindow::datachanged);

    QStringList list = path.split("/");
    song_name = list[list.size() - 1];

    name_label->setAlignment(Qt::AlignCenter);
    name_label->setText(song_name);

    connect(media_player_button, &QPushButton::clicked, [this](){
        if (!play_flag)
        {
            media_player->play();
            play_flag = true;
            media_player_button->setText("▐▐");
        }
        else
        {
            media_player->pause();
            play_flag = false;
            media_player_button->setText("▶");
        }
    });

    connect(media_stop_button, &QPushButton::clicked, [this](){
        media_player->stop();
        media_player_button->setText("▶");
        datachanged();
    });

    connect(slider_time, &QSlider::valueChanged, [this]() {
        if (!lock)
        {
            lock = true;
            media_player->setPosition(slider_time->value());
            lock = false;
        }
    });

    connect(slider_volume, &QSlider::valueChanged, [this](int value) {
        qreal volume = qreal(value) / qreal(slider_volume->maximum());
        audio_output->setVolume(volume);
    });

    connect(media_player, &QMediaPlayer::positionChanged, [this](qint64 position){
        if (!lock)
        {
            lock = true;
            slider_time->setValue(position);
            position = media_player->duration() - position;

            int minutes = position / 60000;
            int seconds = (position % 60000) / 1000;
            QString timeString = QString("%1:%2").arg(minutes, 2, 10, QLatin1Char('0')).arg(seconds, 2, 10, QLatin1Char('0'));
            time_label->setText(timeString);
            lock = false;
        }
    });
}

MainWindow::~MainWindow()
{
    // Destructor implementation
}

void MainWindow::datachanged()
{
    const qint64 duration = media_player->duration();

    int minutes = duration / 60000;
    int seconds = (duration % 60000) / 1000;
    QString timeString = QString("%1:%2").arg(minutes, 2, 10, QLatin1Char('0')).arg(seconds, 2, 10, QLatin1Char('0'));
    time_label->setText(timeString);

    slider_time->setMaximum(duration);
}
