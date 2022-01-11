#pragma once

#include <QWidget>
#include <list>
#include <QPainter>
#include <QTime>
#include <QLabel>
#include <QPushButton>

class Ecg : public QWidget {

	public:
		Ecg(std::list<int> nums, QWidget *parent = 0);
		int getH() {
			return h;
		}
		int getW() {
			return w;
		}

		void paintEvent(QPaintEvent *event);
		void drawEcg(QPainter *qp);
		void start();
		void pause();
		void stop();
		void timerEvent(QTimerEvent *e);

	private:
		int count;
		int size;
		int h;
		int w;
		std::list<int> nums;
		std::list<int> data;
		bool start_b;
		QLabel *label;
		int sec;
		QPushButton *startBtn;
		QPushButton *stopBtn;
		bool stop_b;
		QTimer *timer;
};
