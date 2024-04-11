/*
#include "PlayListHeader.h"

PlayList::PlayList(QWidget* parent):
    QLabel(parent),
    m_vLayout(new QVBoxLayout)
{
    this->setStyleSheet("background-color: #454545;");
    this->setLayout(m_vLayout);

    songs_folder_path = "/Users/vahe/Qt_Projects/MP3_player/Song_list/";
    QDir directory(songs_folder_path);
    songs = directory.entryList(QStringList() << "*.mp3", QDir::Files);

    append_songs();
}

void PlayList::append_songs()
{
    song_buttons = new QPushButton* [songs.size()];

    for (int i = 0; i < songs.size(); ++i)
    {
        QStringList list = songs[i].split("/");
        song_buttons[i] = new QPushButton(list[list.size() - 1]);
        song_buttons[i]->setStyleSheet("background-color: dimGray;");

        connect(song_buttons[i], &QPushButton::clicked, [i, this](){
            setSong(i);
        });

        m_vLayout->addWidget(song_buttons[i]);
    }
}

void PlayList::setSong(int index)
{
    if (index < 0)
    {
        return;
    }

    current_song_full_path = songs_folder_path + songs[index];
    emit songIsReady();
}

void PlayList::setNext()
{
    if (current_song_index + 1 == songs.size())
    {
        current_song_index = 0;
    }

    else
    {
        ++current_song_index;
    }

    setSong(current_song_index);
}

void PlayList::setPrev()
{
    if (current_song_index == 0)
    {
        current_song_index = songs.size() - 1;
    }

    else
    {
        --current_song_index;
    }

    setSong(current_song_index);
}

QString PlayList::getSong()
{
    return current_song_full_path;
}
*/
