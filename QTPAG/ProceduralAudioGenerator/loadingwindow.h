#ifndef LOADINGWINDOW_H
#define LOADINGWINDOW_H

#include <QDialog>

namespace Ui {
class LoadingWindow;
}

class LoadingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoadingWindow(QWidget *parent = nullptr);
    ~LoadingWindow();

private:
    Ui::LoadingWindow *ui;
};

#endif // LOADINGWINDOW_H
