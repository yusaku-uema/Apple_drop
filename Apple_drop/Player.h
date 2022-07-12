#pragma once
#include"Apple.h"
#include"common.h"

//���@�̍\����
struct Player
{
    int flg;       //�g�p�t���O
    int x, y;      //���Wx,y
    int w, h;      //��w, ����h
    int count;     //�^�C�~���O�p
    int speed;     //�ړ����x
};

class PLAYER 
{
public:
    struct Player g_player;

    int image;  //�v���C���[�̕����摜��ύX����Ƃ��̕ϐ�
    int walkspeed;
    int oldkey;
    int blinktime; //�_��
    int invincibletime; //���G����
    int blinkflg;
    int ATARI_HANTEI = 0;

	int g_PlayerImage[16];  //���@�摜 //�L�����摜�ϐ�

    PLAYER();
    void PlayerControl(); //���@����
    void PlayerWalkStart(int a, int b);
    void PlayerWalkEnd(int);
    void PlayerImage(void);

    int HitBoxPlayer(Player* p, Enemy* e);
    void PlayerInit(void);
    void Blink(void);

};


extern PLAYER player;
