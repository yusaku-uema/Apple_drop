/********************************************************************
** 第5章 ミニゲームを作る
** （2)　レース＆避けゲー
********************************************************************/
#include"DxLib.h"
#define _USE_MATH_DEFINES
#include<math.h>
#define RANKING_DATA 5
#include"common.h"

#include"Player.h"
#include"Apple.h"
#include"UI.h"
#include"BGMandSE.h"

ENEMY enemy;
PLAYER player;
UI ui;
BGMSE bgmse;



/***********************************************
 * 定数を宣言
 ***********************************************/

 //自機の機体
const int PLAYER_HP = 1000;
const int PLAYER_FUEL = 20000;
const int PLAYER_BARRIER = 3;
const int PLAYER_BARRIERUP = 10;
//制限時間
//const int TIMELIMIT = 30000;

//アイテムの最大数
const int ITEM_MAX = 3;

const int FONT_X = 100;
const int FONT_Y = 200;


/***********************************************
 * 変数の宣言
 ***********************************************/
int g_OldKey;  // 前回の入力キー 
int g_NowKey;  // 今回の入力キー 
int g_KeyFlg;  // 入力キー情報 

int g_GameState = 0;  // ゲームモード 

int g_TitleImage; // 画像用変数 
int g_Menu; //g//_Cone; //メニュー画面
//int g_Score = 0; //スコア
int g_RankingImage; //画像用変数
int g_Item[2]; //アイテム画像変数
int g_WaitTime = 0; //待ち時間
int g_EndImage; //ゲームエンド
int g_Mileage; //走行距離
int g_MileageB; //止め
//int g_EnemyCount1, g_EnemyCount2, g_EnemyCount3; //敵カウント
int g_StartTime;   // スタート時間
int g_PauseImage;

int g_Time2; //スタート時間2 かみこうが使うよ
int fpscount = 0;  //かみこうが使うよ
int fps = 0;  //かみこうが使うよ


//int Time;   // 現在時間

int g_Teki[4]; //キャラ画像変数

int g_StageImage;
int g_RankingInputImage;//ランキングインプット画面
int g_teki;
//int g_PlayerImage[16];  //自機画像 //キャラ画像変数

int g_Applec; //タイトルカーソル変数　消さないで


//追加します
int g_ky;

int AX, AY; //コントローラ左スティック座標消さないで
int g_fontX = FONT_X;
int g_fontY = FONT_Y;
int g_nowfontX = 0;
int g_nowfontY = 0;

int fonttime = 0;

int fontno = 0;

//カラー取得
int Cr;
int red = GetColor(255, 0, 0);
int white = GetColor(255, 255, 255);
int color = white;

int g_HelpImage;

//const int FONT_X = 100;
//const int FONT_Y = 200;
int Decision = 0;

//int g_WaitTime = 0;
//ステック
struct DINPUT_JOYSTATE
{
    int		X;		// スティックのＸ軸パラメータ( -1000～1000 )
    int		Y;		// スティックのＹ軸パラメータ( -1000～1000 )
    int		Z;		// スティックのＺ軸パラメータ( -1000～1000 )
    int		Rx;		// スティックのＸ軸回転パラメータ( -1000～1000 )
    int		Ry;		// スティックのＹ軸回転パラメータ( -1000～1000 )
    int		Rz;		// スティックのＺ軸回転パラメータ( -1000～1000 )
    int		Slider[2];	// スライダー二つ
    unsigned int	POV[4];	// ハットスイッチ４つ
                    // ( 0xffffffff:入力なし 0:上 4500:右上 9000:右 13500:右下
                    //		 18000:下 22500:左下 27000:左 31500:左上 )
    unsigned char	Buttons[32];	// ボタン３２個( 押されたボタンは 128 になる )
};


struct RankingData g_Ranking[RANKING_DATA];


char g_name[5][13] = {
    {'a','b','c','d','e','f','g','h','i','j','k','l','m'},
    {'n','o','p','q','r','s','t','u','v','w','x','y','z'},
    {'A','B','C','D','E','F','G','H','I','J','K','L','M'},
    {'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'},
    {'0','1','2','3','4','5','6','7','8','9',}
};

char kettei[5] = "決定";


/***********************************************
 * 関数のプロトタイプ宣言
 ***********************************************/
void GameInit(void); //ゲーム初期処理
void GameMain(void); //ゲームメイン処理
void DrawGameTitle(void); //タイトル描画処理
void DrawEnd(void); //ゲームエンド描画処理
void DrawHelp(void); //ゲームヘルプ描画処理
void DrawRanking(void); //ランキング描画処理
void InputRanking(void);//ランキング入力
int LoadImages(); //画像読み込み
void SortRanking(void); //ランキンググ並び替え
int SaveRanking(void); //ランキングデータの保存
int ReadRanking(void); //ランキングデータ読込み

