#pragma once
#include"DxLib.h"
#include"Apple.h"
#define RANKING_DATA 5

//画面領域の大きさ
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int PLAYER_POS_X = 225;
const int PLAYER_POS_Y = SCREEN_HEIGHT - 100;
const int PLAYER_WIDTH = 65; //本当は76
const int PLAYER_HEIGHT = 90; //本当は100
const int PLAYER_SPEED = 5;

//リンゴ
const int APPLE_WIDTH = 40; //本当は50
const int APPLE_HEIGHT = 40; //本当は50

extern int g_Teki[4];
extern int AX;
extern int g_KeyFlg;
extern int g_GameState;
extern int g_StageBGM;

//ランキングデータ（構造体）
struct RankingData {
    int no;
    char name[10];
    long score;
};

extern struct RankingData g_Ranking[RANKING_DATA];