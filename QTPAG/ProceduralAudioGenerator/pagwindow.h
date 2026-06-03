#ifndef PAGWINDOW_H
#define PAGWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class PAGWindow;
}
QT_END_NAMESPACE

class PAGWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PAGWindow(QWidget *parent = nullptr);
    ~PAGWindow() override;

private slots:
    void on_B_MusicChooseFilePath_clicked();

private:
    Ui::PAGWindow *ui;
};
#endif // PAGWINDOW_H
