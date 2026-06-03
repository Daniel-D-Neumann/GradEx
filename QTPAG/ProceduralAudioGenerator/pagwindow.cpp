#include "pagwindow.h"
#include "./ui_pagwindow.h"
#include <QFileDialog>

PAGWindow::PAGWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PAGWindow)
{
    ui->setupUi(this);
}

PAGWindow::~PAGWindow()
{
    delete ui;
}

QString musicFilePath;

void PAGWindow::on_B_MusicChooseFilePath_clicked()
{
    musicFilePath = QFileDialog::getOpenFileName(this, tr("Open Song"), "../MidiSongs", tr("Music Files (*.MIDI)"));
    ui->TI_MusicFilePath->setText(musicFilePath);
}