#pragma once
#include"DxLib.h"

//�G�̍\����
struct ENEMY {
    int flg; //�g�p�t���O
    int type; //�^�C�v
    int img; //�摜
    int x, y, w, h; //���Wx,y,��w,����h
    int speed; //�ړ����x
    int point; //�X�R�A���Z
};
//�G�@

const int ENEMY_MAX = 10;

class Enemy {
public:
    struct ENEMY g_enemy[ENEMY_MAX];
    struct ENEMY g_enemy00;
    int g_Mileage; //���s����

    Enemy();
    void EnemyMove();
    void EnemyDraw();
    void InitEnemy();
    int CreateEnemy();
};

extern Enemy enemy;