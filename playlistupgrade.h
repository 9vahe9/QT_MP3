#ifndef PLAYLISTUPGRADE_H
#define PLAYLISTUPGRADE_H

#include <QListWidget>
#include <QSet>
#include <QString>
#include <QStringList>

class PlayListUpgrade : public QListWidget
{
    Q_OBJECT
public:
    PlayListUpgrade(QWidget *parent = nullptr);

public:
    void append_songs(QStringList &songs);
    void setNext();
    void setPrev();
    QString getCurrentSong();

private:
    QStringList m_plist;
    QSet<QString> m_set;
};

#endif // PLAYLISTUPGRADE_H
