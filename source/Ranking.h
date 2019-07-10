#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "Renderer.h"
#include "Utilities.h"

struct ScoreList
{
	Score *scores = NULL;
	int amount = 0;

	void push(Score *score)
	{
		if (amount >= lenght)
		{
			if (lenght == 0)
			{
				amount = lenght = 1;
				scores = new Score[1];
				scores[0] = *score;
				return;
			}

			lenght *= 2;
			Score *temp = new Score[lenght];
			for (int i = 0; i < amount; ++i)
			{
				temp[i] = scores[i];
			}
			delete[] scores;
			scores = temp;
			scores[amount++] = *score;
		}
		else
		{
			scores[amount++] = *score;
		}
	}

	void clear()
	{
		if(scores) delete[] scores, scores = NULL;;
		amount = lenght = 0;
	}

	~ScoreList()
	{
		if (scores) delete[] scores;
	}

private:
	int lenght = 0;
};

class Ranking
{
public:
	Ranking(Renderer &renderer);
	~Ranking();

	void update();
	void addScore(Score * score, int size);
	void viewRanking(int boardSize);
	void viewMenu();
	void sortByPoints();
	void sortByTime();
	void nextPage();
	void prevPage();

private:
	static const int POINTS_POS_X = 130;
	static const int POINTS_POS_Y = 230;
	static const int TIME_POS_X = 360;
	static const int TIME_POS_Y = 230;
	static const int RECORD_HEIGHT = 35;
	static const int RECORDS_ON_PAGE = 10;
	static const int PAGE_POS_X = 460;
	static const int PAGE_POS_Y = 600;

	static int compareByPoints(const void *score1, const void *score2);
	static int compareByTime(const void *score1, const void *score2);

	Renderer &renderer_;
	ScoreList scoreList_;
	bool view_;
	int page_;
	int lastPage_;

	void getScoreList(int size);
};

