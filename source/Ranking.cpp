#include "Ranking.h"
#include <stdlib.h>  




Ranking::Ranking(Renderer &renderer)
	:view_(false), renderer_(renderer), page_(1), lastPage_(1)
{
}


Ranking::~Ranking()
{
}

void Ranking::update()
{
	if (view_)
	{
		int amount = fmin(RECORDS_ON_PAGE*page_, scoreList_.amount);

		int first = RECORDS_ON_PAGE * (page_ - 1);
		for (int i = first; i < amount; ++i)
		{
			renderer_.renderNumber(POINTS_POS_X, POINTS_POS_Y + (i - first) * RECORD_HEIGHT, scoreList_.scores[i].points);
			renderer_.renderTime(TIME_POS_X, TIME_POS_Y + (i - first) * RECORD_HEIGHT, scoreList_.scores[i].time);
		}
	}
	if(lastPage_ > 1)
		renderer_.renderNumber(PAGE_POS_X, PAGE_POS_Y, page_);
}

void Ranking::addScore(Score * score, int size)
{
	getScoreList(size);
	scoreList_.push(score);

	FILE * file;
	char filename[] = "score0";
	filename[5] = '0' + size;
	file = fopen(filename, "wb");
	if (file != NULL)
	{
		for (int i = 0; i < scoreList_.amount; ++i)
		{
			fwrite(&scoreList_.scores[i], sizeof(struct Score), 1, file);
		}
		fclose(file);
	}
}

void Ranking::viewRanking(int boardSize)
{
	view_ = true;
	page_ = 1;
	getScoreList(boardSize);
}

void Ranking::viewMenu()
{
	view_ = false;
}

void Ranking::sortByPoints()
{
	if (view_)
	{
		qsort(scoreList_.scores, scoreList_.amount, sizeof(Score), compareByPoints);
		page_ = 1;
	}
}

void Ranking::sortByTime()
{
	if (view_)
	{
		qsort(scoreList_.scores, scoreList_.amount, sizeof(Score), compareByTime);
		page_ = 1;
	}
}

void Ranking::nextPage()
{
	if (page_ < lastPage_)
		page_++;
}

void Ranking::prevPage()
{
	if(page_ > 1)
		page_--;
}

int Ranking::compareByPoints(const void * score1, const void * score2)
{
	Score *a = (Score*)score1;
	Score *b = (Score*)score2;

	return b->points - a->points;
}

int Ranking::compareByTime(const void * score1, const void * score2)
{
	Score *a = (Score*)score1;
	Score *b = (Score*)score2;

	return a->time - b->time;
}


void Ranking::getScoreList(int size)
{
	FILE * file;
	char filename[] = "score0";
	filename[5] = '0' + size;
	file = fopen(filename, "rb");

	scoreList_.clear();

	Score *newScore = new Score;
	int counter = 0;
	if (file != NULL)
	{
		while (fread(newScore, sizeof(struct Score), 1, file))
		{
			counter++;
			scoreList_.push(newScore);
		}
		fclose(file);
	}
	lastPage_ = (counter + RECORDS_ON_PAGE - 1) / RECORDS_ON_PAGE;
	delete newScore;
}
