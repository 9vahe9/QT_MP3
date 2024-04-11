#ifndef VIDEOMEDIA_H
#define VIDEOMEDIA_H

#include <QObject>
#include <QWidget>
#include <QLabel>

class VideoMedia : public QLabel
{
    Q_OBJECT
public:
    VideoMedia(QWidget* parent = nullptr);
};

#endif // VIDEOMEDIA_H
