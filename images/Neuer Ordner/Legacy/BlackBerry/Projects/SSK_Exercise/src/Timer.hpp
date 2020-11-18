#include <QObject>
#include <bb/cascades/CustomControl>

#ifndef TIMER_HPP_
#define TIMER_HPP_

class Timer  : public bb::cascades::CustomControl {

	Q_OBJECT
	Q_PROPERTY(
			bool active
			READ isActive
			NOTIFY activeChanged)
	Q_PROPERTY(
			int time_limit
			READ timeLimit
			WRITE setTimeLimit
	        NOTIFY timeLimitChanged)

public:
    explicit Timer(QObject* parent = 0);
	virtual ~Timer();
    bool isActive();
    void setTimeLimit(int mSec);
    int timeLimit();

public slots:
    void start();
    void stop();

signals:
        void timeout();
        void timeLimitChanged();
        void activeChanged();
private:
    QTimer* _timer;
};

#endif /* TIMER_HPP_ */
