#include "../includes/Ecg.hpp"

Ecg::Ecg(std::list<int> data, QWidget *parent) : QWidget(parent), count(0), h(900), w(1300), sec(0), start_b(false), stop_b(false), data(data), size(static_cast<int>(data.size())), timer(new QTimer(this))
{
	hbox = new QHBoxLayout(this);
	hbox->setSpacing(5);

	startBtn = new QPushButton("Play", this);
	hbox->addWidget(startBtn, 0, Qt::AlignLeft | Qt::AlignTop);

	connect(startBtn, &QPushButton::clicked, this, &Ecg::start);

	stopBtn = new QPushButton("Stop", this);
	hbox->addWidget(stopBtn, 1, Qt::AlignLeft | Qt::AlignTop);

	connect(stopBtn, &QPushButton::clicked, this, &Ecg::stop);

	label = new QLabel("", this);

	label->move(200, 8);

	QTime qtime = QTime(0, 0, sec, 0);
	QString stime = qtime.toString();
	label->setText(stime);

	startTimer(1000);

	slider = new QSlider(Qt::Horizontal, this);
	slider->setRange(0, 99);
	slider->setValue(50);
	hbox->addWidget(slider, 0, Qt::AlignRight | Qt::AlignBottom);

	label1 = new QLabel("50", this);
	labelp = new QLabel("%", this);
	hbox->addWidget(label1, 0, Qt::AlignRight | Qt::AlignBottom);
	hbox->addWidget(labelp, 0, Qt::AlignRight | Qt::AlignBottom);

	connect(slider, &QSlider::valueChanged, label1, qOverload<int>(&QLabel::setNum));

	ms4 = new QLabel("0", this);
	ms1 = new QLabel(std::to_string((40000/4) * 3).c_str(), this);
	ms2 = new QLabel(std::to_string((40000/4) * 2).c_str(), this);
	ms3 = new QLabel(std::to_string((40000/4)).c_str(), this);
	ms5 = new QLabel(std::to_string((40000/4) * -1).c_str(), this);
	ms6 = new QLabel(std::to_string((40000/4) * -2).c_str(), this);
	ms7 = new QLabel(std::to_string((40000/4) * -3).c_str(), this);
	ms4->move(5, h/2 - 30);
	ms1->move(5, h/2 - ((h/2)/4) * 3 - 30);
	ms2->move(5, h/2 - ((h/2)/4) * 2 - 30);
	ms3->move(5, h/2 - ((h/2)/4) - 30);
	ms5->move(5, h/2 + ((h/2)/4) - 30);
	ms6->move(5, h/2 + ((h/2)/4) * 2 - 30);
	ms7->move(5, h/2 + ((h/2)/4) * 3 - 30);

	transformData();
}

int		Ecg::getH() const {
	return h;
}

int		Ecg::getW() const {
	return w;
}

void	Ecg::stop() {
	transformData();
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
}

void	Ecg::start() {	
	disconnect(startBtn, &QPushButton::clicked, this, &Ecg::start);
	startBtn->setText("Pause");
	connect(startBtn, &QPushButton::clicked, this, &Ecg::pause);
	start_b = true;
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(6000 / w);
}

void	Ecg::transformData() {
	nums.clear();
	int c = 0;
	float sum = 0;
	float m;
	for (std::list<int>::iterator it = data.begin(); it != data.end(); it++) {
		sum+=static_cast<float>(*it);
		c++;
		if (c >= ((size / 4) / w) || ++it  == data.end()) {
			m = sum / c;
			c = 0;
			sum = 0;
			nums.push_back(m);
			it--;
		}
	}
	size_nums = nums.size();
}

void Ecg::drawEcg(QPainter *qp) {
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


	int zoom = 80000 * (1 - (atoi(label1->text().toStdString().c_str())/static_cast<float>(100)));

	ms1->setText(std::to_string((zoom/4) * 3).c_str());
	ms2->setText(std::to_string((zoom/4) * 2).c_str());
	ms3->setText(std::to_string((zoom/4)).c_str());
	ms5->setText(std::to_string((zoom/4) * -1).c_str());
	ms6->setText(std::to_string((zoom/4) * -2).c_str());
	ms7->setText(std::to_string((zoom/4) * -3).c_str());

	if (start_b) {	
		if (static_cast<int>(nums.size()) <= (size_nums / 4) && !stop_b) {
			pause();
			return ;
		}
		if (!stop_b) {
			if (count >= w)
				nums.pop_front();
			count+=1;
		}
		pen.setWidth(3);
		pen.setColor(Qt::blue);
		qp->setPen(pen);
		int xa = 0;
		int ya = h/2;
		int xb = 0;
		int yb = 0;
		int i = 0;
		for (std::list<float>::iterator it = nums.begin(); it != nums.end(); it++) {
			if (*it >= 0)
				yb = h/2 - (*it /(zoom / (h/2)));
			else
				yb = h/2 + ((*it * -1)/(zoom / (h/2)));
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

void Ecg::closeEvent(QCloseEvent *event)
{
	QMessageBox::StandardButton resBtn = QMessageBox::question( this, "ECG",
			tr("Are you sure?\n"),
			QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
			QMessageBox::Yes);
	if (resBtn != QMessageBox::Yes) {
		delete startBtn;
		delete stopBtn;
		delete label;
		delete slider;
		delete label1;
		delete ms1;
		delete ms2;
		delete ms3;
		delete ms4;
		delete ms5;
		delete ms6;
		delete ms7;
		delete hbox;
		event->ignore();
	} else {
		event->accept();
	}
}
