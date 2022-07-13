#include<math.h>
#include"Apple.h"
#include"common.h"
#include"Player.h"

//struct ENEMY g_enemy[ENEMY_MAX];
struct Enemy g_enemy[ENEMY_MAX];

ENEMY::ENEMY()
{
    g_enemy00 = { TRUE,0,0,0,-50,APPLE_WIDTH,APPLE_HEIGHT,0,1 };
    g_Mileage = 0;
    applecheck = 0;
    applecount = 0;
    drawapple = 0;
}

void ENEMY::EnemyMove()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (g_enemy[i].flg == TRUE)
        {

            //if (player.g_player.flg == FALSE)continue;

            //まっすぐ下に移動
            g_enemy[i].y += g_enemy[i].speed;// + g_player.speed - PLAYER_SPEED + 1;

            //画面をはみ出したら消去
            if (g_enemy[i].y > SCREEN_HEIGHT + g_enemy[i].h) g_enemy[i].flg = FALSE;

            //当たり判定
            if (player.HitBoxPlayer(&player.g_player, &g_enemy[i]) == TRUE && player.g_player.flg == TRUE)
            {
                g_enemy[i].flg = FALSE;

                if (g_enemy[i].type == 3)
                {
                    player.g_player.flg = FALSE;
                }
            }
        }

        
    }

    //走行距離ごとに敵出現パターンを制御する
    /*if (g_Mileage / 10 % 50 == 0)
    {
        CreateEnemy();
    }*/
    applecheck++;

    if (applecheck >= 25)
    {
        CheckApple();
        CreateEnemy();
        applecheck = 0;
    }
}

void ENEMY::EnemyDraw() {
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (g_enemy[i].flg == TRUE)
        {
            //敵の表示
            DrawGraph(g_enemy[i].x, g_enemy[i].y, g_enemy[i].img, TRUE);
        }
    }
}

/***********************************************
 * 敵の生成
 * 引  数:なし
 * 戻り値:TRUE成功、FALSE失敗
 ***********************************************/
int ENEMY::CreateEnemy()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (drawapple > 0)
        {
            if (g_enemy[i].flg == FALSE)
            {
                g_enemy[i] = g_enemy00;
                g_enemy[i].flg = TRUE;

                appletype = GetRand(10);
                if (appletype >= 0 && appletype <= 5) g_enemy[i].type = 0;
                if (appletype >= 6 && appletype <= 7) g_enemy[i].type = 1;
                if (appletype == 8) g_enemy[i].type = 2;
                if (appletype == 9) g_enemy[i].type = 3;

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

                drawapple--;

                //成功
                return TRUE;
            }
        }
    }

    //失敗
    return FALSE;
}

void ENEMY::InitEnemy() {

    //エネミーの初期処理
    for (int i = 0; i < ENEMY_MAX; i++) {
        g_enemy[i].flg = FALSE;
    }
}

void ENEMY::CheckApple()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (g_enemy[i].flg == TRUE)
        {
            applecount++;
        }
    }
    drawapple = ceil(((double)ENEMY_MAX - (double)applecount) / 2);
    applecount = 0;
}

