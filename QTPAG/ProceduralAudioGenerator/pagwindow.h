#ifndef PAGWINDOW_H
#define PAGWINDOW_H

#include <QMainWindow>
class SoundGenerator;
class QMediaPlayer;
class QMediaCaptureSession;
class QMediaRecorder;
class QAudioOutput;
class QAudioInput;
class QTableWidgetItem;

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

    void on_TB_Instruments_itemClicked(QTableWidgetItem *item);

    void on_B_InsSampleGenerate_clicked();

    void on_B_PlayGeneratedInsSample_clicked();

    void on_B_PauseGeneratedInsSample_clicked();

    void on_B_ClearCustomInstruments_clicked();

    void on_B_RecordingChooseFilePath_clicked();

    void on_B_RecordAudio_clicked();

    void on_B_EndRecordAudio_clicked();

private:
    Ui::PAGWindow *ui;
    SoundGenerator* generator;
    QMediaPlayer* med_player;
    QAudioOutput* audio_out;

    QMediaCaptureSession* capture_session;
    QAudioInput* audio_in;
    QMediaRecorder* recorder;

    QString music_File_Path;
    QString ins_File_Path;

    int current_instrument_table_rows = 10;
    int currnet_custom_instrument_count = 0;
    double wav_MSF = 0;
    int selected_instrument_row = 0;
    int selected_instrument_column = 0;
    int generated_instrument_row = -1;
    bool is_Instrument_Sample_Playing = false;
};
#endif // PAGWINDOW_H
