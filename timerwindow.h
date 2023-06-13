#ifndef TIMERWINDOW_H
#define TIMERWINDOW_H

#include <QDialog>
#include <ceffekte.h>

namespace Ui {
class TimerWindow;
}

class QTimer;

class TimerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TimerWindow(CEffekte EffektWerte, QWidget *parent = nullptr);
    ~TimerWindow();

public slots:
    void updateProgress(); // Aktualisierung der Progress Bar
    void closeEvent(QCloseEvent *event); // Routine bei Timer Ende

private:
    Ui::TimerWindow *ui;
    QTimer *timer;
    QDateTime startTime;
    int totalDuration;
};

#endif // TIMERWINDOW_H
