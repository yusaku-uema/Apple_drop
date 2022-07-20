#pragma once
#include"DxLib.h"

//�G�̍\����
struct Enemy {
    int flg; //�g�p�t���O
    int type; //�^�C�v
    int img; //�摜
    int x, y, w, h; //���Wx,y,��w,����h
    int speed; //�ړ����x
    int point; //�X�R�A���Z
    int time;
};

//�G�@

const int ENEMY_MAX = 10;

class ENEMY {
public:

    struct Enemy g_enemy[ENEMY_MAX];
    struct Enemy g_enemy00;

    struct Enemy g_drawenemy[ENEMY_MAX];

    int g_Mileage; //���s����
    int applecheck; //�����S�̌����`�F�b�N���邽�߂̕ϐ�
    int applecount; //�`�悳��Ă��郊���S�̐�������
    double drawapple;  //�\�������郊���S�̐�
    int appletype;
    int g_EnemyCount1, g_EnemyCount2, g_EnemyCount3; //�����S���擾������������ϐ�
    int g_Score; //�X�R�A
    int applex;
    int appley;

    
    ENEMY();
    void EnemyMove();
    void EnemyDraw();
    void InitEnemy();
    int CreateEnemy();
    void CheckApple(); //�`�悳��Ă��郊���S�̌��𐔂���֐�


    void DrawApple();
    
  
};

extern ENEMY enemy;
extern int WaitTime;