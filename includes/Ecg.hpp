#pragma once

#include <QWidget>
#include <list>
#include <QPainter>
#include <QTime>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>

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
		QSlider *slider;
		QLabel *label1;
		QLabel *ms1;
		QLabel *ms2;
		QLabel *ms3;
		QLabel *ms4;
		QLabel *ms5;
		QLabel *ms6;
		QLabel *ms7;
		QHBoxLayout *hbox;
};
