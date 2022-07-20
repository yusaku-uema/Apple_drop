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
       /* if (AX < -0)
        {
            if (oldkey == 0 || oldkey == 1 || oldkey == 2)
            {
                PlayerWalkStart(1, -1);
            }
        }

        if (!(AX < -0))
        {
            if (oldkey == 1 || oldkey == 2)
            {
                if (AX > 0) PlayerWalkEnd(-1, 1);
                else PlayerWalkEnd(-1, 0);
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
                if (AX < 0)PlayerWalkEnd(1, 1);
                else PlayerWalkEnd(1, 0);
            }
        }*/


    if (AX < 0)
    {
        if (oldkey == 0 || oldkey == 1 || oldkey == 2)
        {
            PlayerWalkStart(1, -1);
        }
        if (oldkey == 3 || oldkey == 4)
       {
            PlayerWalkEnd(1, 1);
        }
    }

    else if (AX > 0)
    {
        if (oldkey == 0 || oldkey == 3 || oldkey == 4)
        {
            PlayerWalkStart(3, 1);
        }
        if (oldkey == 1 || oldkey == 2)
        {
            PlayerWalkEnd(-1, 1);
        }
       
    }

    else if (AX == 0)
    {
        if (oldkey == 3 || oldkey == 4)
        {
            PlayerWalkEnd(1, 0);
        }

        if (oldkey == 1 || oldkey == 2)
        {
            PlayerWalkEnd(-1, 0);
        }
    }

    SetFontSize(18);
    DrawFormatString(0, 10, 0x00ffff, "�X�s�[�h�@�@�@�@ = %2d", g_player.speed);
    DrawFormatString(0, 30, 0x00ffff, "�v���C���[�摜�@ = %2d", image);
    DrawFormatString(0, 70, 0x00ffff, "�v���C���[X�� �@ = %d", g_player.x);


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



    if (g_player.flg == TRUE || blinkflg == FALSE)
    {
        DrawGraph(g_player.x, g_player.y, g_PlayerImage[image], TRUE);
    }

    if (g_player.flg == FALSE) Blink();
}

void PLAYER::PlayerWalkEnd(int a, int b)
{
    if (oldkey == 1 || oldkey == 3)
    {
        walkspeed = 0;
        if (oldkey == 1)
        {
            oldkey = 2;
        }
        if (oldkey == 3)
        {
            oldkey = 4;
        }
    }

    if (b != 0)
    {
        if (oldkey == 2)image = 0;
        if (oldkey == 4)image = 0;
    }

    if (g_player.speed >= 1)
    {
        walkspeed++;
        g_player.x += (g_player.speed * a);

        if (walkspeed >= 5)
        {
            if(b == 0) PlayerImage();
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
    g_player = { TRUE,PLAYER_POS_X,PLAYER_POS_Y,PLAYER_WIDTH,PLAYER_HEIGHT,0,PLAYER_SPEED };
    image = 0;  //�v���C���[�̕����摜��ύX����Ƃ��̕ϐ�
    walkspeed = 0;
    oldkey = 0;
    ATARI_HANTEI = 0;
    blinktime = 0;
    invincibletime = 0;
    blinkflg = TRUE;
}

PLAYER::PLAYER()
{
    g_player = { TRUE,PLAYER_POS_X,PLAYER_POS_Y,PLAYER_WIDTH,PLAYER_HEIGHT,0,PLAYER_SPEED };
    image = 0;  //�v���C���[�̕����摜��ύX����Ƃ��̕ϐ�
    walkspeed = 0;
    oldkey = 0;
    ATARI_HANTEI = 0;
    blinktime = 0;
    invincibletime = 0;
    blinkflg = TRUE;
}

void PLAYER::Blink(void)
{
    if (invincibletime < 120)
    {
        blinktime++;

        if (blinktime >= 20)
        {
            blinktime = 0;
            if (blinkflg == FALSE) blinkflg = TRUE;
            else blinkflg = FALSE;
        }
    }

    invincibletime++;

    if (invincibletime >= 120)
    {
        invincibletime = 0;
        g_player.flg = TRUE;
    }
}