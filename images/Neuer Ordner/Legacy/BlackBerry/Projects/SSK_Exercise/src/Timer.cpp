
#include "Timer.hpp"

Timer::Timer(QObject *parent)
	: bb::cascades::CustomControl((bb::cascades::Container *) parent)
	, _timer(new QTimer(this))
{
    connect(_timer, SIGNAL(timeout()), this, SIGNAL(timeout()));
    setVisible(false);
}

Timer::~Timer() {}

bool Timer::isActive() {
    return _timer->isActive();
}

int Timer::timeLimit() {
    return _timer->interval();
}

void Timer::setTimeLimit(int mSec) {
    if (_timer->interval() == mSec)
        return;

    _timer->setInterval(mSec);
    emit timeLimitChanged();
}

void Timer::start() {
    if (_timer->isActive())
        return;

    _timer->start();
    emit activeChanged();
}

void Timer::stop() {
    if (!_timer->isActive())
        return;

    _timer->stop();
    emit activeChanged();
}
