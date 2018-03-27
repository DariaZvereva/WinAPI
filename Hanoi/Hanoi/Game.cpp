#include "stdafx.h"
#include "Game.h"

Game::Game(size_t towers, size_t disks) : 
	numberOfTowers(towers), 
	numberOfDisks(disks)
{
	numberOfSteps = 0;
	currentPosition.resize(numberOfTowers);
	positionsOfDisks.resize(numberOfDisks);
	for (size_t i = 0; i < numberOfDisks; ++i) {
		currentPosition[0].push(i);
		positionsOfDisks[i] = std::make_pair(0, currentPosition[0].size() - 1);
	}
}

int Game::getTop(size_t tower) {
	if (currentPosition[tower].empty()) {
		return -1;
	}
	else {
		return currentPosition[tower].top();
	}
}

bool Game::isAvailableDisk(size_t disk) {
	for (auto towers : currentPosition) {
		if (towers.size() > 0) {
			if (towers.top() == disk) {
				return TRUE;
			}
		}
	}
	return FALSE;
}

bool Game::isCorrectStep(GameStep &step) {
	int fromTop = getTop(step.from_);
	if (fromTop < 0) {
		return FALSE;
	}
	int toTop = getTop(step.to_);
	return toTop < fromTop;
}

void Game::doStep(GameStep &step) {
	size_t diskToReplace = currentPosition[step.from_].top();
	currentPosition[step.from_].pop();
	currentPosition[step.to_].push(diskToReplace);
	positionsOfDisks[diskToReplace] = std::make_pair(step.to_, currentPosition[step.to_].size() - 1);
	numberOfSteps += 1;
	previousStep = step;
}

bool Game::isWin() {
	for (size_t i = 1; i < numberOfTowers; ++i) {
		if (currentPosition[i].size() == numberOfDisks) {
			return TRUE;
		}
	}
	return FALSE;
}

std::pair<size_t, size_t> Game::getPositionOfDisk(size_t disk) {
	return positionsOfDisks[disk];
}