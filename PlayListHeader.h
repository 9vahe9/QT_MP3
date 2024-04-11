/*
#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDir>

class PlayList : public QLabel
{
    Q_OBJECT
public:
    PlayList(QWidget* parent = nullptr);

public:
    void append_songs();
    QString getSong();
    void setNext();
    void setPrev();

private:
    QPushButton** song_buttons;
    QStringList songs;
    qint64 current_song_index{0};
    QString songs_folder_path;
    QString current_song_full_path;
    QVBoxLayout* m_vLayout;

signals:
    void songIsReady();

private:
    void setSong(int index);
};

#endif // PLAYLIST_H
*/
