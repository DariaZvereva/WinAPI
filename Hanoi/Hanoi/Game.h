#pragma once
#include <vector>
#include <stack>  

struct GameStep
{
	size_t from_;
	size_t to_;

	GameStep() : from_(0), to_(0) {}

	GameStep(size_t from, size_t to) : 
				from_(from), to_(to) {}
	bool operator==(GameStep &other) {
		return ((this->from_ == other.from_) && (this->to_ == other.to_));
	}
	void reverse() {
		size_t oldFrom = from_;
		from_ = to_;
		to_ = oldFrom;
	}
};

class Game
{
public:
	Game(size_t towers=3, size_t disks=5);
	/*
	void Load() {
		std::ifstream in;
		in.open(path);
		in >> numberOfTowers >> numberOfDisks;
		currentPosition.resize(numberOfTowers);
		for (int i = 0; i < numberOfTowers; ++i) {
			int n;
			in >> n;
			std::vector<size_t> v(n);
			for (int j = 0; j < n; ++j) {
				in >> v[j];
			}
			for (int j = 0; j < n; ++j) {
				currentPosition[i].push(j);
			}
		}
		in.close();
	}
	void Save() {
		std::ofstream out;
		out.open(path);
		out << numberOfTowers << ' ' << numberOfDisks << std::endl;
		std::vector<std::stack<size_t> > copy(currentPosition);
		for (int i = 0; i < numberOfTowers; ++i) {
			out << copy[i].size();
			while (!copy[j].empty()) {
				out << ' ' << copy[j].top();
				copy[j].pop();
			}
			out << std::endl;
		}
		out.close();
	}*/

	void doStep(GameStep &step);
	bool isCorrectStep(GameStep &step);
	bool isWin();
	size_t getNumberOfDisks() {
		return numberOfDisks;
	}
	size_t getNumberOfTowers() {
		return numberOfTowers;
	}
	std::pair<size_t, size_t> getPositionOfDisk(size_t disk);
	int getNumberOfSteps() {
		return numberOfSteps;
	}
	GameStep getPrevStep() {
		return previousStep;
	}
	bool isAvailableDisk(size_t i);
	int getTop(size_t tower);
private:
	size_t numberOfTowers;
	size_t numberOfDisks;
	int numberOfSteps;
	GameStep previousStep;
	std::vector< std::stack<size_t> > currentPosition;
	std::vector< std::pair<size_t, size_t> > positionsOfDisks;


};

