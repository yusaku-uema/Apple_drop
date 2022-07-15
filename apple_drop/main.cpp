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

ENEMY enemy;
PLAYER player;
UI ui;
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


int g_Time2; //スタート時間2 かみこうが使うよ
int fpscount = 0;  //かみこうが使うよ
int fps = 0;  //かみこうが使うよ


//int Time;   // 現在時間

int g_Teki[4]; //キャラ画像変数

int g_StageImage;
int g_teki;
//int g_PlayerImage[16];  //自機画像 //キャラ画像変数

int g_Applec; //タイトルカーソル変数　消さないで

int g_StageBGM; //mainのBGM追加します
int g_TitleBGM;//タイトルBGM
int g_RankingBGM;//ランキングBGM
int g_HelpBGM;//ヘルプBGM
int g_EndBGM;//エンドBGM

int g_SE1;//選択SE
int g_SE2;//ポーズ画面選択SE
int g_SE3;//ポーズ画面からメインに戻るSE
int g_SE4;
//追加します
int g_ky;

int AX, AY; //コントローラ左スティック座標消さないで
int g_fontX = 100;
int g_fontY = 200;
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

////ランキングデータ（構造体）
//struct RankingData {
//    int no;
//    char name[10];
//    long score;
//};
struct RankingData g_Ranking[RANKING_DATA];


char name[5][13] = {
    {'a','b','c','d','e','f','g','h','i','j','k','l','m'},
    {'n','o','p','q','r','s','t','u','v','w','x','y','z'},
    {'A','B','C','D','E','F','G','H','I','J','K','L','M'},
    {'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'},
    {'0','1','2','3','4','5','6','7','8','9','削除'}
};


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

int LoadSounds(); //サウンドの読み込み処理

void Pause(); //ポーズ画面

/***********************************************
 * プログラムの開始
 ***********************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    //タイトルを設定
    SetMainWindowText("Drive&Avoid");
    //ウィンドウモードで起動する 
    ChangeWindowMode(TRUE);

    // DX ライブラリの初期化処理:エラーが起きたら直ちに終了 
    if (DxLib_Init() == -1) return -1;

    //描画先画面を裏にする 
    SetDrawScreen(DX_SCREEN_BACK);

    if (ReadRanking() == -1) return -1;//ランキングデータの読込み


    if (LoadImages() == -1) return -1; //画像読込み関数を呼び出し
    if (LoadSounds() == -1) return -1;      //サウンド読みこみ関数を呼び出し

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
            PlaySoundMem(g_SE4, DX_PLAYTYPE_BACK, FALSE);
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
    PlaySoundMem(g_TitleBGM, DX_PLAYTYPE_BACK, FALSE);
    // 音量の設定
    ChangeVolumeSoundMem(255 * 50 / 100, g_TitleBGM);
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
        PlaySoundMem(g_SE1, DX_PLAYTYPE_BACK, TRUE);
        g_GameState = MenuNo + 1;
    }

    //タイトル画像表示
    DrawGraph(0, 0, g_TitleImage, FALSE);

    //メニュー
    DrawGraph(310, 220 + MenuNo * 50, g_Applec, TRUE);
    // 音量の設定
    ChangeVolumeSoundMem(255 * 80 / 100, g_SE1);
    //ランキングBGMを止める　ランキング画面からタイトルに戻るときにランキングBGMが流れないように
    StopSoundMem(g_RankingBGM);
    //ヘルプBGMを止める　ヘルプ画面からタイトルに戻るときにヘルプBGMが流れないように
    StopSoundMem(g_HelpBGM);
    //エンドBGMを止める　エンド画面からタイトルに戻るときにエンドBGMが流れないように
    StopSoundMem(g_EndBGM);
}

/***********************************************
 * ゲーム初期処理
 ***********************************************/
void GameInit(void)
{
    //スコアの初期処理
    //g_Score = 0;

    //走行距離を初期化

    g_MileageB = 0;

    ////敵１を避けた数の初期設定
    //g_EnemyCount1 = 0;
    //g_EnemyCount2 = 0;
    //g_EnemyCount3 = 0;

    player.PlayerInit();
    enemy.InitEnemy();

    ////現在の経過時間を得る
    //g_StartTime = GetNowCount();

    g_Time2 = GetNowCount();

    ui.UIInit();

    //ゲームメイン処理へ
    g_GameState = 5;
}
/***********************************************
 * ゲームランキング表示
 ***********************************************/
