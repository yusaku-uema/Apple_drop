#include"DxLib.h"
#include<math.h>
#include"BGMandSE.h"

//サウンド読み込み
int  BGMSE ::LoadSounds() {

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
    //SE5毒リンゴSE
    if ((g_SE5 = LoadSoundMem("sounds/Chapter9/dokuringoSE.wav")) == -1)return -1;
    //SE6りんごabcのSE
    if ((g_SE6 = LoadSoundMem("sounds/Chapter9/ringokakutokuSE.wav")) == -1)return -1;
    //SE7ランキング入力画面選択SE
    if ((g_SE7 = LoadSoundMem("sounds/Chapter9/inputSE.wav")) == -1)return -1;
    //SE8ランキング削除SE
    if ((g_SE8 = LoadSoundMem("sounds/Chapter9/sakujyoSE.wav")) == -1)return -1;
}

