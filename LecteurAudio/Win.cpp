#include "Win.h"
#include "ui_Win.h"
#include <QMessageBox>
#include <iostream>
#include <QFileDialog>
#include <QDirIterator>
#include <QStringList>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QList>
#include <QUrl>
#include <QtAlgorithms>
#include <QMediaMetaData>

Win::Win(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Win)
{
    ui->setupUi(this);
    ui->btn_allShuffle->setFlat(true);

    connect(ui->btn_full,&QToolButton::clicked,[=](){
        if(this->isFullScreen()){
            ui->btn_full->setIcon(QIcon("use/fullScreen.png"));
            this->showNormal();
        }
        else{
            ui->btn_full->setIcon(QIcon("use/deFull.png"));
            this->showFullScreen();
        }
    });

    playList = new QButtonGroup(this);
    player = new QMediaPlayer;
    connect(player,&QMediaPlayer::stateChanged,[=](QMediaPlayer::State state){
        if(state == QMediaPlayer::PlayingState){
            last->setIcon(QIcon("use/pause2.png"));
            ui->btn_play->setIcon(QIcon("use/pause2.png"));}
        else if(state == QMediaPlayer::PausedState){
            ui->btn_play->setIcon(QIcon("use/play.png"));
            last->setIcon(QIcon("use/minus.png"));
        }
        else if(state == QMediaPlayer::StoppedState){
            last->setIcon(QIcon("use/play2.png"));
            ui->btn_play->setIcon(QIcon("use/play2.png"));}
    });
    last = 0;
    playerBtn = new QButtonGroup;
    playerBtn->addButton(ui->btn_play,1);
    playerBtn->addButton(ui->btn_next);
    playerBtn->addButton(ui->btn_previous);
    playerBtn->addButton(ui->btn_shuffle);
    playerBtn->addButton(ui->btn_stop);
    playerBtn->addButton(ui->btn_repeat);
    connect(playerBtn,static_cast<void(QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonPressed),[=](QAbstractButton *btn){
        if(player->state()==QMediaPlayer::PlayingState){
            if(btn->text()=="stp"){
                player->stop();
            }
            else if(btn->text()=="play"){
                player->pause();
            }
        }
        else if(player->state()==QMediaPlayer::PausedState){
            if(btn->text()=="play" or btn == last){
                player->play();
            }
        }
    });

}

Win::~Win()
{
    delete ui;
}


void Win::on_upload_clicked()
{
    QString dossier = QFileDialog::getExistingDirectory(this);
    ui->filePlace->setText(dossier);
    QDirIterator *audio = new QDirIterator(dossier,{"*.mp3", "*.mp4"}, QDir::Files);
    audioList = new QStringList;
    QString title;
    playList = new QButtonGroup;
    allBtn = new QList <QToolButton *>;
    int i=0;

    while (audio->hasNext()) {
        title = audio->next();
        audioList->append(title);
        QToolButton *tool = new QToolButton;
            tool->setIcon(QIcon("use/play2.png"));
            tool->setText(title);
            playList->addButton(tool,i);
            allBtn->append(tool);
        QLabel *lab = new QLabel(title.replace(dossier+"/",""));
        ui->titleLay->addWidget(tool,i,0);
        ui->titleLay->addWidget(lab,i,1);
        i++;
    }
    connect(playList,static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),[=](int id){
        if(playList->button(id) != last){
            if(last!=0){last->setIcon(QIcon("use/play2.png"));}
            allBtn->at(id)->setIcon(QIcon("use/pause2.png"));
            last = allBtn->at(id);
            connect(last,&QAbstractButton::clicked,[=](){
                if(player->state()==QMediaPlayer::PausedState){
                    ui->btn_play->setIcon(QIcon("use/play.png"));
                    allBtn->at(id)->setIcon(QIcon("use/minus.png"));
                    player->play();
                }
                else if(player->state()==QMediaPlayer::PlayingState){
                    ui->btn_play->setIcon(QIcon("use/pause.png"));
                    last->setIcon(QIcon("use/pause2.png"));
                    player->pause();
                }
            });
            player->setMedia(QUrl::fromLocalFile(playList->button(id)->text()));
            player->play();
        }
    });
    QList<QUrl> audioPlay = QUrl::fromStringList(*audioList);
}

void Win::on_btn_allShuffle_clicked()
{}
