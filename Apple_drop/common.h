#pragma once
#include"DxLib.h"
#include"Apple.h"
#define RANKING_DATA 5

//��ʗ̈�̑傫��
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int PLAYER_POS_X = 225;
const int PLAYER_POS_Y = SCREEN_HEIGHT - 100;
const int PLAYER_WIDTH = 65; //�{����76
const int PLAYER_HEIGHT = 90; //�{����100
const int PLAYER_SPEED = 5;

//�����S
const int APPLE_WIDTH = 40; //�{����50
const int APPLE_HEIGHT = 40; //�{����50

extern int g_Teki[4];
extern int AX;
extern int g_KeyFlg;
extern int g_GameState;
extern int g_StageBGM;

//�����L���O�f�[�^�i�\���́j
struct RankingData {
    int no;
    char name[10];
    long score;
};

extern struct RankingData g_Ranking[RANKING_DATA];