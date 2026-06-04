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
    ui->TI_Attack->setValidator(new QDoubleValidator(0, 1, 3, this));
    ui->TI_Decay->setValidator(new QDoubleValidator(0, 1, 3, this));
    ui->TI_Sustain->setValidator(new QDoubleValidator(0, 1, 3, this));
    ui->TI_Release->setValidator(new QDoubleValidator(0, 1, 3, this));

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

    int num_custom = generator->GetNumberOfCustomInstruments();
    for (int i = 0; i < num_custom; ++i)
    {
        if(currnet_custom_instrument_count > current_instrument_table_rows)
        {
            ui->TB_Instruments->setRowCount(++current_instrument_table_rows);
        }
        QString name = QString::fromStdString("Custom_" + std::to_string(currnet_custom_instrument_count));
        ui->TB_Instruments->setItem(currnet_custom_instrument_count, 1, new QTableWidgetItem(name));

        currnet_custom_instrument_count++;
    }

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
        ui->B_PauseGeneratedInsSample->setDisabled(true);
        ui->B_PlayGeneratedInsSample->setEnabled(true);
        break;
    }

    switch (med_player->mediaStatus())
    {

    case QMediaPlayer::EndOfMedia:
        ui->B_PauseGeneratedMusic->setDisabled(true);
        ui->B_PlayGeneratedMusic->setEnabled(true);
        ui->B_PauseGeneratedInsSample->setDisabled(true);
        ui->B_PlayGeneratedInsSample->setEnabled(true);
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
    if(currnet_custom_instrument_count > current_instrument_table_rows)
    {
        ui->TB_Instruments->setRowCount(++current_instrument_table_rows);
    }
    QString name = QString::fromStdString("Custom_" + std::to_string(currnet_custom_instrument_count));
    ui->TB_Instruments->setItem(currnet_custom_instrument_count, 1, new QTableWidgetItem(name));

    generated_instrument_row = currnet_custom_instrument_count;
    currnet_custom_instrument_count++;
}


void PAGWindow::on_TB_Instruments_itemClicked(QTableWidgetItem *item)
{
    ui->B_InsSampleGenerate->setEnabled(true);
    selected_instrument_column = item->column();
    selected_instrument_row = item->row();
}

void PAGWindow::on_B_InsSampleGenerate_clicked()
{
    ADSREnvelope env = ADSREnvelope(ui->TI_Attack->text().toDouble(),ui->TI_Decay->text().toDouble(),ui->TI_Sustain->text().toDouble(),ui->TI_Release->text().toDouble());
    std::vector<FrequencyBreakdown>* freqs = generator->GetInstrumentFreqs(selected_instrument_column, selected_instrument_row);

    Instrument* custIns = new Instrument(env,*freqs);
    Instrument* silence = new Silence();
    std::vector<double> harmC, harmA, harmD4, harmD5, harmE, harmG, sile;

    double MSF = 440.0;
    if(generated_instrument_row == selected_instrument_row)
    {
        MSF = wav_MSF;
    }

    custIns->Sound(&harmC,1.f, MSF,1.f,44100.f);
    custIns->Sound(&harmD4,0.2f,custIns->MoveSemitones(MSF,1),1.f,44100.f);
    custIns->Sound(&harmD5,0.2f, custIns->MoveSemitones(MSF, 13),1.f,44100.f);
    custIns->Sound(&harmA, 0.2f, custIns->MoveSemitones(MSF, 9), 1.f, 44100.f);
    custIns->Sound(&harmE, 1.f, custIns->MoveSemitones(MSF, 4), 1.f, 44100.f);
    custIns->Sound(&harmG, 1.f, custIns->MoveSemitones(MSF, 7), 1.f, 44100.f);
    silence->Sound(&sile, 0.2f, 0.f, 0.f, 44100.f);

    for (int i = 0; i<harmC.size(); i++)
    {
        harmC[i] += harmE[i] + harmG[i];
    }

    WavWriter* output =  new WavWriter("Assets/InstrumentSample.wav");
    output->WriteAudioToFile(harmD4);
    output->WriteAudioToFile(harmD4);
    output->WriteAudioToFile(harmD5);
    output->WriteAudioToFile(sile);
    output->WriteAudioToFile(harmA);
    output->WriteAudioToFile(sile);
    output->WriteAudioToFile(harmC);

    ui->B_PlayGeneratedInsSample->setEnabled(true);
}


void PAGWindow::on_B_PlayGeneratedInsSample_clicked()
{
    if(med_player->playbackState() == QMediaPlayer::PausedState && is_Instrument_Sample_Playing)
    {
        med_player->play();
        ui->B_PauseGeneratedInsSample->setEnabled(true);
        ui->B_PlayGeneratedInsSample->setDisabled(true);
        ui->B_PlayGeneratedMusic->setDisabled(true);
        ui->B_PauseGeneratedMusic->setDisabled(true);
    }
    else
    {
        med_player->setSource(QUrl::fromLocalFile("Assets/InstrumentSample.wav"));
        med_player->play();
        ui->B_PauseGeneratedInsSample->setEnabled(true);
        ui->B_PlayGeneratedInsSample->setDisabled(true);
        ui->B_PlayGeneratedMusic->setDisabled(true);
        ui->B_PauseGeneratedMusic->setDisabled(true);
        is_Instrument_Sample_Playing = true;
    }
}

void PAGWindow::on_B_PlayGeneratedMusic_clicked()
{
    if(med_player->playbackState() == QMediaPlayer::PausedState && !is_Instrument_Sample_Playing)
    {
        med_player->play();
        ui->B_PauseGeneratedMusic->setEnabled(true);
        ui->B_PlayGeneratedMusic->setDisabled(true);
        ui->B_PlayGeneratedInsSample->setDisabled(true);
        ui->B_PauseGeneratedInsSample->setDisabled(true);
    }
    else
    {
        med_player->setSource(QUrl::fromLocalFile("Assets/BlankGeneratedWavFile.wav"));
        med_player->play();
        ui->B_PauseGeneratedMusic->setEnabled(true);
        ui->B_PlayGeneratedMusic->setEnabled(false);
        ui->B_PlayGeneratedInsSample->setDisabled(true);
        ui->B_PauseGeneratedInsSample->setDisabled(true);
        is_Instrument_Sample_Playing = false;
    }
}

void PAGWindow::on_B_PauseGeneratedMusic_clicked()
{
    med_player->pause();
    ui->B_PauseGeneratedMusic->setDisabled(true);
    ui->B_PlayGeneratedMusic->setEnabled(true);
    ui->B_PlayGeneratedInsSample->setEnabled(true);
}



void PAGWindow::on_B_PauseGeneratedInsSample_clicked()
{
    med_player->pause();
    ui->B_PauseGeneratedInsSample->setDisabled(true);
    ui->B_PlayGeneratedInsSample->setEnabled(true);
    ui->B_PlayGeneratedMusic->setEnabled(true);
}

