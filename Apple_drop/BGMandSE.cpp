#include"DxLib.h"
#include<math.h>
#include"BGMandSE.h"

//�T�E���h�ǂݍ���
int  BGMSE ::LoadSounds() {

    //�X�e�[�WBGM�f�[�^�̓ǂݍ���
    if ((g_StageBGM = LoadSoundMem("sounds/Chapter9/StageBGM.wav")) == -1)return -1;
    //�^�C�g��BGM
    if ((g_TitleBGM = LoadSoundMem("sounds/Chapter9/TitleBGm.wav")) == -1)return -1;
    //�����L���OBGM
    if ((g_RankingBGM = LoadSoundMem("sounds/Chapter9/RankingBGM.wav")) == -1)return -1;
    //�w���vBGM
    if ((g_HelpBGM = LoadSoundMem("sounds/Chapter9/HelpBGM.wav")) == -1)return -1;
    //�G���hBGM
    if ((g_EndBGM = LoadSoundMem("sounds/Chapter9/EndBGM.wav")) == -1)return -1;
    //SE1
    if ((g_SE1 = LoadSoundMem("sounds/Chapter9/sentakuSE.wav")) == -1)return -1;
    //SE2�v�[�Y��ʂɍs���{�^��
    if ((g_SE2 = LoadSoundMem("sounds/Chapter9/pausesentakuSE.wav")) == -1)return -1;
    //SE3���C���ɖ߂�SE
    if ((g_SE3 = LoadSoundMem("sounds/Chapter9/meinnimodoruSE.wav")) == -1)return -1;
    //SE4�����I��SE
    if ((g_SE4 = LoadSoundMem("sounds/Chapter9/endSE.wav")) == -1)return -1;
    //SE5�Ń����SSE
    if ((g_SE5 = LoadSoundMem("sounds/Chapter9/dokuringoSE.wav")) == -1)return -1;
    //SE6���abc��SE
    if ((g_SE6 = LoadSoundMem("sounds/Chapter9/ringokakutokuSE.wav")) == -1)return -1;
    //SE7�����L���O���͉�ʑI��SE
    if ((g_SE7 = LoadSoundMem("sounds/Chapter9/inputSE.wav")) == -1)return -1;
    //SE8�����L���O�폜SE
    if ((g_SE8 = LoadSoundMem("sounds/Chapter9/sakujyoSE.wav")) == -1)return -1;
}

