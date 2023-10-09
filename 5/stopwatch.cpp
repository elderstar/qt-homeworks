#include "stopwatch.h"

Stopwatch::Stopwatch(QObject *parent)
    : QObject{parent}
{
    timer       =   new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &Stopwatch::setTime);
}

Stopwatch::~Stopwatch()
{
}

void Stopwatch::ToggleStopwatch()
{

    if(timer->isActive())
    {
        timer->stop();
    }else{
        timer->start(100);
    }
    emit sig_SendToggleSig();
}

void Stopwatch::ResetStopwatch()
{
    time = 0;
    time_lap = 0;
    laps_count = 0;
    time_lap_prev = 0;
    emit sig_SendResetSig();
}

void Stopwatch::RecordLapTime()
{
    ++laps_count;
    time_lap = time;
    emit sig_SendRecLapTimeSig();
}

size_t Stopwatch::getLapsCount()
{
    return laps_count;
}

void Stopwatch::setTime()
{
    time += 100;
    emit sig_TimerChanged();
}

QString Stopwatch::c_str(const unsigned int& time)
{
    std::chrono::milliseconds ms (time);
    std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(ms);
    ms -= std::chrono::duration_cast<std::chrono::milliseconds>(s);
    std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(ms);
    s -= std::chrono::duration_cast<std::chrono::seconds>(ms);
    return QString::number(m.count())+":"+QString::number(s.count())+"."+QString::number(ms.count()/100);
}

unsigned int Stopwatch::getTime()
{
    return time;
}

unsigned int Stopwatch::getLastLapTime()
{
    double new_lap = time_lap - time_lap_prev;
    time_lap_prev += new_lap;

    return new_lap;
}



