#pragma once
#include"common.h"

//�����L���O�f�[�^�i�\���́j
struct RankingData {
    int no;
    char name[10];
    long score;
};
struct RankingData g_Ranking[RANKING_DATA];

char g_name[5][13] = {
    {'a','b','c','d','e','f','g','h','i','j','k','l','m'},
    {'n','o','p','q','r','s','t','u','v','w','x','y','z'},
    {'A','B','C','D','E','F','G','H','I','J','K','L','M'},
    {'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'},
    {'0','1','2','3','4','5','6','7','8','9',}
};

char kettei[5] = "����";

class Ranking {
private:
    FILE* fp;
    RankingData data[5];
    int g_RankingImage; //�摜�p�ϐ�
    int g_RankingInputImage; //�摜
    int g_fontX;
    int g_fontY;
    int g_nowfontX;
    int g_nowfontY;
    int fonttime;
    int fontno;
    //�J���[�擾
    int red;
    int white;
    int color;
    const int FONT_X = 100;
    const int FONT_Y = 200;
public:
    Ranking();
    void SortRanking(void);		//�����L���O���בւ�
    int  SaveRanking(void);		//�����L���O�f�[�^�̕ۑ�
    int  ReadRanking(void);		//�����L���O�f�[�^�ǂݍ���
    void DrawRanking(void);		//�����L���O�`��
    void InputRanking(void);	//�����L���O����
    void InputRankingInit(void); //������
    int getScore(int n); 
    char* getname(); 

};