#include "PlayListUpgrade.h"
#include <qtypes.h>

PlayListUpgrade::PlayListUpgrade(QWidget *parent)
    : QListWidget(parent)
{
}

void PlayListUpgrade::append_songs(QStringList &songs)
{
    for (auto &song : songs)
    {
        if (m_set.contains(song))
        {
            continue;
        }

        m_set.insert(song.section("/", -1));

        m_plist << song;

        this->addItem(song.section("/", -1));
    }
}

void PlayListUpgrade::setNext()
{
    qint64 curr = currentRow();
    ++curr;

    if (curr == count())
    {
        curr = 0;
    }

    setCurrentRow(curr);
}

void PlayListUpgrade::setPrev()
{
    qint64 curr = currentRow();
    --curr;

    if (curr == -1)
    {
        curr = count() - 1;
    }

    setCurrentRow(curr);
}

QString PlayListUpgrade::getCurrentSong()
{
    return m_plist[currentRow()];
}
