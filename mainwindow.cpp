#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , play_flag(false)
    , lock(false)
    , media_player_button(new QPushButton(this))
    , media_stop_button(new QPushButton(this))
    , media_forward_button(new QPushButton(this))
    , media_back_button(new QPushButton(this))
    , media_player(new QMediaPlayer(this))
    , audio_output(new QAudioOutput(this))
    , slider_volume(new QSlider(Qt::Orientation::Horizontal, this))
    , slider_time(new QSlider(Qt::Orientation::Horizontal, this))
    , time_label(new QLabel("", this))
    , name_label(new QLabel("", this))
    , playlist(new PlayList(this))
{
    this->setGeometry(100, 200, 800, 600);
    this->setFixedSize(800, 600);

    media_player_button->setGeometry(50, 50, 50, 50);
    media_player_button->setText("▶");
    media_player_button->setStyleSheet("background-color: dimGray;");

    media_stop_button->setGeometry(125, 50, 50, 50);
    media_stop_button->setStyleSheet("background-color: dimGray;");
    media_stop_button->setText("⏮");

    media_back_button->setGeometry(200, 50, 50, 50);
    media_back_button->setStyleSheet("background-color: dimGray;");
    media_back_button->setText("-10");

    media_forward_button->setGeometry(275, 50, 50, 50);
    media_forward_button->setStyleSheet("background-color: dimGray;");
    media_forward_button->setText("+10");

    slider_volume->setGeometry(600, 100, 150, 50);
    slider_volume->setMaximum(100);

    slider_time->setGeometry(50, 140, 600, 50);

    time_label->setGeometry(660, 145, 90, 50);
    time_label->setStyleSheet("background-color: dimGray;");
    time_label->setAlignment(Qt::AlignCenter);
    time_label->setText("00:00");

    name_label->setGeometry(350, 50, 400, 50);
    name_label->setStyleSheet("background-color: dimGray;");

    //Open Music window

    media_player->setAudioOutput(audio_output);

    connect(media_player, &QMediaPlayer::metaDataChanged, this, &MainWindow::datachanged);

    name_label->setAlignment(Qt::AlignCenter);

    playlist->setGeometry(50, 225, 700, 350);

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

    connect(media_back_button, &QPushButton::clicked, [this](){
        media_player->setPosition(media_player->position() - 10000);
    });

    connect(media_forward_button, &QPushButton::clicked, [this](){
        media_player->setPosition(media_player->position() + 10000);
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

    connect(playlist, &PlayList::songIsReady, this, &MainWindow::handleSongReady);
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

double MainWindow::percent(int val, double p)
{
    return val * 100 / p;
}

void MainWindow::resizeEvent(QResizeEvent* e)
{
}

void MainWindow::handle_next()
{
    playlist->setNext();
}

void MainWindow::handle_prev()
{
    playlist->setPrev();
}

void MainWindow::handleSongReady()
{
    media_player->setSource(QUrl(playlist->getSong()));
    QStringList list = playlist->getSong().split("/");
    name_label->setText(list[list.size() - 1]);
}
