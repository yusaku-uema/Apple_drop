#include"DxLib.h"
#include"Player.h"
#include"Apple.h"
#include"common.h"



/***********************************************
 * �v���C���[�̈ړ�����
 * ��  ��:�Ȃ�
 * �߂�l:�Ȃ�
 ***********************************************/
void PLAYER::PlayerControl()
{
    //���E�ړ�
    if (g_player.flg == TRUE)
    {
        if (AX < -0)
        {
            if (oldkey == 0 || oldkey == 1 || oldkey == 2)
            {
                PlayerWalkStart(1, -1);
                //PlayerImage();
            }
        }

        if (!(AX < -0))
        {
            if (oldkey == 1 || oldkey == 2)
            {
                PlayerWalkEnd(-1);
            }
        }

        if (AX > 0)
        {
            if (oldkey == 0 || oldkey == 3 || oldkey == 4)
            {
                PlayerWalkStart(3, 1);
            }
        }

        if (!(AX > 0))
        {
            if (oldkey == 3 || oldkey == 4)
            {
                PlayerWalkEnd(1);
            }
        }

        DrawFormatString(0, 50, 0x00ffff, "�X�s�[�h�@�@�@�@ = %2d", g_player.speed);
        DrawFormatString(0, 70, 0x00ffff, "�v���C���[�摜�@ = %2d", image);
        DrawFormatString(0, 33, 0x00ffff, "�����蔻�� = %d", ATARI_HANTEI);
    }

    //��ʂ��͂ݏo���Ȃ��悤�ɂ���
    if (g_player.x < 0)
    {
        g_player.x = 0;
        g_player.speed = 1;
    }
    if (g_player.x > 440)
    {
        g_player.x = 440;
        g_player.speed = 1;
    }

    DrawGraph(g_player.x, g_player.y, g_PlayerImage[image], TRUE);
}

void PLAYER::PlayerWalkEnd(int a)
{
    if (oldkey == 1 || oldkey == 3)
    {
        walkspeed = 0;

        if (oldkey == 1)oldkey = 2;
        if (oldkey == 3)oldkey = 4;
    }

    if (g_player.speed >= 1)
    {
        walkspeed++;
        g_player.x += (g_player.speed * a);

        if (walkspeed >= 5)
        {
            PlayerImage();
            g_player.speed--;
            walkspeed = 0;
        }
    }

    if (g_player.speed <= 1)
    {
        oldkey = 0;
    }
}


void PLAYER::PlayerWalkStart(int a, int b)
{
    if (oldkey != a)
    {
        oldkey = a;
        walkspeed = 0;
        PlayerImage();
        if (oldkey == 0)g_player.speed = 1;
    }

    if (oldkey == a)
    {
        if (walkspeed >= 8)
        {
            PlayerImage();

            if (g_player.speed <= 4)
            {
                g_player.speed++;
            }

            walkspeed = 0;
        }

        g_player.x += (g_player.speed * b);
        walkspeed++;
    }
}


void PLAYER::PlayerImage(void)
{

    if (oldkey == 1 || oldkey == 2)
    {
        if (image >= 8 && image <= 10)
        {
            image++;
        }

        else
        {
            image = 8;
        }
    }

    if (oldkey == 3 || oldkey == 4)
    {
        if (image >= 4 && image <= 6)
        {
            image++;
        }

        else
        {
            image = 4;
        }
    }
}


/***********************************************
 * ���@�ƓG�@�̓����蔻��i�l�p�j
 * ��  ��:PLAYER�|�C���^
 * �߂�l:TRUE������AFALSE�Ȃ�
 ***********************************************/
int PLAYER::HitBoxPlayer(Player* p, Enemy* e)
{

    //x,y�͒��S���W�Ƃ���
    int sx1 = p->x;
    int sy1 = p->y;
    int sx2 = sx1 + p->w;
    int sy2 = sy1 + p->h;

    int dx1 = e->x;
    int dy1 = e->y;
    int dx2 = dx1 + e->w;
    int dy2 = dy1 + e->h;

    //�Z�`���d�Ȃ��Ă���΂�����
    if (sx1 < dx2 && dx1 < sx2 && sy1 < dy2 && dy1 < sy2) {
        return TRUE;
    }
    return FALSE;
}

void PLAYER::PlayerInit(void)
{
    g_player.flg = TRUE;
    g_player.x = PLAYER_POS_X;
    g_player.y = PLAYER_POS_Y;
    g_player.w = PLAYER_WIDTH;
    g_player.h = PLAYER_HEIGHT;

    g_player.count = 0;
    g_player.speed = PLAYER_SPEED;
}

PLAYER::PLAYER()
{
    struct Player g_player = { TRUE,PLAYER_POS_X,PLAYER_POS_Y,PLAYER_WIDTH,PLAYER_HEIGHT,0,PLAYER_SPEED };
    int image = 0;  //�v���C���[�̕����摜��ύX����Ƃ��̕ϐ�
    int walkspeed = 0;
    int oldkey = 0;
    int ATARI_HANTEI = 0;
}