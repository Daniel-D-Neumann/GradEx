#ifndef PAGWINDOW_H
#define PAGWINDOW_H

#include <QMainWindow>
class SoundGenerator;
class QMediaPlayer;
class QAudioOutput;

QT_BEGIN_NAMESPACE
namespace Ui {
class PAGWindow;
}
QT_END_NAMESPACE

class PAGWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PAGWindow(SoundGenerator* gen, QWidget *parent = nullptr);
    ~PAGWindow() override;

private slots:
    void on_B_MusicChooseFilePath_clicked();

    void on_B_LoadMusicIntoGenerator_clicked();

    void on_B_PlayGeneratedMusic_clicked();

private:
    Ui::PAGWindow *ui;
    SoundGenerator* generator;
    QMediaPlayer* med_player;
    QAudioOutput* audio_out;
};
#endif // PAGWINDOW_H
