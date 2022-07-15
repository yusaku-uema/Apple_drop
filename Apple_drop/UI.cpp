#include"UI.h"
#include"Apple.h"
#include"common.h"

UI::UI() {
    TIMELIMIT = 30000;
}

void UI::TimeCount()
{
    //�������Ԃ��߂�����Q�[���I�[�o�[
    Time = TIMELIMIT - (GetNowCount() - g_StartTime);
    if (Time <= 0)
    {
        if (g_Ranking[RANKING_DATA - 1].score <= enemy.g_Score) {
            StopSoundMem(g_StageBGM); //�Q�[���I�[�o�[�ɒǉ�����
            g_GameState = 6;
        }
        else {
            StopSoundMem(g_StageBGM); //�Q�[���I�[�o�[�ɒǉ�����
            g_GameState = 2;
        }
    }
}

void UI::UIView()
{
    //UI�uTIME�v�\��
    SetFontSize(50);
    DrawString(520, 40, "TIME", 0xffffff, 0);

    //�E��������\��
    SetFontSize(16);
    DrawRotaGraph(523, 220, 0.5f, 0, g_Teki[0], TRUE, FALSE);
    DrawRotaGraph(573, 220, 0.5f, 0, g_Teki[1], TRUE, FALSE);
    DrawRotaGraph(623, 220, 0.5f, 0, g_Teki[2], TRUE, FALSE);
    DrawFormatString(510, 240, 0xFFFFFF, "%03d", enemy.g_EnemyCount1);
    DrawFormatString(560, 240, 0xFFFFFF, "%03d", enemy.g_EnemyCount2);
    DrawFormatString(610, 240, 0xFFFFFF, "%03d", enemy.g_EnemyCount3);

    //UI�uSCORE�v�\��
    SetFontSize(45);
    DrawString(510, 320, "SCORE", 0xFFFFFF, 0);
    SetFontSize(35);
    DrawFormatString(530, 370, 0xFFFFFF, "%05d", enemy.g_Score);

    //���Ԃ̕\��
    SetFontSize(50);
    DrawFormatString(550, 100, 0xffffff, "%2d", Time / 1000);
}

void UI::UIInit() {
    //���݂̌o�ߎ��Ԃ𓾂�
    g_StartTime = GetNowCount();
}

void UI::StopTime() {
    if (g_KeyFlg & PAD_INPUT_2) {
        TIMELIMIT = Time;
        g_StartTime = GetNowCount();
    }
}
