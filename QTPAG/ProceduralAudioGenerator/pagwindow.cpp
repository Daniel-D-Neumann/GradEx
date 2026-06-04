#include "pagwindow.h"
#include "./ui_pagwindow.h"
#include "SoundGenerator.h"
#include <QFileDialog>
#include <QMediaPlayer>
#include <QAudioOutput>

PAGWindow::PAGWindow(SoundGenerator* gen, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PAGWindow)
{
    ui->setupUi(this);

    generator = gen;

    med_player = new QMediaPlayer(this);
    audio_out = new QAudioOutput(this);
    med_player->setAudioOutput(audio_out);
    audio_out->setVolume(0.5f);
}

PAGWindow::~PAGWindow()
{
    delete generator;
    delete ui;
}

QString music_File_Path;
QMediaPlayer media_Player;
QAudioOutput audio_Output;

void PAGWindow::on_B_MusicChooseFilePath_clicked()
{
    music_File_Path = QFileDialog::getOpenFileName(this, tr("Open Song"), "Assets/MidiSongs", tr("Music Files (*.mid)"));
    ui->TI_MusicFilePath->setText(music_File_Path);
}



void PAGWindow::on_B_LoadMusicIntoGenerator_clicked()
{
    std::string filePath = music_File_Path.toStdString();
    generator->Load_Music_File_Into_Generator(filePath);

    ui->CB_SongLoaded->setCheckState(Qt::Checked);
}


void PAGWindow::on_B_PlayGeneratedMusic_clicked()
{
    med_player->setSource(QUrl::fromLocalFile("Assets/MyGeneratedWavFile.wav"));
    med_player->play();
}

