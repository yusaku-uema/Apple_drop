#include"Ranking.h"
#include"BGMandSE.h"


Ranking::Ranking() {
    int g_fontX = FONT_X;
    int g_fontY = FONT_Y;
    int g_nowfontX = 0;
    int g_nowfontY = 0;
    int fonttime = 0;
    int fontno = 0;
    int red = GetColor(255, 0, 0);
    int white = GetColor(255, 255, 255);
    int color = white;
    int g_HelpImage;
    //ランキングデータの読込み
     g_RankingImage = LoadGraph("images/Chapter5/ranking.png");
     //ランキング入力画面
    g_RankingInputImage = LoadGraph("images/Chapter5/rankingnyuuryoku.png");

}

/***********************************************
 * ゲームランキング表示
 ***********************************************/
void Ranking::DrawRanking(void)
{
    //ランキングサウンド
    PlaySoundMem(bgmse.g_RankingBGM, DX_PLAYTYPE_BACK, FALSE);
    // 音量の設定
    ChangeVolumeSoundMem(255 * 50 / 100, bgmse.g_RankingBGM);
    //スペースキーでメニューに戻る
    if (g_KeyFlg & PAD_INPUT_M) g_GameState = 0;

    //ランキング画像表示
    DrawGraph(0, 0, g_RankingImage, FALSE);

    //ランキング一覧を表示
    SetFontSize(30);
    for (int i = 0; i < RANKING_DATA; i++) {
        DrawFormatString(120, 170 + i * 25, 0xffffff, "%2d %-10s %10d", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
    }
    SetFontSize(30);
    DrawString(50, 450, "----Bボタン押してタイトルに戻る----", 0xffffff, 0);
    //Bボタンでタイトルに戻る
    if (g_KeyFlg & PAD_INPUT_B) g_GameState = 0;
    StopSoundMem(bgmse.g_TitleBGM); //ゲームオーバーに追加する
}

/***********************************************
 * ランキングデータ読込み
 ***********************************************/
int  Ranking::ReadRanking(void)
{
    FILE* fp;
#pragma warning(disable:4996)

    //ファイルオープン
    if ((fp = fopen("dat/rankingdata.txt", "r")) == NULL) {
    //エラー処理
        return -1;
    }

    //ランキングデータ配分列データを読み込む
    for (int i = 0; i < 5; i++) {
        fscanf(fp, "%2d %10s %10d\n", &g_Ranking[i].no, g_Ranking[i].name, &g_Ranking[i].score);
    }

    //ファイルクローズ
    fclose(fp);

    return 0;
}

/***********************************************
 * ランキング並び替え
 ***********************************************/
void Ranking::SortRanking(void)
{
    int i, j;
    RankingData work;

    // 選択法ソート
    for (i = 0; i < 4; i++) {
        for (j = i + 1; j < 5; j++) {
            if (g_Ranking[i].score <= g_Ranking[j].score) {
                work = g_Ranking[i];
                g_Ranking[i] = g_Ranking[j];
                g_Ranking[j] = work;
            }
        }
    }

    //順位付け
    for (i = 0; i < 5; i++) {
        g_Ranking[i].no = 1;
    }
    //得点が同じ場合は、同じ順位とする
    //同順位があった場合の次の順位はデータ個数が加算された順位とする
    for (i = 0; i < 4; i++) {
        for (j = i + 1; j < 5; j++) {
            if (g_Ranking[i].score > g_Ranking[j].score) {
                g_Ranking[j].no++;
            }
        }
    }
}

/***********************************************
 * ランキングデータの保存
 ***********************************************/
int Ranking::SaveRanking(void)
{
    FILE* fp;
#pragma warning(disable:4996)


    if ((fp = fopen("dat/rankingdata.txt", "w")) == NULL) {
        /* エラー処理 */
        return -1;
    }

    //ランキングデータ分配列データを書き込む
    for (int i = 0; i < 5; i++) {
        fprintf(fp, "%2d %10s %10d\n", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
    }

    //ファイルクローズ
    fclose(fp);

    return 0;
}

/***********************************************
 * ランキング入力処理
 ***********************************************/
void Ranking::InputRanking(void)
{
    InputRankingInit();

    // //ランキング画像表示
    DrawGraph(0, 0, g_RankingInputImage, FALSE);

    //フォントサイズの設定
    SetFontSize(20);
    DrawBox(FONT_X - 10, FONT_Y - 5, FONT_X + 450, FONT_Y + 175, 0x000000, TRUE);
    DrawBox(FONT_X - 10, FONT_Y - 5, FONT_X + 450, FONT_Y + 175, white, FALSE);

    if (fonttime >= 7)
    {
        if (AX > 0 || g_KeyFlg & PAD_INPUT_RIGHT)
        {
            if (g_nowfontY == 4 && g_nowfontX >= 10 && g_nowfontX <= 11)g_nowfontX = 12;
            g_nowfontX++;
            if (g_nowfontX > 12) g_nowfontX = 0;
            fonttime = 0;
        }
        else if (AX < 0 || g_KeyFlg & PAD_INPUT_LEFT)
        {
            g_nowfontX--;
            if (g_nowfontY == 4 && g_nowfontX <= 11 && g_nowfontX >= 10)g_nowfontX = 9;
            if (g_nowfontX < 0) g_nowfontX = 12;
            fonttime = 0;
        }
        else if (AY > 0 || g_KeyFlg & PAD_INPUT_DOWN)
        {
            g_nowfontY++;
            if (g_nowfontY > 4) g_nowfontY = 0;
            fonttime = 0;
        }
        else if (AY < 0 || g_KeyFlg & PAD_INPUT_UP)
        {
            g_nowfontY--;
            if (g_nowfontY < 0) g_nowfontY = 4;
            fonttime = 0;
        }
    }

    SetFontSize(30);
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            if (i == g_nowfontY && j == g_nowfontX)
            {
                color = red;
            }
            DrawFormatString(FONT_X + 365, FONT_Y + 140, white, "%s", kettei);

            if (g_nowfontY == 4 && g_nowfontX >= 10 && g_nowfontX <= 12)
            {
                DrawFormatString(FONT_X + 365, FONT_Y + 140, red, "%s", kettei);
            }

            DrawFormatString(g_fontX, g_fontY, color, "%c", g_name[i][j]);
            color = white;
            g_fontX += 35;
        }
        g_fontX = FONT_X;
        g_fontY += 35;
    }
    g_fontY = FONT_Y;


    if (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_A)//決定
    {

        if (fonttime >= 10)
        {
            PlaySoundMem(bgmse.g_SE7, DX_PLAYTYPE_BACK, TRUE);//ランキング入力画面の選択SE
            if (fontno < 9)
            {
                g_Ranking[4].name[fontno] = g_name[g_nowfontY][g_nowfontX];
                fontno++;
            }

            if (g_nowfontY == 4 && g_nowfontX >= 10 && g_nowfontX <= 12)
            {
                g_Ranking[4].score = enemy.g_Score;	// ランキングデータの5番目にスコアを登録
                SortRanking();		// ランキング並べ替え
                SaveRanking();		// ランキングデータの保存
                g_GameState = 2;		// ゲームモードの変更
            }

            fonttime = 0;
        }

    }

    if (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_B)//消去
    {
        if (fonttime >= 10)
        {
            PlaySoundMem(bgmse.g_SE8, DX_PLAYTYPE_BACK, TRUE);//ランキング削除SE
            fontno--;
            g_Ranking[4].name[fontno] = '\0';
            if (fontno < 0)fontno = 0;
            fonttime = 0;
        }

    }

    fonttime++;

    DrawFormatString(210, 165, color, "%s", g_Ranking[4].name);

}

void Ranking::InputRankingInit(void)
{
    for (int i = 0; i < 10; i++)
    {
        g_Ranking[4].name[i] = '\0';
    }
    fontno = 0;
}

int Ranking::getScore(int n) {
    return data[n].score;
}
char* Ranking::getname() {
    return data[4].name;
}