void DrawRanking(void)
{
    //ランキングサウンド
    PlaySoundMem(g_RankingBGM, DX_PLAYTYPE_BACK, FALSE);
    // 音量の設定
    ChangeVolumeSoundMem(255 * 50 / 100, g_RankingBGM);
    //スペースキーでメニューに戻る
    if (g_KeyFlg & PAD_INPUT_M) g_GameState = 0;

    //ランキング画像表示
    DrawGraph(0, 0, g_RankingImage, FALSE);

    //ランキング一覧を表示
    SetFontSize(18);
    for (int i = 0; i < RANKING_DATA; i++) {
        DrawFormatString(50, 170 + i * 25, 0xffffff, "%2d %-10s %10d", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
    }
    DrawString(100, 450, "----Bボタン押してタイトルに戻る ----", 0xffffff, 0);
    //Bボタンでタイトルに戻る
    if (g_KeyFlg & PAD_INPUT_B) g_GameState = 0;
    StopSoundMem(g_TitleBGM); //ゲームオーバーに追加する
}

/***********************************************
 * ゲームヘルプ描画処理
 ***********************************************/
void DrawHelp(void)
{
    //ヘルプのBGM
    PlaySoundMem(g_HelpBGM, DX_PLAYTYPE_BACK, FALSE);
    // 音量の設定
    ChangeVolumeSoundMem(255 * 30 / 100, g_HelpBGM);
    //Bボタンでタイトルに戻る
    if (g_KeyFlg & PAD_INPUT_B) g_GameState = 0;
    //Aボタンでゲームメイン
    if (g_KeyFlg & PAD_INPUT_A) g_GameState = 1;
    //タイトル画像表示//
    DrawGraph(0, 0, g_HelpImage, FALSE);
    StopSoundMem(g_TitleBGM); //ゲームオーバーに追加する
    SetFontSize(30);
    DrawString(100, 110, "ゲームをしてね", 0xFFFFFF);

}

/***********************************************
 * ゲームエンド描画処理
 ***********************************************/
void DrawEnd(void)
{
    //エンドBGM
    PlaySoundMem(g_EndBGM, DX_PLAYTYPE_BACK, FALSE);
    // 音量の設定
    ChangeVolumeSoundMem(255 * 50 / 100, g_EndBGM);
    //エンド画像表示
    DrawGraph(0, 0, g_EndImage, FALSE);

    //使用イラストとBGM、SEの描画処理
    SetFontSize(35);
    DrawString(100, 110, "使用イラスト", 0xFFFFFF);
    SetFontSize(25);
    DrawString(100, 150, "いらすとやｓん", 0xFFFFFF);
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

    StopSoundMem(g_StageBGM); //ゲームオーバーに追加する
    StopSoundMem(g_TitleBGM); //ゲームオーバーに追加する
}
/***********************************************
 * ゲームメイン
 ***********************************************/
void GameMain(void)
{
    //mainにBGMを流す。消さないで
    PlaySoundMem(g_StageBGM, DX_PLAYTYPE_BACK, FALSE);
    // 音量の設定
    ChangeVolumeSoundMem(255 * 50 / 100, g_StageBGM);

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

    StopSoundMem(g_TitleBGM);
    SetFontSize(16);

    //STARTボタンでポーズ画面へ
    if (g_KeyFlg & PAD_INPUT_8) {
        PlaySoundMem(g_SE2, DX_PLAYTYPE_BACK, TRUE);
        g_GameState = 7;
    }
   
    // ポーズSE"の音量の設定
    ChangeVolumeSoundMem(255 * 80 / 100, g_SE2);
    // ポーズSE"の音量の設定
    ChangeVolumeSoundMem(255 * 80 / 100, g_SE4);
}


//ポーズ画面追加
void Pause(void) {
    BackScrool();
    DrawGraph(player.g_player.x, player.g_player.y, player.g_PlayerImage[player.image], TRUE);
    enemy.EnemyDraw();
    StopSoundMem(g_StageBGM); //ゲームオーバーに追加する
    ui.UIView();
    if (g_KeyFlg & PAD_INPUT_2) {
        PlaySoundMem(g_SE3, DX_PLAYTYPE_BACK, TRUE);
        ui.StopTime();
        g_GameState = 5;
    }
    SetFontSize(30);
    DrawString(225, 250, "---Pause中---", GetColor(255, 0, 0), 0);
    DrawString(100, 300, "---Bボタンを押してゲームへ---", GetColor(255, 0, 0), 0);

    // 音量の設定
    ChangeVolumeSoundMem(255 * 80 / 100, g_SE3);
}

/***********************************************
 * ランキング入力処理
 ***********************************************/
void InputRanking(void)
{

   // //ランキング画像表示
   DrawGraph(0, 0, g_RankingImage, FALSE);

   //フォントサイズの設定
    SetFontSize(20);
    DrawBox(90, 195, 550, 385, 0x000000, TRUE);
    DrawBox(90, 195, 550, 385, white, FALSE);

    if (fonttime >= 7)
    {
        if (AX > 0)
        {
            g_nowfontX++;
            if (g_nowfontX > 12) g_nowfontX = 0;
            fonttime = 0;
        }
        else if (AX < 0)
        {
            g_nowfontX--;
            if (g_nowfontX < 0) g_nowfontX = 12;
            fonttime = 0;
        }
        else if (AY > 0)
        {
            g_nowfontY++;
            if (g_nowfontY > 4) g_nowfontY = 0;
            fonttime = 0;
        }
        else if (AY < 0)
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
            DrawFormatString(g_fontX, g_fontY, color, "%c", name[i][j]);
            color = white;
            g_fontX += 35;
        }
        g_fontX = 100;
        g_fontY += 35;
    }
    g_fontY = 200;


    if (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_A/* && g_OldKey == 0*/)//決定
    {
        if (fonttime >= 10)
        {
            if (fontno < 9)
            {
                g_Ranking[4].name[fontno] = name[g_nowfontY][g_nowfontX];
                fontno++;
                
            }
            fonttime = 0;
        }
        /*(170, 310, 10, g_Ranking[4].name, FALSE);*/


    }

    if (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_B /*&& g_OldKey == 0*/)//消去
    {

        if (fonttime >= 10)
        {
            fontno--;
            g_Ranking[4].name[fontno] = '\0';
            if (fontno < 0)fontno = 0;
            fonttime = 0;
        }

    }

    fonttime++;

    DrawFormatString(0, 0, color, "%s", g_Ranking[4].name);

    
    if (g_KeyFlg & PAD_INPUT_8) 
    {
        g_Ranking[4].score = enemy.g_Score;	// ランキングデータの5番目にスコアを登録
        SortRanking();		// ランキング並べ替え
        SaveRanking();		// ランキングデータの保存
        g_GameState = 2;		// ゲームモードの変更
    }
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


}
//サウンド読み込み
int LoadSounds() {

    //ステージBGMデータの読み込み
    if ((g_StageBGM = LoadSoundMem("sounds/Chapter9/StageBGM.wav")) == -1)return -1;
    //タイトルBGM
    if ((g_TitleBGM = LoadSoundMem("sounds/Chapter9/TitleBGm.wav")) == -1)return -1;
    //ランキングBGM
    if ((g_RankingBGM = LoadSoundMem("sounds/Chapter9/RankingBGM.wav")) == -1)return -1;
    //ヘルプBGM
    if ((g_HelpBGM = LoadSoundMem("sounds/Chapter9/HelpBGM.wav")) == -1)return -1;
    //エンドBGM
    if ((g_EndBGM = LoadSoundMem("sounds/Chapter9/EndBGM.wav")) == -1)return -1;
    //SE1
    if ((g_SE1 = LoadSoundMem("sounds/Chapter9/sentakuSE.wav")) == -1)return -1;
    //SE2プーズ画面に行くボタン
    if ((g_SE2 = LoadSoundMem("sounds/Chapter9/pausesentakuSE.wav")) == -1)return -1;
    //SE3メインに戻るSE
    if ((g_SE3 = LoadSoundMem("sounds/Chapter9/meinnimodoruSE.wav")) == -1)return -1;
    //SE4強制終了SE
    if ((g_SE4 = LoadSoundMem("sounds/Chapter9/endSE.wav")) == -1)return -1;
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
    for (i = 0; i < 4; i++) {
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
