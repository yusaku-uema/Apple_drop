#include<math.h>
#include"Apple.h"
#include"common.h"
#include"Player.h"
#include"BGMandSE.h"
struct Enemy g_enemy[ENEMY_MAX];

ENEMY::ENEMY()
{
    g_enemy00 = { TRUE,0,0,0,-50,APPLE_WIDTH,APPLE_HEIGHT,0,1 };
    g_Mileage = 0;
}

void ENEMY::EnemyMove()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (g_enemy[i].flg == TRUE)
        {

            //まっすぐ下に移動
            g_enemy[i].y += g_enemy[i].speed;// + g_player.speed - PLAYER_SPEED + 1;

            //画面をはみ出したら消去
            if (g_enemy[i].y > SCREEN_HEIGHT + g_enemy[i].h) g_enemy[i].flg = FALSE;

            //当たり判定
            if (player.HitBoxPlayer(&player.g_player, &g_enemy[i]) == TRUE && player.g_player.flg == TRUE)
            {
                g_enemy[i].flg = FALSE;

                applex = g_enemy[i].x;
                appley = g_enemy[i].y;
                if (g_enemy[i].type != 3)PlaySoundMem(bgmse.g_SE6, DX_PLAYTYPE_BACK, TRUE);
                SetFontSize(25);

                if (g_enemy[i].type == 0)
                {
                    g_EnemyCount1++;
                    if (g_drawenemy[i].flg == true)
                    {
                        g_drawenemy[i].x = g_enemy[i].x;
                        g_drawenemy[i].y = g_enemy[i].y;
                        g_drawenemy[i].point = g_enemy[i].point;
                        g_drawenemy[i].flg = g_enemy[i].flg;
                    }

                    g_Score += g_enemy[i].point;
                }
                if (g_enemy[i].type == 1)
                {
                    g_EnemyCount2++;
                    //DrawString(applex, appley, "+300", 0xFFFFFF);
                    if (g_drawenemy[i].flg == true)
                    {
                        g_drawenemy[i].x = g_enemy[i].x;
                        g_drawenemy[i].y = g_enemy[i].y;
                        g_drawenemy[i].point = g_enemy[i].point;
                        g_drawenemy[i].flg = g_enemy[i].flg;
                    }
                    g_Score += g_enemy[i].point;
                }
                if (g_enemy[i].type == 2)
                {
                    g_EnemyCount3++;
                    //DrawString(applex, appley, "+500", 0xFFFFFF);
                    if (g_drawenemy[i].flg == true)
                    {
                        g_drawenemy[i].x = g_enemy[i].x;
                        g_drawenemy[i].y = g_enemy[i].y;
                        g_drawenemy[i].point = g_enemy[i].point;
                        g_drawenemy[i].flg = g_enemy[i].flg;
                    }
                    g_Score += g_enemy[i].point;
                }
                if (g_enemy[i].type == 3)
                {
                    //DrawString(applex, appley, "-1000", 0xFFFFFF);
                    if (g_drawenemy[i].flg == true)
                    {
                        g_drawenemy[i].x = g_enemy[i].x;
                        g_drawenemy[i].y = g_enemy[i].y;
                        g_drawenemy[i].point = g_enemy[i].point;
                        g_drawenemy[i].flg = g_enemy[i].flg;
                    }
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

    DrawApple();

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
        g_drawenemy[i].flg = true;
        g_drawenemy[i].time = 0;
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

void ENEMY::DrawApple()
{
    SetFontSize(30);

    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (g_drawenemy[0].flg == false)
        {
            if (g_drawenemy[0].time <= 150)
            {
                DrawFormatString(g_drawenemy[0].x, g_drawenemy[0].y, 0xffffff, "%d", g_drawenemy[0].point);
               
            }
            g_drawenemy[0].time++;
            if (g_drawenemy[0].time >= 150)
            {
                g_drawenemy[0].time = 0;
                g_drawenemy[0].flg = true;
            }
        }
        if (g_drawenemy[1].flg == false)
        {
            if (g_drawenemy[1].time <= 150)
            {
                DrawFormatString(g_drawenemy[1].x, g_drawenemy[1].y, 0xffffff, "%d", g_drawenemy[1].point);
               
            }
            g_drawenemy[1].time++;
            if (g_drawenemy[1].time >= 150)
            {
                g_drawenemy[1].time = 0;
                g_drawenemy[1].flg = true;
            }
        }
        if (g_drawenemy[2].flg == false)
        {
            if (g_drawenemy[2].time <= 150)
            {
                DrawFormatString(g_drawenemy[2].x, g_drawenemy[2].y, 0xffffff, "%d", g_drawenemy[2].point);
               
            }
            g_drawenemy[2].time++;
            if (g_drawenemy[2].time >= 150)
            {
                g_drawenemy[2].time = 0;
                g_drawenemy[2].flg = true;
            }
        }
        if (g_drawenemy[3].flg == false)
        {
            if (g_drawenemy[3].time <= 150)
            {
                DrawFormatString(g_drawenemy[3].x, g_drawenemy[3].y, 0xffffff, "%d", g_drawenemy[3].point);
           
            }
            g_drawenemy[3].time++;
            if (g_drawenemy[3].time >= 150)
            {
                g_drawenemy[3].time = 0;
                g_drawenemy[3].flg = true;
            }
        }
        if (g_drawenemy[4].flg == false)
        {
            if (g_drawenemy[4].time <= 150)
            {
                DrawFormatString(g_drawenemy[4].x, g_drawenemy[4].y, 0xffffff, "%d", g_drawenemy[4].point);
               
            }
            g_drawenemy[4].time++;
            if (g_drawenemy[4].time >= 150)
            {
                g_drawenemy[4].time = 0;
                g_drawenemy[4].flg = true;
            }
        }
        if (g_drawenemy[5].flg == false)
        {
            if (g_drawenemy[5].time <= 150)
            {
                DrawFormatString(g_drawenemy[5].x, g_drawenemy[5].y, 0xffffff, "%d", g_drawenemy[5].point);
             
            }
            g_drawenemy[5].time++;
            if (g_drawenemy[5].time >= 150)
            {
                g_drawenemy[5].time = 0;
                g_drawenemy[5].flg = true;
            }
        }
        if (g_drawenemy[6].flg == false)
        {
            if (g_drawenemy[6].time <= 150)
            {
                DrawFormatString(g_drawenemy[6].x, g_drawenemy[6].y, 0xffffff, "%d", g_drawenemy[6].point);
        
            }
            g_drawenemy[6].time++;
            if (g_drawenemy[6].time >= 150)
            {
                g_drawenemy[6].time = 0;
                g_drawenemy[6].flg = true;
            }
        }
        if (g_drawenemy[7].flg == false)
        {
            if (g_drawenemy[7].time <= 150)
            {
                DrawFormatString(g_drawenemy[7].x, g_drawenemy[7].y, 0xffffff, "%d", g_drawenemy[7].point);
                
            }
            g_drawenemy[7].time++;
            if (g_drawenemy[7].time >= 150)
            {
                g_drawenemy[7].time = 0;
                g_drawenemy[7].flg = true;
            }
        }
        if (g_drawenemy[8].flg == false)
        {
            if (g_drawenemy[8].time <= 150)
            {
                DrawFormatString(g_drawenemy[8].x, g_drawenemy[8].y, 0xffffff, "%d", g_drawenemy[8].point);
               
            }
            g_drawenemy[8].time++;
            if (g_drawenemy[8].time >= 150)
            {
                g_drawenemy[8].time = 0;
                g_drawenemy[8].flg = true;
            }
        }
        if (g_drawenemy[9].flg == false)
        {
            if (g_drawenemy[9].time <= 150)
            {
                DrawFormatString(g_drawenemy[9].x, g_drawenemy[9].y, 0xffffff, "%d", g_drawenemy[9].point);
                
            }
            g_drawenemy[9].time++;
            if (g_drawenemy[9].time >= 150)
            {
                g_drawenemy[9].time = 0;
                g_drawenemy[9].flg = true;
            }
        }
    }
}