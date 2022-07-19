#include<math.h>
#include"Apple.h"
#include"common.h"
#include"Player.h"
#include"BGMandSE.h"

//struct ENEMY g_enemy[ENEMY_MAX];
struct Enemy g_enemy[ENEMY_MAX];

ENEMY::ENEMY()
{
    g_enemy00 = { TRUE,0,0,0,-50,APPLE_WIDTH,APPLE_HEIGHT,0,1 };
    g_Mileage = 0;
    /*applecheck = 0;
    applecount = 0;
    drawapple = 0;
    g_EnemyCount1 = 0;
    g_EnemyCount2 = 0;
    g_EnemyCount3 = 0;*/
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
               

                if (g_enemy[i].type == 0)
                {
                    PlaySoundMem(bgmse.g_SE6, DX_PLAYTYPE_BACK, TRUE);
                    applex = g_enemy[i].x;
                    appley = g_enemy[i].y;
                    g_EnemyCount1++;
                    SetFontSize(25);
                    DrawString(applex,appley,"+150",0xFFFFFF); 
                    WaitTime(1000);
                    g_Score += g_enemy[i].point;
                }
                if (g_enemy[i].type == 1)
                {
                    PlaySoundMem(bgmse.g_SE6, DX_PLAYTYPE_BACK, TRUE);
                    applex = g_enemy[i].x;
                    appley = g_enemy[i].y;
                    g_EnemyCount2++;
                    SetFontSize(25);
                    DrawString(applex, appley, "+300", 0xFFFFFF);
                    g_Score += g_enemy[i].point;
                }
                if (g_enemy[i].type == 2)
                {
                    PlaySoundMem(bgmse.g_SE6, DX_PLAYTYPE_BACK, TRUE);
                    applex = g_enemy[i].x;
                    appley = g_enemy[i].y;
                    g_EnemyCount3++;
                    SetFontSize(25);
                    DrawString(applex, appley, "+500", 0xFFFFFF);
                    g_Score += g_enemy[i].point;
                }
                if (g_enemy[i].type == 3)
                {
                    applex = g_enemy[i].x;
                    appley = g_enemy[i].y;
                    SetFontSize(25);
                    DrawString(applex, appley, "-1000", 0xFFFFFF);
                    PlaySoundMem(bgmse.g_SE5, DX_PLAYTYPE_BACK, TRUE);
                    g_Score += g_enemy[i].point;
                }
                if (g_Score <= 0) g_Score = 0;


                if (g_enemy[i].type == 3)  //当たったリンゴがタイプ3（毒リンゴ）の時、プレイヤーフラグをFALSEにする
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

                appletype = GetRand(9);
                if (appletype >= 0 && appletype <= 5) g_enemy[i].type = 0;
                if (appletype >= 6 && appletype <= 7) g_enemy[i].type = 1;
                if (appletype == 8) g_enemy[i].type = 2;
                if (appletype == 9) g_enemy[i].type = 3;

                g_enemy[i].img = g_Teki[g_enemy[i].type];
                g_enemy[i].x = GetRand(6) * 70 + 30;


                //リンゴのスピードとポイントを入れる処理
                if (g_enemy[i].type == 0)
                {
                    g_enemy[i].speed = 2;
                    g_enemy[i].point = 150;

                }
                if (g_enemy[i].type == 1)
                {
                    g_enemy[i].speed = 5;
                    g_enemy[i].point = 300;
                }
                if (g_enemy[i].type == 2)
                {
                    g_enemy[i].speed = 10;
                    g_enemy[i].point = 500;
                }
                if (g_enemy[i].type == 3)
                {
                    g_enemy[i].speed = 1;
                    g_enemy[i].point = -1000;
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

void ENEMY::InitEnemy()
{

    //エネミーの初期処理
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        g_enemy[i].flg = FALSE;
    }

    applecheck = 0;
    applecount = 0;
    drawapple = 0;
    g_EnemyCount1 = 0;
    g_EnemyCount2 = 0;
    g_EnemyCount3 = 0;
    g_Score = 0;
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

