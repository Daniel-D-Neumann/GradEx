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

    void on_B_MusicGenerate_clicked();

    void Handle_Media_Player_State_Change();

    void on_B_PauseGeneratedMusic_clicked();

    void on_B_InsChooseFilePath_clicked();

    void on_B_LoadInsIntoGenerator_clicked();

    void on_B_InsGenerate_clicked();

private:
    Ui::PAGWindow *ui;
    SoundGenerator* generator;
    QMediaPlayer* med_player;
    QAudioOutput* audio_out;

    QString music_File_Path;
    QString ins_File_Path;

    int current_instrument_table_rows = 10;
    double wav_MSF = 0;
};
#endif // PAGWINDOW_H
