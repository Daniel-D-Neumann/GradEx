#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include "../../SoundGenerator.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString musicFilePath;

void MainWindow::on_B_MusicChooseFilePath_clicked()
{
    musicFilePath = QFileDialog::getOpenFileName(this, tr("Open Song"), "../MidiSongs", tr("Music Files (*.MIDI)"));
    ui->TI_MusicFilePath->setText(musicFilePath);
}

