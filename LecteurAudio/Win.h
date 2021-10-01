#ifndef WIN_H
#define WIN_H

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QMediaPlayer>
#include <QToolButton>

namespace Ui {
class Win;
}

class Win : public QWidget
{
    Q_OBJECT

public:
    explicit Win(QWidget *parent = nullptr);
    ~Win();

private slots:
    void on_upload_clicked();
    void on_btn_allShuffle_clicked();

private:
    Ui::Win *ui;
    QButtonGroup *playList;
    QButtonGroup *playerBtn;
    QStringList *audioList;
    QMediaPlayer *player;
    QToolButton *last;
    QList <QToolButton *> *allBtn;
};

#endif // WIN_H
