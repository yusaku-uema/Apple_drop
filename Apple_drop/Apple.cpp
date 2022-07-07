#include"Apple.h"
#include"common.h"

Enemy enemy;
struct ENEMY g_enemy[ENEMY_MAX];

    Enemy::Enemy() {
    struct ENEMY g_enemy00 = { TRUE,0,0,0,-50,APPLE_WIDTH,APPLE_HEIGHT,0,1 };
    g_Mileage = 0;
}

void Enemy::EnemyMove()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (g_enemy[i].flg == TRUE)
        {

            if (g_player.flg == FALSE)continue;

            //�܂��������Ɉړ�
            g_enemy[i].y += g_enemy[i].speed;// + g_player.speed - PLAYER_SPEED + 1;

            //��ʂ��͂ݏo���������
            if (g_enemy[i].y > SCREEN_HEIGHT + g_enemy[i].h) g_enemy[i].flg = FALSE;

            //�����蔻��
            if (HitBoxPlayer(&g_player, &g_enemy[i]) == TRUE)
            {
                g_enemy[i].flg = FALSE;

                g_player.ATARI_HANTEI += 1;
            }
        }

        DrawFormatString(0, 33, 0x00ffff, "�����蔻�� = %d", g_player.ATARI_HANTEI);
    }

    //���s�������ƂɓG�o���p�^�[���𐧌䂷��
    if (g_Mileage / 10 % 50 == 0)
    {
        CreateEnemy();
    }
}

void Enemy::EnemyDraw() {
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (g_enemy[i].flg == TRUE)
        {
            //�G�̕\��
            DrawGraph(g_enemy[i].x, g_enemy[i].y, g_enemy[i].img, TRUE);
        }
    }
}

/***********************************************
 * �G�̐���
 * ��  ��:�Ȃ�
 * �߂�l:TRUE�����AFALSE���s
 ***********************************************/
int Enemy::CreateEnemy()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (g_enemy[i].flg == FALSE)
        {
            g_enemy[i] = g_enemy00;
            g_enemy[i].flg = TRUE;
            g_enemy[i].type = GetRand(3);
            g_enemy[i].img = g_Teki[g_enemy[i].type];
            g_enemy[i].x = GetRand(6) * 70 + 30;

            if (g_enemy[i].type == 0)
            {
                g_enemy[i].speed = 2;
            }
            if (g_enemy[i].type == 1)
            {
                g_enemy[i].speed = 5;
            }
            if (g_enemy[i].type == 2)
            {
                g_enemy[i].speed = 10;
            }
            if (g_enemy[i].type == 3)
            {
                g_enemy[i].speed = 1;
            }

            //����
            return TRUE;
        }
    }

    //���s
    return FALSE;
}

void Enemy::InitEnemy() {

    //�G�l�~�[�̏�������
    for (int i = 0; i < ENEMY_MAX; i++) {
        g_enemy[i].flg = FALSE;
    }
}