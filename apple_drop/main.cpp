/********************************************************************
** 第5章 ミニゲームを作る
** （2)　レース＆避けゲー
********************************************************************/
#include"DxLib.h"
#define _USE_MATH_DEFINES
#include<math.h>
#define RANKING_DATA 5
#include"common.h"
/***********************************************
 * 変数の宣言
 ***********************************************/
int g_OldKey;  // 前回の入力キー 
int g_NowKey;  // 今回の入力キー 
int g_KeyFlg;  // 入力キー情報 

int g_GameState = 0;  // ゲームモード 

int g_TitleImage; // 画像用変数 
int g_Menu; //g//_Cone; //メニュー画面
int g_Score = 0; //スコア
int g_RankingImage; //画像用変数
int g_Item[2]; //アイテム画像変数
int g_WaitTime = 0; //待ち時間
int g_EndImage; //ゲームエンド
int g_Mileage; //走行距離
int g_MileageB; //止め
int g_EnemyCount1, g_EnemyCount2, g_EnemyCount3; //敵カウント
int g_StartTime;   // スタート時間
int Time;   // 現在時間

int g_Teki[4]; //キャラ画像変数

int g_StageImage;
int g_teki;
int g_PlayerImage[16];  //自機画像 //キャラ画像変数
int g_Barrier; //バリア画像

int g_Applec; //タイトルカーソル変数　消さないで

int g_StageBGM; //mainのBGM追加します
//追加します
int g_ky;

int AX, AY; //コントローラ左スティック座標消さないで

int g_HelpImag;

/***********************************************
 * 定数を宣言
 ***********************************************/
 
//自機の機体
const int PLAYER_POS_X = SCREEN_WIDTH / 2;
const int PLAYER_POS_Y = SCREEN_HEIGHT - 100;
const int PLAYER_WIDTH = 65; //本当は76
const int PLAYER_HEIGHT = 90; //本当は100
const int PLAYER_SPEED = 5;
const int PLAYER_HP = 1000;
const int PLAYER_FUEL = 20000;
const int PLAYER_BARRIER = 3;
const int PLAYER_BARRIERUP = 10;
//制限時間
const int TIMELIMIT = 30000;

struct PLAYER g_player;

//アイテムの最大数
const int ITEM_MAX = 3;

//ステック
struct DINPUT_JOYSTATE
{
    int		X;		// スティックのＸ軸パラメータ( -1000〜1000 )
    int		Y;		// スティックのＹ軸パラメータ( -1000〜1000 )
    int		Z;		// スティックのＺ軸パラメータ( -1000〜1000 )
    int		Rx;		// スティックのＸ軸回転パラメータ( -1000〜1000 )
    int		Ry;		// スティックのＹ軸回転パラメータ( -1000〜1000 )
    int		Rz;		// スティックのＺ軸回転パラメータ( -1000〜1000 )
    int		Slider[2];	// スライダー二つ
    unsigned int	POV[4];	// ハットスイッチ４つ
                    // ( 0xffffffff:入力なし 0:上 4500:右上 9000:右 13500:右下
                    //		 18000:下 22500:左下 27000:左 31500:左上 )
    unsigned char	Buttons[32];	// ボタン３２個( 押されたボタンは 128 になる )
};

//ランキングデータ（構造体）
struct RankingData {
    int no;
    char name[11];
    long score;
};
struct RankingData g_Ranking[RANKING_DATA];

/***********************************************
 * 関数のプロトタイプ宣言
 ***********************************************/
void GameInit(void); //ゲーム初期処理
void GameMain(void); //ゲームメイン処理

void DrawGameTitle(void); //タイトル描画処理
void DrawGameOver(void); //ゲームオーバー画面描画処理
void DrawEnd(void); //ゲームエンド描画処理
void DrawHelp(void); //ゲームヘルプ描画処理

void DrawRanking(void); //ランキング描画処理
void InputRanking(void);//ランキング入力
int LoadImages(); //画像読み込み
void UIView();
void TimeCount();
void SortRanking(void); //ランキンググ並び替え
int SaveRanking(void); //ランキングデータの保存
int ReadRanking(void); //ランキングデータ読込み
void BackScrool(); //背景画像スクロール処理
void PlayerControl(); //自機処理
int HitBoxPlayer(PLAYER* p, ENEMY* e); //当たり判定

