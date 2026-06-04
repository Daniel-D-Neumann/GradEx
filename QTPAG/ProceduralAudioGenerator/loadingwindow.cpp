#include "loadingwindow.h"
#include "ui_loadingwindow.h"
#include <QLabel>
#include <QMovie>

LoadingWindow::LoadingWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoadingWindow)
{
    ui->setupUi(this);

    QLabel* lbl = new QLabel(this);
    lbl->move(150,125);
    lbl->setText("LOADING...");
    //QMovie* movie = new QMovie("../../GUIassets/LoadingGIF.gif");

    //lbl->setMovie(movie);
    lbl->show();
    //movie->start();

}

LoadingWindow::~LoadingWindow()
{
    delete ui;
}
