#include "includes/Ecg.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <list>
#include <QtCore>
#include <QApplication>
#include <QFrame>
#include <QGridLayout>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QPainter>
#include <QScreen>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTime>
#include <unistd.h>
#include <QThread>
#include <thread>

void	Ecg::stop() {
	nums.clear();
	nums = data;
	stop_b = false;
	start_b = false;
	sec = 0;
	count = 0;
	disconnect(startBtn, &QPushButton::clicked, this, &Ecg::pause);
	startBtn->setText("Play");
	connect(startBtn, &QPushButton::clicked, this, &Ecg::start);
	update();
	timer->stop();
}

void	Ecg::pause() {
	if (stop_b == false) {
		stop_b = true;
		startBtn->setText("Play");
	}
	else {
		stop_b = false;
		startBtn->setText("Pause");
	}
	//connect(startBtn, &QPushButton::clicked, this, &Ecg::stop);
}

void	Ecg::start() {	
	disconnect(startBtn, &QPushButton::clicked, this, &Ecg::start);
	startBtn->setText("Pause");
	connect(startBtn, &QPushButton::clicked, this, &Ecg::pause);
	start_b = true;
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(6000 / w);
}

Ecg::Ecg(std::list<int> nums, QWidget *parent) : QWidget(parent), count(0), size(static_cast<int>(nums.size())), h(900), w(1300), sec(0), nums(nums), start_b(false), stop_b(false), data(nums), timer(new QTimer(this))
{
	QHBoxLayout *hbox = new QHBoxLayout(this);
	hbox->setSpacing(5);

	startBtn = new QPushButton("Play", this);
	hbox->addWidget(startBtn, 0, Qt::AlignLeft | Qt::AlignTop);

	connect(startBtn, &QPushButton::clicked, this, &Ecg::start);

	stopBtn = new QPushButton("Stop", this);
	hbox->addWidget(stopBtn, 0, Qt::AlignLeft | Qt::AlignTop);

	connect(stopBtn, &QPushButton::clicked, this, &Ecg::stop);

	label = new QLabel("", this);
	hbox->addWidget(label, 0, Qt::AlignRight | Qt::AlignBottom);

	QTime qtime = QTime(0, 0, sec, 0);
	QString stime = qtime.toString();
	label->setText(stime);

	startTimer(1000);

	//auto *hbox = new QHBoxLayout(this);
	//hbox->setSpacing(5);



	//QHBoxLayout *hbox_stop = new QHBoxLayout(this);
	//hbox_stop->setSpacing(5);


	//QPushButton *button = new QPushButton("&Ecg::stop", this);
}

void Ecg::drawEcg(QPainter *qp) {

	/*
	   QScreen *screen = QGuiApplication::primaryScreen();
	   QRect  screenGeometry = screen->geometry();
	   int height = screenGeometry.height();
	   int width = screenGeometry.width();
	   std::cout << height << " "<< h << std::endl;
	   std::cout << width << " " << w << std::endl;
	 */
	QPen pen(Qt::gray, 5, Qt::SolidLine);
	qp->setPen(pen);
	qp->drawLine(10, h/2, w - 10, h/2);

	pen.setWidth(1);
	qp->setPen(pen);
	qp->drawLine(10, (h/2)/4, w - 10, (h/2)/4);
	qp->drawLine(10, (h/2)/2, w - 10, (h/2)/2);
	qp->drawLine(10, ((h/2)/4) * 3, w - 10, ((h/2)/4) * 3);
	qp->drawLine(10, (h/2)/4 + h/2, w - 10, (h/2)/4 + h/2);
	qp->drawLine(10, (h/2)/2 + h/2, w - 10, (h/2)/2 + h/2);
	qp->drawLine(10, ((h/2)/4) * 3 + h/2, w - 10, ((h/2)/4) * 3 + h/2);

	if (start_b) {	
		if (static_cast<int>(nums.size()) <= ((size /4) / w) * w && !stop_b) {
			pause();
			return ;
		}
		if (!stop_b) {
			if (count >= w) {
				for (int i = 0; i < ((size / 4) / w); i++)
					nums.pop_front();
			}
			//		if (((size/4)/w) * w > static_cast<int>(nums.size()))
			//			break ;
			count+=1;
		}
		pen.setWidth(2);
		pen.setColor(Qt::blue);
		qp->setPen(pen);
		//qp.drawLine(10, h/2 + 2, w - 10, h/2 + 2);
		long long xa = 1;
		long long  ya = h/2;
		long long xb = 0;
		long long yb = 0;
		int i = 0;
		int c;
		float sum;
		float m;
		for (std::list<int>::iterator it = nums.begin(); it != nums.end(); it++) {
			sum = 0;
			c = 0;
			while (it != nums.end() && c < ((size / 4) / w)) {
				sum+=static_cast<float>(*it);
				c++;
				it++;
			}
			m = sum / ((size / 4) / w);
			if (m >= 0)
				yb = h/2 - (m /(40000 / (h/2)));
			else
				yb = h/2 + ((m * -1)/(40000 / (h/2)));
			if (i != 0)
				qp->drawLine(xa, ya, xb, yb);
			xa = xb;
			ya = yb;
			xb+=1;
			if (i++ == count)
				break ;
		}
	}
}

void Ecg::paintEvent(QPaintEvent *e) {
	Q_UNUSED(e);

	QPainter qp(this);
	drawEcg(&qp);
}

void Ecg::timerEvent(QTimerEvent *e) {

	Q_UNUSED(e);
	if (!stop_b && !start_b) {
	   QTime qtime = QTime(0, 0, sec, 0);
	   QString stime = qtime.toString();
	   label->setText(stime);
	}
	else if (!stop_b) {
		QTime qtime = QTime(0, 0, ++sec, 0);
		QString stime = qtime.toString();
		label->setText(stime);
	}
}

int main(int argc, char **argv)
{
	std::ifstream data("data_ecg/ecg.txt");
	std::string line;
	std::list<int>	nums;

	if (data.is_open()) {
		while (data) {
			if (!line.empty()) {
				//	std::cout << line << std::endl;
				nums.push_back(atoi(line.c_str()));
			}
			std::getline (data, line);
		}
	}
	else
		std::cout << "Couldn't open file" << std::endl;

	std::cout << "Qt version: " << qVersion() << std::endl;

	QApplication app(argc, argv);

	Ecg window(nums);

	window.setFixedSize(window.getW(), window.getH());
	window.setWindowTitle("ECG");
	//window.setToolTip("QWidget");
	window.show();

	return app.exec();
}