void BackScrool(); //背景画像スクロール処理
//int LoadSounds(); //サウンドの読み込み処理

void Pause(); //ポーズ画面

/***********************************************
 * プログラムの開始
 ***********************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    //タイトルを設定
    SetMainWindowText("リンゴ落としゲーム");
    //ウィンドウモードで起動する 
    ChangeWindowMode(TRUE);

    // DX ライブラリの初期化処理:エラーが起きたら直ちに終了 
    if (DxLib_Init() == -1) return -1;

    //描画先画面を裏にする 
    SetDrawScreen(DX_SCREEN_BACK);

    if (ReadRanking() == -1) return -1;//ランキングデータの読込み


    if (LoadImages() == -1) return -1; //画像読込み関数を呼び出し
    if (bgmse.LoadSounds() == -1) return -1; //サウンド読みこみ関数を呼び出し

      //ゲームループ 
    while (ProcessMessage() == 0 && g_GameState != 99) {
        //キー入力取得 
        g_OldKey = g_NowKey;
        g_NowKey = GetJoypadInputState(DX_INPUT_PAD1);
        g_KeyFlg = g_NowKey & ~g_OldKey;

        //左右のアナログ入力状態を取得する
        GetJoypadAnalogInput(&AX, &AY, DX_INPUT_PAD1);

        //BACKボタンで強制終了
        if (g_KeyFlg & PAD_INPUT_7) {
            PlaySoundMem(bgmse.g_SE4, DX_PLAYTYPE_BACK, FALSE);
            g_GameState = 4;
        }

        // 画面の初期化 
        ClearDrawScreen();

        switch (g_GameState) {
        case 0:
            DrawGameTitle(); //ゲームタイトル描画処理
            break;
        case 1:
            GameInit();
            break;
        case 2:
            DrawRanking();
            break;
        case 3:
            DrawHelp();
            break;
        case 4:
            DrawEnd();
            break;
        case 5:
            GameMain();
            break;
        case 6:
            InputRanking();
            break;
        case 7:
            Pause();
            break;
        }

        //裏画面の内容を表画面に反映します 
        ScreenFlip();
    }
    //DX ライブラリ使用の終了処理 
    DxLib_End();

    //ソフトの終了 
    return 0;

}

/***********************************************
 * ゲームタイトル表示
 ***********************************************/
void DrawGameTitle(void) {

    //TitleにBGMを流す。消さないで
    PlaySoundMem(bgmse.g_TitleBGM, DX_PLAYTYPE_BACK, FALSE);
    // 音量の設定
    ChangeVolumeSoundMem(255 * 50 / 100, bgmse.g_TitleBGM);
    static int MenuNo = 0;

    //メニューカーソル移動処理
    if (g_KeyFlg & PAD_INPUT_DOWN) {
        if (++MenuNo > 3)MenuNo = 0;
    }
    if (g_KeyFlg & PAD_INPUT_UP) {
        if (--MenuNo < 0)MenuNo = 3;
    }
    //zキーでメニュー選択
    if (g_KeyFlg & PAD_INPUT_A) {
        PlaySoundMem(bgmse.g_SE1, DX_PLAYTYPE_BACK, TRUE);
        g_GameState = MenuNo + 1;
    }

    //タイトル画像表示
    DrawGraph(0, 0, g_TitleImage, FALSE);

    //メニュー
    DrawGraph(330, 232 + MenuNo * 52, g_Applec, TRUE);
    // 音量の設定
    ChangeVolumeSoundMem(255 * 80 / 100, bgmse.g_SE1);
    //ランキングBGMを止める　ランキング画面からタイトルに戻るときにランキングBGMが流れないように
    StopSoundMem(bgmse.g_RankingBGM);
    //ヘルプBGMを止める　ヘルプ画面からタイトルに戻るときにヘルプBGMが流れないように
    StopSoundMem(bgmse.g_HelpBGM);
    //エンドBGMを止める　エンド画面からタイトルに戻るときにエンドBGMが流れないように
    StopSoundMem(bgmse.g_EndBGM);
}

/***********************************************
 * ゲーム初期処理
 ***********************************************/
void GameInit(void)
{

    //走行距離を初期化

    g_MileageB = 0;

    player.PlayerInit();
    enemy.InitEnemy();

    g_Time2 = GetNowCount();

    ui.UIInit();
    //InputRankingInit();

    //ゲームメイン処理へ
    g_GameState = 5;
}
/***********************************************
 * ゲームランキング表示
 ***********************************************/
void DrawRanking(void)
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
 * ゲームヘルプ描画処理
 ***********************************************/
