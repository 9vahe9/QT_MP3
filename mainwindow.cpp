#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , play_flag(false)
    , lock(false)
    , media_player_button(new QPushButton(this))
    , media_stop_button(new QPushButton(this))
    , media_forward_button(new QPushButton(this))
    , media_back_button(new QPushButton(this))
    , next_song_button(new QPushButton(this))
    , prev_song_button(new QPushButton(this))
    , add_folder_songs(new QPushButton(this))
    , media_player(new QMediaPlayer(this))
    , audio_output(new QAudioOutput(this))
    , slider_volume(new QSlider(Qt::Orientation::Horizontal, this))
    , slider_time(new QSlider(Qt::Orientation::Horizontal, this))
    , time_label(new QLabel("", this))
    , name_label(new QLabel("", this))
    , image_lbl(new QLabel(this))
    , playlist(new PlayListUpgrade(this))
    , video_widget(new QVideoWidget(this))
{
    this->setGeometry(100, 200, 800, 600);
    this->setMinimumWidth(600);
    this->setMinimumHeight(450);

    // media_player_button->setGeometry(50, 50, 50, 50);
    media_player_button->setText("▶");
    media_player_button->setStyleSheet("background-color: dimGray;");

    // media_stop_button->setGeometry(125, 50, 50, 50);
    media_stop_button->setStyleSheet("background-color: dimGray;");
    media_stop_button->setText("⏮");

    // media_back_button->setGeometry(200, 50, 50, 50);
    media_back_button->setStyleSheet("background-color: dimGray;");
    media_back_button->setText("-10");

    // media_forward_button->setGeometry(275, 50, 50, 50);
    media_forward_button->setStyleSheet("background-color: dimGray;");
    media_forward_button->setText("+10");

    // prev_song_button->setGeometry(350, 50, 50, 50);
    prev_song_button->setStyleSheet("background-color: dimGray;");
    prev_song_button->setText("prev");

    // add_folder_songs->setGeometry(500, 50, 50, 50);
    add_folder_songs->setStyleSheet("background-color: dimGray;");
    add_folder_songs->setText("Dir");

    connect(add_folder_songs, &QPushButton::clicked, [this]()
    {
        QStringList songs = QFileDialog::getOpenFileNames(nullptr, "Select MP3 and MP4 Files", QDir::homePath(), "MP3 and MP4 files (*.mp3 *.mp4)");
        playlist->append_songs(songs);
    });

    connect(prev_song_button, &QPushButton::clicked, [this](){
        handle_prev();
    });

    // next_song_button->setGeometry(425, 50, 50, 50);
    next_song_button->setStyleSheet("background-color: dimGray;");
    next_song_button->setText("next");

    connect(next_song_button, &QPushButton::clicked, [this](){
        handle_next();
    });

    //slider_volume->setGeometry(600, 100, 150, 50);
    slider_volume->setMaximum(100);

    slider_volume->setValue(50);

    //slider_time->setGeometry(50, 140, 600, 50);

    //time_label->setGeometry(675, 140, 75, 50);
    time_label->setStyleSheet("background-color: dimGray;");
    time_label->setAlignment(Qt::AlignCenter);
    time_label->setText("00:00");

    //name_label->setGeometry(575, 50, 175, 50);
    name_label->setStyleSheet("background-color: dimGray;");

    //Open Music window

    media_player->setAudioOutput(audio_output);

    connect(media_player, &QMediaPlayer::metaDataChanged, this, &MainWindow::datachanged);

    name_label->setAlignment(Qt::AlignCenter);

    //playlist->setGeometry(50, 225, 340, 350);

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

    connect(playlist, &QListWidget::currentRowChanged, this, &MainWindow::handleCurrentChanged);

    //video_widget->setGeometry(410, 225, 340, 350);
    //image_lbl->setGeometry(410, 225, 340, 350);
    QPixmap pixmap("/Users/vahe/Qt_Projects/MP3_player/picture/gle1.png");
    image_lbl->setPixmap(pixmap);
    image_lbl->setVisible(true);

    media_player->setVideoOutput(video_widget);
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

double MainWindow::p(int val, double p)
{
    return val * p / 100;
}

void MainWindow::resizeEvent(QResizeEvent* e)
{
    qint64 w = this->width();
    qint64 h = this->height();

    //this->setGeometry(100, 200, 800, 600);
    media_player_button->setGeometry(p(w, 6.25), p(h, 25 / 3), p(w, 6.25), p(h, 25 / 3));
    media_stop_button->setGeometry(p(w, 15.625), p(h, 25 / 3), p(w, 6.25), p(h, 25 / 3));
    media_back_button->setGeometry(p(w, 25), p(h, 25 / 3), p(w, 6.25), p(h, 25 / 3));
    media_forward_button->setGeometry(p(w, 34.375), p(h, 25 / 3), p(w, 6.25), p(h, 25 / 3));
    prev_song_button->setGeometry(p(w, 43.75), p(h, 25 / 3), p(w, 6.25), p(h, 25 / 3));
    next_song_button->setGeometry(p(w, 53.125), p(h, 25 / 3), p(w, 6.25), p(h, 25 / 3));
    add_folder_songs->setGeometry(p(w, 62.5), p(h, 25 / 3), p(w, 6.25), p(h, 25 / 3));
    name_label->setGeometry(p(w, 71.875), p(h, 25 / 3), p(w, 21.875), p(h, 25 / 3));

    slider_volume->setGeometry(p(w, 71.875), p(h, 50 / 3), p(w, 21.875), p(h, 25 / 3));
    slider_time->setGeometry(p(w, 6.25), p(h, 70 / 3), p(w, 75), p(h, 25 / 3));

    time_label->setGeometry(p(w, 84.375), p(h, 70 / 3), p(w, 9.375), p(h, 25 / 3));

    playlist->setGeometry(p(w, 6.25), p(h, 112.5 / 3), p(w, 42.5), p(h, 175 / 3));

    video_widget->setGeometry(p(w, 51.25), p(h, 112.5 / 3), p(w, 42.5), p(h, 175 / 3));
    image_lbl->setGeometry(p(w, 51.25), p(h, 112.5 / 3), p(w, 42.5), p(h, 175 / 3));
}

void MainWindow::handle_next()
{
    playlist->setNext();
}

void MainWindow::handle_prev()
{
    playlist->setPrev();
}

void MainWindow::handleCurrentChanged(qint64 curr)
{
    media_player_button->setText("▶");
    media_player->setSource(QUrl(playlist->getCurrentSong()));

    if (playlist->getCurrentSong().endsWith(".mp3"))
    {
        image_lbl->setVisible(true);
        video_widget->hide();
    }

    else
    {
        image_lbl->setVisible(false);
        video_widget->show();
    }

    QStringList list = playlist->getCurrentSong().split("/");
    name_label->setText(list[list.size() - 1]);
}
