#pragma once
#include"DxLib.h"

class BGMSE {
public:

	int g_StageBGM; //main��BGM�ǉ����܂�
	int g_TitleBGM;//�^�C�g��BGM
	int g_RankingBGM;//�����L���OBGM
	int g_HelpBGM;//�w���vBGM
	int g_EndBGM;//�G���hBGM
	
	int g_SE1;//�I��SE
	int g_SE2;//�|�[�Y��ʑI��SE
	int g_SE3;//�|�[�Y��ʂ��烁�C���ɖ߂�SE
	int g_SE4;//�����I��SE
	int g_SE5;//�Ń����SSE
	int g_SE6;//�����SSE
	int g_SE7;//�����L���O���͑I��SE
	int g_SE8;//�����L���O�폜SE

	int LoadSounds();

};

extern BGMSE bgmse;