void DrawHelp(void)
{
    //ヘルプのBGM
    PlaySoundMem(bgmse.g_HelpBGM, DX_PLAYTYPE_BACK, FALSE);
    // 音量の設定
    ChangeVolumeSoundMem(255 * 30 / 100, bgmse.g_HelpBGM);
    //Bボタンでタイトルに戻る
    if (g_KeyFlg & PAD_INPUT_B) g_GameState = 0;
    //Aボタンでゲームメイン
    if (g_KeyFlg & PAD_INPUT_A) g_GameState = 1;
    //タイトル画像表示//
    DrawGraph(0, 0, g_HelpImage, FALSE);
    StopSoundMem(bgmse.g_TitleBGM); //ゲームオーバーに追加する
    SetFontSize(30);
    DrawString(160, 110, "Lスティック…移動操作", 0xFFFFFF);
    SetFontSize(30);
    DrawString(160, 160, "Aボタン…決定", 0xFFFFFF);
    SetFontSize(30);
    DrawString(160, 210, "Bボタン…戻る", 0xFFFFFF);
    SetFontSize(30);
    DrawString(100, 260, "STARTボタン…ポーズ画面に移動する", 0xFFFFFF);
    SetFontSize(30);
    DrawString(140, 310, "BACKボタン…ゲームを終了する", 0xFFFFFF);

    SetFontSize(30);
    DrawString(20, 390, "---Aボタン押してゲームをスタートする---", 0xffffff, 0);
    SetFontSize(30);
    DrawString(300, 420, "OR", 0xffffff, 0);
    SetFontSize(30);
    DrawString(50, 450, "----Bボタン押してタイトルに戻る----", 0xffffff, 0);
    //Bボタンでタイトルに戻る
}

/***********************************************
 * ゲームエンド描画処理
 ***********************************************/
void DrawEnd(void)
{
    //エンドBGM
    PlaySoundMem(bgmse.g_EndBGM, DX_PLAYTYPE_BACK, FALSE);
    // 音量の設定
    ChangeVolumeSoundMem(255 * 50 / 100, bgmse.g_EndBGM);
    //エンド画像表示
    DrawGraph(0, 0, g_EndImage, FALSE);

    //使用イラストとBGM、SEの描画処理
    SetFontSize(35);
    DrawString(100, 110, "使用イラスト", 0xFFFFFF);
    SetFontSize(25);
    DrawString(100, 150, "いらすとやさん", 0xFFFFFF);
    DrawString(100, 180, "freepikさん", 0xFFFFFF);
    DrawString(100, 210, "illstACさん", 0xFFFFFF);
    DrawString(100, 240, "パブリックドメインQさん", 0xFFFFFF);
    SetFontSize(35);
    DrawString(100, 280, "使用BGMとSE", 0xFFFFFF);
    SetFontSize(25);
    DrawString(100, 320, "MUSMUSさん", 0xFFFFFF);
    DrawString(100, 350, "甘茶の音楽公房さん", 0xFFFFFF);
    DrawString(100, 380, "無料効果音で遊ぼう！さん", 0xFFFFFF);
    DrawString(100, 410, "効果音ラボさん", 0xFFFFFF);






    //タイムの加算処理＆終了（3秒後）
    if (++g_WaitTime > 180)g_GameState = 99;

    StopSoundMem(bgmse.g_StageBGM); //ゲームオーバーに追加する
    StopSoundMem(bgmse.g_TitleBGM); //ゲームオーバーに追加する
}
/***********************************************
 * ゲームメイン
 ***********************************************/
void GameMain(void)
{
    //mainにBGMを流す。消さないで
    PlaySoundMem(bgmse.g_StageBGM, DX_PLAYTYPE_BACK, FALSE);
    // 音量の設定
    ChangeVolumeSoundMem(255 * 50 / 100, bgmse.g_StageBGM);

    BackScrool();
    player.PlayerControl();

    ui.UIView();
    ui.TimeCount();

    enemy.EnemyDraw();
    enemy.EnemyMove();


    fpscount++;
    SetFontSize(18);
    DrawFormatString(0, 50, 0x00ffff, "fps　 = %d", fps);
    if ((GetNowCount() - g_Time2) >= 1000)
    {
        fps = fpscount;
        fpscount = 0;
        g_Time2 = GetNowCount();
    }

    StopSoundMem(bgmse.g_TitleBGM);
    SetFontSize(16);

    //STARTボタンでポーズ画面へ
    if (g_KeyFlg & PAD_INPUT_8) {
        PlaySoundMem(bgmse.g_SE2, DX_PLAYTYPE_BACK, TRUE);
        g_GameState = 7;
    }
   
    // ポーズSE"の音量の設定
    ChangeVolumeSoundMem(255 * 80 / 100, bgmse.g_SE2);
    // ポーズSE"の音量の設定
    ChangeVolumeSoundMem(255 * 80 / 100, bgmse.g_SE4);
}