int LoadSounds(); //サウンドの読み込み処理

void PlayerWalkStart(int a, int b);
void PlayerWalkEnd(int);
void PlayerImage(void);
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

    //アイテム画像の読込み
    if ((g_Item[0] = LoadGraph("images/Chapter5/gasoline.bmp")) == -1)return -1;
    if ((g_Item[1] = LoadGraph("images/Chapter5/supana.bmp")) == -1)return -1;

    //ランキングデータの読込み
    if ((g_RankingImage = LoadGraph("images/Chapter5/Ranking.bmp")) == -1)return-1;

    //エンディング画像の読込み  
    if ((g_EndImage = LoadGraph("images/Chapter5/GameEnd.png")) == -1)return -1;

    //敵
    if (LoadDivGraph("images/Chapter5/apple.png", 4, 4, 1, 50, 50, g_Teki) == -1)return-1;

    //ステージ背景
    if ((g_StageImage = LoadGraph("images/Chapter5/haikei_abcd.png")) == -1)return -1;
    if ((g_HelpImag = LoadGraph("images/Chapter5/Help2.png")) == -1)return -1;

    //プレイヤー
    if (LoadDivGraph("images/Chapter5/Player_1.png", 16, 4, 4, 76, 100, g_PlayerImage) == -1) return -1; //自機画像
    //if ((g_Barrier = LoadGraph("images/Chapter5/barrier.png")) == -1)return -1;

    if (LoadImages() == -1) return -1; //画像読込み関数を呼び出し
    if (LoadSounds() == -1) return -1;      //サウンド読みこみ関数を呼び出し

      //ゲームループ 
    while (ProcessMessage() == 0 && g_GameState != 99) {// && !(g_KeyFlg & PAD_INPUT_START)
        //キー入力取得 
        g_OldKey = g_NowKey;
        g_NowKey = GetJoypadInputState(DX_INPUT_PAD1);
        g_KeyFlg = g_NowKey & ~g_OldKey;

        //左右のアナログ入力状態を取得する
        GetJoypadAnalogInput(&AX, &AY, DX_INPUT_PAD1);

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
            DrawGameOver();
            break;
        case 7:
            InputRanking();
        case 8:
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
    static int MenuNo = 0;

    //メニューカーソル移動処理
    if (g_KeyFlg & PAD_INPUT_DOWN) {
        if (++MenuNo > 3)MenuNo = 0;
    }
    if (g_KeyFlg & PAD_INPUT_UP) {
        if (--MenuNo < 0)MenuNo = 3;
    }
    //zキーでメニュー選択
    if (g_KeyFlg & PAD_INPUT_A)g_GameState = MenuNo + 1;

    //タイトル画像表示
    DrawGraph(0, 0, g_TitleImage, FALSE);

    //メニュー
    //DrawGraph(120, 200, g_Menu, TRUE);

      //メニュー
    DrawGraph(310, 220 + MenuNo * 50, g_Applec, TRUE);

    ////メニューカーソル
    //DrawRotaGraph(90, 220 + MenuNo * 40, 0.7f, M_PI / 2, g_Cone, TRUE);
}

/***********************************************
 * ゲーム初期処理
 ***********************************************/
void GameInit(void)
{
    //スコアの初期処理
    g_Score = 0;

    //走行距離を初期化
    
    g_MileageB = 0;

    //敵１を避けた数の初期設定
    g_EnemyCount1 = 0;
    g_EnemyCount2 = 0;
    g_EnemyCount3 = 0;


    //プレイヤーの初期設定
    g_player.flg = TRUE;
    g_player.x = PLAYER_POS_X;
    g_player.y = PLAYER_POS_Y;
    g_player.w = PLAYER_WIDTH;
    g_player.h = PLAYER_HEIGHT;
    //g_player.angle = 0.0;
    g_player.count = 0;
    g_player.speed = PLAYER_SPEED;

    
    enemy.InitEnemy();
   

    //現在の経過時間を得る
    g_StartTime = GetNowCount();

    //ゲームメイン処理へ
    g_GameState = 5;
}
/***********************************************
 * ゲームランキング表示
 ***********************************************/
