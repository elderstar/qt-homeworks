#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>
#include <chrono>

class Stopwatch : public QObject
{
    Q_OBJECT
public:
    explicit Stopwatch(QObject *parent = nullptr);
    ~Stopwatch();
    void ToggleStopwatch();
    void ResetStopwatch();
    void RecordLapTime();
    size_t getLapsCount();
    unsigned int getTime();
    unsigned int getLastLapTime();
    void setTime();
    QString c_str(const unsigned int& time);
signals:
    void sig_SendToggleSig();
    void sig_SendResetSig();
    void sig_SendRecLapTimeSig();
    void sig_TimerChanged();
private:

    QTimer *timer;
    unsigned int time = 0;
    unsigned int time_lap = 0;
    unsigned int time_lap_prev = 0;
    size_t  laps_count = 0;
};

#endif // STOPWATCH_H
