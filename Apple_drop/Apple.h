#pragma once
#include"DxLib.h"

//敵の構造体
struct Enemy {
    int flg; //使用フラグ
    int type; //タイプ
    int img; //画像
    int x, y, w, h; //座標x,y,幅w,高さh
    int speed; //移動速度
    int point; //スコア加算
};

//敵機

const int ENEMY_MAX = 10;

class ENEMY {
public:
  
    struct Enemy g_enemy[ENEMY_MAX];
    struct Enemy g_enemy00;

    int g_Mileage; //走行距離
    int applecheck; //リンゴの個数をチェックするための変数
    int applecount; //描画されているリンゴの数を入れる
    double drawapple;  //表示させるリンゴの数
    int appletype;
    int g_EnemyCount1, g_EnemyCount2, g_EnemyCount3; //リンゴを取得した数を入れる変数
    int g_Score; //スコア

    ENEMY();
    void EnemyMove();
    void EnemyDraw();
    void InitEnemy();
    int CreateEnemy();
    void CheckApple(); //描画されているリンゴの個数を数える関数

    
};

extern ENEMY enemy;
