#include "Ecg.cpp"

int main(int argc, char **argv)
{
	std::ifstream data("data_ecg/ecg.txt");
	std::string line;
	std::list<int>	nums;

	if (data.is_open()) {
		while (data) {
			if (!line.empty())
				nums.push_back(atoi(line.c_str()));
			std::getline(data, line);
		}
	data.close();
	}
	else
		std::cout << "Couldn't open file" << std::endl;

	QApplication app(argc, argv);

	Ecg window(nums);

	window.setFixedSize(window.getW(), window.getH());
	window.setWindowTitle("ECG");
	window.show();

	return app.exec();
}
