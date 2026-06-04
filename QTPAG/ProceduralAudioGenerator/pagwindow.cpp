#include "pagwindow.h"
#include "./ui_pagwindow.h"
#include "SoundGenerator.h"
#include <QFileDialog>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QValidator>

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

    ui->TI_EventsInMusic->setValidator(new QIntValidator(0,100,this));

    connect(med_player, &QMediaPlayer::mediaStatusChanged, this,
            &PAGWindow::Handle_Media_Player_State_Change, Qt::UniqueConnection);

    ui->TB_Instruments->setRowCount(10);
    ui->TB_Instruments->setItem(0, 0, new QTableWidgetItem("Piano"));
    ui->TB_Instruments->setItem(1, 0, new QTableWidgetItem("Marimba"));
    ui->TB_Instruments->setItem(2, 0, new QTableWidgetItem("Harmonica"));
    ui->TB_Instruments->setItem(3, 0, new QTableWidgetItem("Accoustic Guitar"));
    ui->TB_Instruments->setItem(4, 0, new QTableWidgetItem("Bass Guitar"));
    ui->TB_Instruments->setItem(5, 0, new QTableWidgetItem("Violin"));
    ui->TB_Instruments->setItem(6, 0, new QTableWidgetItem("Trumpet"));
    ui->TB_Instruments->setItem(7, 0, new QTableWidgetItem("Flute"));
    ui->TB_Instruments->setItem(8, 0, new QTableWidgetItem("Banjo"));
    ui->TB_Instruments->setItem(9, 0, new QTableWidgetItem("Drum"));

}

PAGWindow::~PAGWindow()
{
    delete generator;
    delete ui;
}

void PAGWindow::on_B_MusicChooseFilePath_clicked()
{
    music_File_Path = QFileDialog::getOpenFileName(this, tr("Open Song"), "Assets/MidiSongs", tr("Music Files (*.mid)"));

    ui->TI_MusicFilePath->setText(music_File_Path);
}

void PAGWindow::Handle_Media_Player_State_Change()
{
    switch (med_player->playbackState())
    {
    case QMediaPlayer::StoppedState:
        break;

    case QMediaPlayer::PausedState:
        ui->B_PauseGeneratedMusic->setDisabled(true);
        ui->B_PlayGeneratedMusic->setEnabled(true);
        break;
    }

    switch (med_player->mediaStatus())
    {

    case QMediaPlayer::EndOfMedia:
        ui->B_PauseGeneratedMusic->setDisabled(true);
        ui->B_PlayGeneratedMusic->setEnabled(true);
        break;
    }
}

void PAGWindow::on_B_LoadMusicIntoGenerator_clicked()
{
    QFileInfo fileInfo(music_File_Path);

    if(!fileInfo.exists() || !fileInfo.isFile() || music_File_Path.isEmpty()) return;

    std::string filePath = music_File_Path.toStdString();

    generator->Load_Music_File_Into_Generator(filePath);

    ui->CB_SongLoaded->setCheckState(Qt::Checked);
    ui->B_MusicGenerate->setEnabled(true);
}


void PAGWindow::on_B_PlayGeneratedMusic_clicked()
{
    if(med_player->playbackState() == QMediaPlayer::PausedState)
    {
        med_player->play();
        ui->B_PauseGeneratedMusic->setEnabled(true);
        ui->B_PlayGeneratedMusic->setEnabled(false);
    }
    else
    {
        QUrl file_path = QUrl::fromLocalFile("Assets/BlankGeneratedWavFile.wav");
        med_player->setSource(QUrl::fromLocalFile("Assets/BlankGeneratedWavFile.wav"));
        med_player->play();
        ui->B_PauseGeneratedMusic->setEnabled(true);
        ui->B_PlayGeneratedMusic->setEnabled(false);
    }
}

void PAGWindow::on_B_PauseGeneratedMusic_clicked()
{
    med_player->pause();
    ui->B_PauseGeneratedMusic->setDisabled(true);
    ui->B_PlayGeneratedMusic->setEnabled(true);
}

void PAGWindow::on_B_MusicGenerate_clicked()
{
    int num_events = ui->TI_EventsInMusic->text().toInt();
    if(num_events <= 0) return;

    generator->Generate_Music(num_events);
    ui->B_PlayGeneratedMusic->setEnabled(true);
}

void PAGWindow::on_B_InsChooseFilePath_clicked()
{
    ins_File_Path = QFileDialog::getOpenFileName(this, tr("Open Song"), "Assets/InstrumentSamples", tr("Instrument Samples (*.wav)"));

    ui->TI_InsFilePath->setText(ins_File_Path);
}


void PAGWindow::on_B_LoadInsIntoGenerator_clicked()
{
    QFileInfo fileInfo(ins_File_Path);

    if(!fileInfo.exists() || !fileInfo.isFile() || ins_File_Path.isEmpty()) return;

    std::string file_Path = ins_File_Path.toStdString();

    generator->Load_Wav_File_Into_Generator(file_Path);

    ui->CB_WavLoaded->setCheckState(Qt::Checked);
    ui->B_InsGenerate->setEnabled(true);
}


void PAGWindow::on_B_InsGenerate_clicked()
{
    wav_MSF = generator->Generate_Instrument_From_Wav();
}

