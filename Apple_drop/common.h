#pragma once
#include"DxLib.h"
#include"Apple.h"

//画面領域の大きさ
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//リンゴ
const int APPLE_WIDTH = 40; //本当は50
const int APPLE_HEIGHT = 40; //本当は50

extern int g_Teki[4];

//自機の構造体
struct PLAYER
{
    int flg;       //使用フラグ
    int x, y;      //座標x,y
    int w, h;      //幅w, 高さh
    //double angle;  //機体の向き
    int count;     //タイミング用
    int speed;     //移動速度

    int image = 3;  //プレイヤーの歩く画像を変更するときの変数
    int walkspeed;
    int oldkey;

    int ATARI_HANTEI = 0;
};
//自機
extern struct PLAYER g_player;

int HitBoxPlayer(PLAYER* p, ENEMY* e); //当たり判定