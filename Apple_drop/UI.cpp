#include"UI.h"
#include"Apple.h"
#include"common.h"
#include"BGMandSE.h"

void UI::TimeCount()
{
    //制限時間を過ぎたらゲームオーバー
    Time = TIMELIMIT - (GetNowCount() - g_StartTime);
    if (Time <= 0)
    {
        if (g_Ranking[RANKING_DATA - 1].score <= enemy.g_Score) {
            StopSoundMem(bgmse.g_StageBGM); //ゲームオーバーに追加する
            InputRankingInit();
            g_GameState = 6;
        }
        else {
            StopSoundMem(bgmse.g_StageBGM); //ゲームオーバーに追加する
            g_GameState = 2;
        }
    }
}

void UI::UIView()
{
    //UI「TIME」表示
    SetFontSize(50);
    DrawString(520, 40, "TIME", 0xffffff, 0);

    //拾った数を表示
    SetFontSize(20);
    DrawRotaGraph(523, 220, 0.5f, 0, g_Teki[0], TRUE, FALSE);
    DrawRotaGraph(570, 220, 0.5f, 0, g_Teki[1], TRUE, FALSE);
    DrawRotaGraph(620, 220, 0.5f, 0, g_Teki[2], TRUE, FALSE);
    SetFontSize(19);
    DrawFormatString(510, 240, 0xFFFFFF, "%03d", enemy.g_EnemyCount1);
    DrawFormatString(555, 240, 0xFFFFFF, "%03d", enemy.g_EnemyCount2);
    DrawFormatString(605, 240, 0xFFFFFF, "%03d", enemy.g_EnemyCount3);

    //UI「SCORE」表示
    SetFontSize(35);
    DrawString(525, 320, "SCORE", 0xFFFFFF, 0);
    SetFontSize(40);
    DrawFormatString(520, 370, 0xFFFFFF, "%05d", enemy.g_Score);

    //時間の表示
    SetFontSize(50);
    DrawFormatString(550, 100, 0xffffff, "%2d", Time / 1000);
}

void UI::UIInit() {
    //現在の経過時間を得る
    g_StartTime = GetNowCount();
    TIMELIMIT = 31000;
}

void UI::StopTime() {
    if (g_KeyFlg & PAD_INPUT_2) {
        TIMELIMIT = Time;
        g_StartTime = GetNowCount();
    }
}

void UI::Alpha() {
    // 描画ブレンドモードをアルファブレンド（５０％）にする
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
}