void DrawRanking(void)
{
    //スペースキーでメニューに戻る
    if (g_KeyFlg & PAD_INPUT_M) g_GameState = 0;

    //ランキング画像表示
    DrawGraph(0, 0, g_RankingImage, FALSE);

    //ランキング一覧を表示
    SetFontSize(18);
    for (int i = 0; i < RANKING_DATA; i++) {
        DrawFormatString(50, 170 + i * 25, 0xffffff, "%2d %-10s %10d", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
    }
    DrawString(100, 450, "----スペースキーを押してタイトルに戻る ----", 0xffffff, 0);
}
/***********************************************
 * ゲームヘルプ描画処理
 ***********************************************/
void DrawHelp(void)
{
    //スペースキーでメニューに戻る
    if (g_KeyFlg & PAD_INPUT_M) g_GameState = 0;



    //タイトル画像表示//
    DrawGraph(0, 0, g_HelpImag, FALSE);


  //SetFontSize(16);
  //  DrawString(20, 120, "ヘルプ画面", 0xffffff, 0);

  //  DrawString(20, 160, "これはリンゴを採るゲームです。", 0xffffff, 0);
  //  DrawString(20, 180, "4つのリンゴが落ちてきますが、毒リンゴを採ると大きな失点になります。", 0xffffff);
  //  DrawString(20, 200, "制限時間内にリンゴをとりハイスコアを目指そう", 0xffffff, 0);
  //  DrawString(20, 220, "ゲーム中の操作", 0xffffff, 0);
  //  DrawString(20, 250, "Aボタンが決定ボタン", 0xffffff, 0);
  //  DrawGraph(20, 260, g_Item[0], TRUE);
  //  DrawString(20, 315, "左スティックがプレイヤー移動取ると燃料が回復するよ。", 0xffffff, 0);
  //  DrawGraph(20, 335, g_Item[1], TRUE);
  //  DrawString(20, 385, "ダメージを受けている時に取ると耐久回復", 0xffffff, 0);
  //  DrawString(20, 405, "耐久が減っていなかったら燃料が少し回復しますよ。", 0xffffff, 0);
  //  DrawString(150, 450, "---- スペースキーを押してタイトルへ戻る ----", 0xffffff, 0);

    //int Pad;        //ジョイパッドの入力状態格納用変数

    //    // while( 裏画面を表画面に反映, メッセージ処理, 画面クリア )
    //// while( 裏画面を表画面に反映, メッセージ処理, 画面クリア )
    //while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
    //    Pad = GetJoypadInputState(DX_INPUT_PAD1);        //入力状態をPadに格納
    //    if (Pad & PAD_INPUT_A) {        //ボタン1の入力フラグが立っていたら
    //        DrawFormatString(0, 0, GetColor(255, 255, 255), "Aです");
    //    }
    //    if (Pad & PAD_INPUT_B) {        //ボタン1の入力フラグが立っていたら
    //        DrawFormatString(0, 0, GetColor(255, 255, 255), "Bです");
    //    }
    //    if (Pad & PAD_INPUT_C) {        //ボタン1の入力フラグが立っていたら
    //        DrawFormatString(0, 0, GetColor(255, 255, 255), "Cです");
    //    }
    //    if (Pad & PAD_INPUT_X) {        //ボタン1の入力フラグが立っていたら
    //        DrawFormatString(0, 0, GetColor(255, 255, 255), "Xです");
    //    }
    //    if (Pad & PAD_INPUT_Y) {        //ボタン1の入力フラグが立っていたら
    //        DrawFormatString(0, 0, GetColor(255, 255, 255), "Yです");
    //    }
    //    if (Pad & PAD_INPUT_Z) {        //ボタン1の入力フラグが立っていたら
    //        DrawFormatString(0, 0, GetColor(255, 255, 255), "Zです");
    //    }
    //    if (Pad & PAD_INPUT_L) {        //ボタン1の入力フラグが立っていたら
    //        DrawFormatString(0, 0, GetColor(255, 255, 255), "Lです");
    //    }
    //    if (Pad & PAD_INPUT_R) {        //ボタン1の入力フラグが立っていたら
    //        DrawFormatString(0, 0, GetColor(255, 255, 255), "Rです");
    //    }
    //    if (Pad & PAD_INPUT_START) {        //ボタン1の入力フラグが立っていたら
    //        DrawFormatString(0, 0, GetColor(255, 255, 255), "STARTです");
    //    }
    //    if (Pad & PAD_INPUT_M) {        //ボタン1の入力フラグが立っていたら
    //        DrawFormatString(0, 0, GetColor(255, 255, 255), "Mです");
    //    }
    //}
}
/***********************************************
 * ゲームエンド描画処理
 ***********************************************/
void DrawEnd(void)
{
    //エンド画像表示
    DrawGraph(0, 0, g_EndImage, FALSE);

   /* SetFontSize(24);
    DrawString(360, 480 - 24, "Thank you for Playing", 0xffffff, 0);*/

    //タイムの加算処理＆終了（3秒後）
    if (++g_WaitTime > 180)g_GameState = 99;
    StopSoundMem(g_StageBGM); //ゲームオーバーに追加する
}
/***********************************************
 * ゲームメイン
 ***********************************************/
void GameMain(void)
{
    //mainにBGMを流す。消さないで
    PlaySoundMem(g_StageBGM, DX_PLAYTYPE_BACK, FALSE);

    BackScrool();
    PlayerControl();

    UIView();
    TimeCount();

    enemy.EnemyDraw();
    enemy.EnemyMove();


    ////スペースキーでメニューに戻る　ゲームメインからタイトルに戻る追加
    //if (g_KeyFlg & PAD_INPUT_M)g_GameState = 6;
    SetFontSize(16);
    //DrawString(150, 450, "---スペースキーを押してゲームオーバーへ---", 0xffffff, 0);

    //STARTボタンでポーズ画面へ
    if (g_KeyFlg & PAD_INPUT_8)g_GameState = 8;
    //BACKボタンで強制終了
    if (g_KeyFlg & PAD_INPUT_7)g_GameState = 4;
}
void Pause(void) {
    BackScrool();
    DrawGraph(g_player.x, g_player.y, g_PlayerImage[g_player.image], TRUE);
    enemy.EnemyDraw();
    StopSoundMem(g_StageBGM);

    if (g_KeyFlg & PAD_INPUT_2)g_GameState = 5;
    SetFontSize(30);
    DrawString(225, 250, "---Pause中---", GetColor(255, 0, 0), 0);
    DrawString(100, 300, "---Bボタンを押してゲームへ---", GetColor(255, 0, 0), 0);
    
}
/***********************************************
 *ゲームオーバー画面描画処理
 ***********************************************/
void DrawGameOver(void)
{

    
    BackScrool();//チャレンジ3
    DrawGraph(g_player.x, g_player.y, g_PlayerImage[g_player.image], TRUE);
    g_Score = (g_MileageB / 10 * 10) + g_EnemyCount3 * 50 + g_EnemyCount2 * 100 + g_EnemyCount1 * 200;


    //スペースキーでメニューに戻る
    if (g_KeyFlg & PAD_INPUT_M) {
        if (g_Ranking[RANKING_DATA - 1].score >= g_Score) {
            g_GameState = 0;
        }
        else {
            g_GameState = 7;
        }
    }

    DrawBox(150, 150, 490, 330, 0x009900, TRUE);
    DrawBox(150, 150, 490, 330, 0x000000, FALSE);

    SetFontSize(20);
    DrawString(220, 170, "ゲームオーバー", 0xcc0000);
    SetFontSize(16);
    DrawString(180, 200, "走行距離   ", 0x000000);
    DrawRotaGraph(230, 230, 0.3f, M_PI / 2, g_Teki[2], TRUE, FALSE);

    DrawRotaGraph(230, 250, 0.3f, M_PI / 2, g_Teki[1], TRUE, FALSE);

    DrawRotaGraph(230, 270, 0.3f, M_PI / 2, g_Teki[0], TRUE, FALSE);

    DrawFormatString(260, 200, 0xFFFFFF, "%6d x 10=%6d", g_MileageB / 10, g_MileageB / 10 * 10);

    DrawFormatString(260, 222, 0xFFFFFF, "%6d x 50=%6d", g_EnemyCount3, g_EnemyCount3 * 50);

    DrawFormatString(260, 243, 0xFFFFFF, "%6dx 100=%6d", g_EnemyCount2, g_EnemyCount2 * 100);

    DrawFormatString(260, 264, 0xFFFFFF, "%6dx 200=%6d", g_EnemyCount1, g_EnemyCount1 * 200);

    DrawString(300, 290, "スコア", 0x000000);

    DrawFormatString(260, 290, 0xFFFFFF, "           =%6d", g_Score);

    DrawString(150, 450, "---スペースキーを押してタイトルへ戻る ---", 0xffffff, 0);

    
    
}
/***********************************************
 * ランキング入力処理
 ***********************************************/
void InputRanking(void)
{
    //ランキング画像表示
    DrawGraph(0, 0, g_RankingImage, FALSE);

    //フォントサイズの設定
    SetFontSize(20);

    //名前入力指示文字列の描画
    DrawString(150, 240, "ランキングに登録します", 0xFFFFFF);
    DrawString(150, 270, "名前を英字で入力してください", 0xFFFFFF);

    //名前の入力
    DrawString(150, 310, "> ", 0xFFFFFF);
    DrawBox(160, 305, 300, 335, 0x000055, TRUE);
    if (KeyInputSingleCharString(170, 310, 10, g_Ranking[4].name, FALSE) == 1) {
        g_Ranking[4].score = g_Score;	// ランキングデータの5番目にスコアを登録
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
    //アイテム画像の読込み
    if ((g_Item[0] = LoadGraph("images/Chapter5/gasoline.bmp")) == -1)return -1;
    if ((g_Item[1] = LoadGraph("images/Chapter5/supana.bmp")) == -1)return -1;

    //ランキングデータの読込end

    if ((g_RankingImage = LoadGraph("images/Chapter5/ranking.png")) == -1)return-1;

    //エンディング画像の読込み 
    if ((g_EndImage = LoadGraph("images/Chapter5/GameEnd.png")) == -1)return -1;

    //敵
    if (LoadDivGraph("images/Chapter5/apple.png", 4, 4, 1, 50, 50, g_Teki) == -1)return-1;

    //ステージ背景
    if ((g_StageImage = LoadGraph("images/Chapter5/haikei_abcd.png")) == -1)return -1;

    //プレイヤー
    if (LoadDivGraph("images/Chapter5/Player_1.png", 16, 4, 4, 76, 100, g_PlayerImage) == -1) return -1; //自機画像
    //if ((g_Barrier = LoadGraph("images/Chapter5/barrier.png")) == -1)return -1;
    return 0;
}
//サウンド読み込み
int LoadSounds() {

    //ステージBGMデータの読み込み
    if ((g_StageBGM = LoadSoundMem("sounds/Chapter9/MusMus-BGM-104.wav")) == -1)return -1;
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

   
    if ((fp = fopen("dat/Chapter5/rankingdata.txt", "w")) == NULL) {
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
    if ((fp = fopen("dat/Chapter5/rankingdata.txt", "r")) == NULL) {
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

/***********************************************
 * プレイヤーの移動処理
 * 引  数:なし
 * 戻り値:なし
 ***********************************************/
void PlayerControl()
{
    //左右移動
    if (g_player.flg == TRUE)
    {
        if (AX < -0)
        {
            if (g_player.oldkey == 0 || g_player.oldkey == 1 || g_player.oldkey == 2)
            {
                PlayerWalkStart(1, -1);
                //PlayerImage();
            }
        }

        if (!(AX < -0))
        {
            if (g_player.oldkey == 1 || g_player.oldkey == 2)
            {
                PlayerWalkEnd(-1);
                //PlayerImage();
            }
        }

        if (AX > 0)
        {
            if (g_player.oldkey == 0 || g_player.oldkey == 3 || g_player.oldkey == 4)
            {
                PlayerWalkStart(3, 1);
                //PlayerImage();
            }
        }

        if (!(AX > 0))
        {
            if (g_player.oldkey == 3 || g_player.oldkey == 4)
            {
                PlayerWalkEnd(1);
                //PlayerImage();
            }
        }

        DrawFormatString(0, 50, 0x00ffff, "スピード　　　　 = %2d", g_player.speed);
        DrawFormatString(0, 70, 0x00ffff, "プレイヤー画像　 = %2d", g_player.image);
        DrawFormatString(0, 33, 0x00ffff, "当たり判定 = %d", g_player.ATARI_HANTEI);
    }

    //画面をはみ出さないようにする
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

    DrawGraph(g_player.x, g_player.y, g_PlayerImage[g_player.image], TRUE);
}

void PlayerWalkEnd(int a)
{
    if (g_player.oldkey == 1 || g_player.oldkey == 3)
    {
        g_player.walkspeed = 0;

        if (g_player.oldkey == 1)g_player.oldkey = 2;
        if (g_player.oldkey == 3)g_player.oldkey = 4;
    }

    if (g_player.speed >= 1)
    {
        g_player.walkspeed++;
        g_player.x += (g_player.speed * a);

        if (g_player.walkspeed >= 5)
        {
            PlayerImage();
            g_player.speed--;
            g_player.walkspeed = 0;
        }
    }

    if (g_player.speed <= 1)
    {
        g_player.oldkey = 0;
    }
}


void PlayerWalkStart(int a, int b)
{
    if (g_player.oldkey != a)
    {
        g_player.oldkey = a;
        g_player.walkspeed = 0;
        PlayerImage();
        if (g_player.oldkey == 0)g_player.speed = 1;
    }

    if (g_player.oldkey == a)
    {
        if (g_player.walkspeed >= 8)
        {
            PlayerImage();

            if (g_player.speed <= 4)
            {
                g_player.speed++;
            }

            g_player.walkspeed = 0;
        }

        g_player.x += (g_player.speed * b);
        g_player.walkspeed++;
    }
}


void PlayerImage(void)
{

    if (g_player.oldkey == 1 || g_player.oldkey == 2)
    {
        if (g_player.image >= 8 && g_player.image <= 10)
        {
            g_player.image++;
        }

        else
        {
            g_player.image = 8;
        }
    }

    if (g_player.oldkey == 3 || g_player.oldkey == 4)
    {
        if (g_player.image >= 4 && g_player.image <= 6)
        {
            g_player.image++;
        }

        else
        {
            g_player.image = 4;
        }
    }
}


  

    

/***********************************************
 * 自機と敵機の当たり判定（四角）
 * 引  数:PLAYERポインタ
 * 戻り値:TRUE当たり、FALSEなし
 ***********************************************/
int HitBoxPlayer(PLAYER* p, ENEMY* e)
{

    //x,yは中心座標とする
    int sx1 = p->x;
    int sy1 = p->y;
    int sx2 = sx1 + p->w;
    int sy2 = sy1 + p->h;

    int dx1 = e->x;
    int dy1 = e->y;
    int dx2 = dx1 + e->w;
    int dy2 = dy1 + e->h;

    //短形が重なっていればあたり
    if (sx1 < dx2 && dx1 < sx2 && sy1 < dy2 && dy1 < sy2) {
        return TRUE;
    }
    return FALSE;
}

void TimeCount(void)
{
    //制限時間を過ぎたらゲームオーバー
    int Time = TIMELIMIT - (GetNowCount() - g_StartTime);
    if (Time <= 0)
    {
        if (g_Ranking[RANKING_DATA - 1].score >= g_Score) {
            StopSoundMem(g_StageBGM); //ゲームオーバーに追加する
            g_GameState = 2;
        }
        else {
            StopSoundMem(g_StageBGM); //ゲームオーバーに追加する
            g_GameState = 7;
        }
    }
    SetFontSize(50);
    DrawFormatString(550, 100, 0xffffff, "%2d", Time / 1000);
}

void UIView(void)
{
    //UI「TIME」表示
    SetFontSize(50);
    DrawString(520, 40, "TIME", 0xffffff, 0);

    //拾った数を表示
    SetFontSize(16);
    DrawRotaGraph(523, 220, 0.5f, 0, g_Teki[0], TRUE, FALSE);
    DrawRotaGraph(573, 220, 0.5f, 0, g_Teki[1], TRUE, FALSE);
    DrawRotaGraph(623, 220, 0.5f, 0, g_Teki[2], TRUE, FALSE);
    DrawFormatString(510, 240, 0xFFFFFF, "%03d", g_EnemyCount1);
    DrawFormatString(560, 240, 0xFFFFFF, "%03d", g_EnemyCount2);
    DrawFormatString(610, 240, 0xFFFFFF, "%03d", g_EnemyCount3);

    //UI「SCORE」表示
    SetFontSize(45);
    DrawString(510, 320, "SCORE", 0xffffff, 0);
}