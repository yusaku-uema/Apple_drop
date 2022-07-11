#pragma once
#include"DxLib.h"
#include"Apple.h"

//画面領域の大きさ
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int PLAYER_POS_X = SCREEN_WIDTH / 2;
const int PLAYER_POS_Y = SCREEN_HEIGHT - 100;
const int PLAYER_WIDTH = 65; //本当は76
const int PLAYER_HEIGHT = 90; //本当は100
const int PLAYER_SPEED = 5;

//リンゴ
const int APPLE_WIDTH = 40; //本当は50
const int APPLE_HEIGHT = 40; //本当は50

extern int g_Teki[4];

//struct Enemy {
//    int flg; //使用フラグ
//    int type; //タイプ
//    int img; //画像
//    int x, y, w, h; //座標x,y,幅w,高さh
//    int speed; //移動速度
//    int point; //スコア加算
//};

//自機
//extern struct PLAYER g_player;

//int HitBoxPlayer(PLAYER* p, ENEMY* e); //当たり判定