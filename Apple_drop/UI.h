#pragma once
#include"DxLib.h"

class UI {
public:
	int g_StartTime;   // �X�^�[�g����
	int Time;   // ���ݎ���
	//��������
	int TIMELIMIT;

	UI();
	void UIView();
	void TimeCount();
	void UIInit();
	void StopTime();
	void Alpha();
};

extern UI ui;