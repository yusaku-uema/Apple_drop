#pragma once
#include"DxLib.h"
#include"Ranking.h"

class UI {
public:
	int g_StartTime;   // �X�^�[�g����
	int Time;   // ���ݎ���
	//��������
	int TIMELIMIT;

	void UIView();
	void TimeCount();
	void UIInit();
	void StopTime();
	void Alpha();
};
extern UI ui;
extern Ranking ranking;