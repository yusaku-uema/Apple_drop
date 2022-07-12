#pragma once
#include"Apple.h"
#include"common.h"

//自機の構造体
struct Player
{
    int flg;       //使用フラグ
    int x, y;      //座標x,y
    int w, h;      //幅w, 高さh
    int count;     //タイミング用
    int speed;     //移動速度
};

class PLAYER 
{
public:
    struct Player g_player;

    int image;  //プレイヤーの歩く画像を変更するときの変数
    int walkspeed;
    int oldkey;
    int blinktime; //点滅
    int invincibletime; //無敵時間
    int blinkflg;
    int ATARI_HANTEI = 0;

	int g_PlayerImage[16];  //自機画像 //キャラ画像変数

    PLAYER();
    void PlayerControl(); //自機処理
    void PlayerWalkStart(int a, int b);
    void PlayerWalkEnd(int);
    void PlayerImage(void);

    int HitBoxPlayer(Player* p, Enemy* e);
    void PlayerInit(void);
    void Blink(void);

};


extern PLAYER player;
