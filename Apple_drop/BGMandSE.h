#pragma once
#include"DxLib.h"

class BGMSE {
public:

	int g_StageBGM; //mainのBGM追加します
	int g_TitleBGM;//タイトルBGM
	int g_RankingBGM;//ランキングBGM
	int g_HelpBGM;//ヘルプBGM
	int g_EndBGM;//エンドBGM
	
	int g_SE1;//選択SE
	int g_SE2;//ポーズ画面選択SE
	int g_SE3;//ポーズ画面からメインに戻るSE
	int g_SE4;//強制終了SE
	int g_SE5;//毒リンゴSE
	int g_SE6;//リンゴSE
	int g_SE7;//ランキング入力選択SE
	int g_SE8;//ランキング削除SE

	int LoadSounds();

};

extern BGMSE bgmse;