#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <list>
#include <QApplication>
#include <QFrame>
#include <QGridLayout>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QScreen>
#include <QHBoxLayout>
#include <QTime>
#include <QTimer>
#include <unistd.h>
#include <QSlider>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QSlider>
#include <QCloseEvent>
#include <QMessageBox>

class Ecg : public QWidget {

	public:
		Ecg(std::list<int> data, QWidget *parent = 0);
		int	getH() const;
		int getW() const;
		void paintEvent(QPaintEvent *event);
		void drawEcg(QPainter *qp);
		void start();
		void pause();
		void stop();
		void timerEvent(QTimerEvent *e);
		void transformData();
		void closeEvent(QCloseEvent *event);

	private:
		int count;
		int h;
		int w;	
		int sec;
		bool start_b;
		bool stop_b;
		std::list<float> nums;
		std::list<int> data;
		int size;
		int size_nums;
		QLabel *label;
		QPushButton *startBtn;
		QPushButton *stopBtn;
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
		QLabel *labelp;
		QHBoxLayout *hbox;
};
