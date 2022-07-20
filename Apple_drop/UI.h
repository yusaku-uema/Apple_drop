#pragma once
#include"DxLib.h"
#include"Ranking.h"

class UI {
public:
	int g_StartTime;   // スタート時間
	int Time;   // 現在時間
	//制限時間
	int TIMELIMIT;

	void UIView();
	void TimeCount();
	void UIInit();
	void StopTime();
	void Alpha();
};
extern UI ui;
extern Ranking ranking;