#pragma once
#include"DxLib.h"
#include"Apple.h"

//��ʗ̈�̑傫��
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//�����S
const int APPLE_WIDTH = 40; //�{����50
const int APPLE_HEIGHT = 40; //�{����50

extern int g_Teki[4];

//���@�̍\����
struct PLAYER
{
    int flg;       //�g�p�t���O
    int x, y;      //���Wx,y
    int w, h;      //��w, ����h
    //double angle;  //�@�̂̌���
    int count;     //�^�C�~���O�p
    int speed;     //�ړ����x

    int image = 3;  //�v���C���[�̕����摜��ύX����Ƃ��̕ϐ�
    int walkspeed;
    int oldkey;

    int ATARI_HANTEI = 0;
};
//���@
extern struct PLAYER g_player;

int HitBoxPlayer(PLAYER* p, ENEMY* e); //�����蔻��