//ポーズ画面追加
void Pause(void) {
    BackScrool();
    DrawGraph(player.g_player.x, player.g_player.y, player.g_PlayerImage[player.image], TRUE);
    enemy.EnemyDraw();
    StopSoundMem(bgmse.g_StageBGM); //ゲームオーバーに追加する
    ui.UIView();
    // 描画ブレンドモードをアルファブレンド（５０％）にする
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    if (g_KeyFlg & PAD_INPUT_2) {
        // 描画ブレンドモードをアルファブレンド（５０％）にする
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        PlaySoundMem(bgmse.g_SE3, DX_PLAYTYPE_BACK, TRUE);
        ui.StopTime();
        g_GameState = 5;
    }
    if (g_WaitTime > 60) {
        g_WaitTime = 0;
    }
    if (++g_WaitTime < 30) {
        SetFontSize(30);
        DrawString(225, 250, "---Pause中---", GetColor(255, 255, 50), 0);
        DrawString(100, 300, "---Bボタンを押してゲームへ---", GetColor(255, 255, 0), 0);
    }
    // 音量の設定
    ChangeVolumeSoundMem(255 * 80 / 100, bgmse.g_SE3);
}

void InputRankingInit(void)
{
    for (int i = 0; i < 11; i++)
    {
        g_Ranking[4].name[i] = '\0';
    }
    fontno = 0;
}

/***********************************************
 * ランキング入力処理
 ***********************************************/
void InputRanking(void)
{

   // //ランキング画像表示
    DrawGraph( 0, 0, g_RankingInputImage , FALSE);

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
        /*(170, 310, 10, g_Ranking[4].name, FALSE);*/
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
    DrawFormatString(210, 185, color, "%d", fontno);
    
}
/***********************************************
 * 画像読み込み
 ***********************************************/
int LoadImages()
{
    //タイトル タイトル画像替えました。
    if ((g_TitleImage = LoadGraph("images/Chapter5/Title.png")) == -1) return -1;

    //メニュー
    if ((g_Applec = LoadGraph("images/Chapter5/Applec.png")) == -1) return -1;

    //ランキングデータの読込み
    if ((g_RankingImage = LoadGraph("images/Chapter5/ranking.png")) == -1)return-1;

    //エンディング画像の読込み  
    if ((g_EndImage = LoadGraph("images/Chapter5/GameEnd.png")) == -1)return -1;

    //敵
    if (LoadDivGraph("images/Chapter5/apple.png", 4, 4, 1, 50, 50, g_Teki) == -1)return-1;

    //ステージ背景
    if ((g_StageImage = LoadGraph("images/Chapter5/haikei_abcd.png")) == -1)return -1;

    //ヘルプ画面
    if ((g_HelpImage = LoadGraph("images/Chapter5/Help2.png")) == -1)return -1;

    //プレイヤー
    if (LoadDivGraph("images/Chapter5/Player_1.png", 16, 4, 4, 76, 100, player.g_PlayerImage) == -1) return -1; //自機画像

    //ランキング入力画面
    if ((g_RankingInputImage = LoadGraph("images/Chapter5/rankingnyuuryoku.png")) == -1)return -1;

    //ポーズ画面
    if ((g_PauseImage = LoadGraph("images/Chapter5/black_00073.jpg")) == -1)return -1;
}

/***********************************************
 * ランキング並び替え
 ***********************************************/
void SortRanking(void)
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
int  SaveRanking(void)
{
    FILE* fp;
#pragma warning(disable:4996)

   
    if ((fp = fopen("dat/rankingdata.txt", "w")) == NULL) {
        /* エラー処理 */
        printf("Ranking Data Error\n");
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
 * ランキングデータ読込み
 ***********************************************/
int ReadRanking(void)
{
    FILE* fp;
#pragma warning(disable:4996)

    //ファイルオープン
    if ((fp = fopen("dat/rankingdata.txt", "r")) == NULL) {
        //エラー処理
        printf("Ranking Data Error\n");
        return -1;
    }

    //ランキングデータ配分列データを読み込む
    for (int i = 0; i < 5; i++) {
        fscanf(fp, "%2d %10s %10d\n", &g_Ranking[i].no, g_Ranking[i].name, &g_Ranking[i].score);//あやしい
    }

    //ファイルクローズ
    fclose(fp);

    return 0;
}

/***********************************************
 * 背景画像スクロール処理
 * 引  数:なし
 * 戻り値:なし
 ***********************************************/
void BackScrool()
{
    DrawGraph(0, 0, g_StageImage, FALSE);
}
