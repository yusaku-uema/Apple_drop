#pragma once
#include"DxLib.h"
#include"Apple.h"

//��ʗ̈�̑傫��
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int PLAYER_POS_X = SCREEN_WIDTH / 2;
const int PLAYER_POS_Y = SCREEN_HEIGHT - 100;
const int PLAYER_WIDTH = 65; //�{����76
const int PLAYER_HEIGHT = 90; //�{����100
const int PLAYER_SPEED = 5;

//�����S
const int APPLE_WIDTH = 40; //�{����50
const int APPLE_HEIGHT = 40; //�{����50

extern int g_Teki[4];

//struct Enemy {
//    int flg; //�g�p�t���O
//    int type; //�^�C�v
//    int img; //�摜
//    int x, y, w, h; //���Wx,y,��w,����h
//    int speed; //�ړ����x
//    int point; //�X�R�A���Z
//};

//���@
//extern struct PLAYER g_player;

//int HitBoxPlayer(PLAYER* p, ENEMY* e); //�����蔻��