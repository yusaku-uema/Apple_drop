#pragma once
#include"DxLib.h"

//敵の構造体
struct ENEMY {
    int flg; //使用フラグ
    int type; //タイプ
    int img; //画像
    int x, y, w, h; //座標x,y,幅w,高さh
    int speed; //移動速度
    int point; //スコア加算
};
//敵機

const int ENEMY_MAX = 10;

class Enemy {
public:
    struct ENEMY g_enemy[ENEMY_MAX];
    struct ENEMY g_enemy00;
    int g_Mileage; //走行距離

    Enemy();
    void EnemyMove();
    void EnemyDraw();
    void InitEnemy();
    int CreateEnemy();
};

extern Enemy enemy;