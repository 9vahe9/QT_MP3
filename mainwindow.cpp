#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    // , ui(new Ui::MainWindow)
    , play_flag(false)
    , media_player_button(new QPushButton(this))
    , media_stop_button(new QPushButton(this))
    , media_player(new QMediaPlayer(this))
    , audio_output(new QAudioOutput(this))
    , slider1(new QSlider(Qt::Orientation::Horizontal, this))
    , slider2(new QSlider(Qt::Orientation::Horizontal, this))
    , time_label(new QLabel("", this))
    , name_label(new QLabel("", this))
{
    // ui->setupUi(this);
    this->setGeometry(100, 200, 800, 200);
    this->setFixedSize(800, 200);

    media_player_button->setGeometry(50, 50, 100, 50);
    media_player_button->setText("▶");
    media_player_button->setStyleSheet("background-color: dimGray;");

    media_stop_button->setGeometry(200, 50, 100, 50);
    media_stop_button->setStyleSheet("background-color: dimGray;");
    media_stop_button->setText("⏮");

    slider1->setGeometry(600, 50, 150, 50);
    slider2->setGeometry(50, 120, 600, 50);

    time_label->setGeometry(675, 115, 100, 50);
    time_label->setStyleSheet("background-color: dimGray;");

    name_label->setGeometry(350, 50, 200, 50);
    name_label->setStyleSheet("background-color: dimGray;");

    media_player->setAudioOutput(audio_output);

    path = "/Users/vahe/Downloads/price-of-freedom.mp3";

    media_player->setSource(QUrl(path));


    QStringList list = path.split("/");
    song_name = list[list.size() - 1];

    name_label->setAlignment(Qt::AlignCenter);
    name_label->setText(song_name);

    connect(media_player_button, &QPushButton::clicked, [this](){
        if (!play_flag)
        {
            media_player->play(); // ham miacnum ham pause dnum;
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
    });
}

MainWindow::~MainWindow()
{
    //delete ui;
}
