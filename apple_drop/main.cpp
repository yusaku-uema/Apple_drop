/********************************************************************
** ��5�� �~�j�Q�[�������
** �i2)�@���[�X�������Q�[
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
 * �ϐ��̐錾
 ***********************************************/
int g_OldKey;  // �O��̓��̓L�[ 
int g_NowKey;  // ����̓��̓L�[ 
int g_KeyFlg;  // ���̓L�[��� 

int g_GameState = 0;  // �Q�[�����[�h 

int g_TitleImage; // �摜�p�ϐ� 
int g_Menu; //g//_Cone; //���j���[���
//int g_Score = 0; //�X�R�A
int g_RankingImage; //�摜�p�ϐ�
int g_Item[2]; //�A�C�e���摜�ϐ�
int g_WaitTime = 0; //�҂�����
int g_EndImage; //�Q�[���G���h
int g_Mileage; //���s����
int g_MileageB; //�~��
//int g_EnemyCount1, g_EnemyCount2, g_EnemyCount3; //�G�J�E���g
int g_StartTime;   // �X�^�[�g����


int g_Time2; //�X�^�[�g����2 ���݂������g����
int fpscount = 0;  //���݂������g����
int fps = 0;  //���݂������g����


//int Time;   // ���ݎ���

int g_Teki[4]; //�L�����摜�ϐ�

int g_StageImage;
int g_teki;
//int g_PlayerImage[16];  //���@�摜 //�L�����摜�ϐ�

int g_Applec; //�^�C�g���J�[�\���ϐ��@�����Ȃ���

int g_StageBGM; //main��BGM�ǉ����܂�
int g_TitleBGM;//�^�C�g��BGM
int g_RankingBGM;//�����L���OBGM
int g_HelpBGM;//�w���vBGM
int g_EndBGM;//�G���hBGM

int g_SE1;//�I��SE
int g_SE2;//�|�[�Y��ʑI��SE
int g_SE3;//�|�[�Y��ʂ��烁�C���ɖ߂�SE
int g_SE4;
//�ǉ����܂�
int g_ky;

int AX, AY; //�R���g���[�����X�e�B�b�N���W�����Ȃ���
int g_fontX = 100;
int g_fontY = 200;
int g_nowfontX = 0;
int g_nowfontY = 0;

int fonttime = 0;

int fontno = 0;

//�J���[�擾
int Cr;
int red = GetColor(255, 0, 0);
int white = GetColor(255, 255, 255);
int color = white;

int g_HelpImage;

/***********************************************
 * �萔��錾
 ***********************************************/

 //���@�̋@��
const int PLAYER_HP = 1000;
const int PLAYER_FUEL = 20000;
const int PLAYER_BARRIER = 3;
const int PLAYER_BARRIERUP = 10;
//��������
//const int TIMELIMIT = 30000;

//�A�C�e���̍ő吔
const int ITEM_MAX = 3;

//�X�e�b�N
struct DINPUT_JOYSTATE
{
    int		X;		// �X�e�B�b�N�̂w���p�����[�^( -1000�`1000 )
    int		Y;		// �X�e�B�b�N�̂x���p�����[�^( -1000�`1000 )
    int		Z;		// �X�e�B�b�N�̂y���p�����[�^( -1000�`1000 )
    int		Rx;		// �X�e�B�b�N�̂w����]�p�����[�^( -1000�`1000 )
    int		Ry;		// �X�e�B�b�N�̂x����]�p�����[�^( -1000�`1000 )
    int		Rz;		// �X�e�B�b�N�̂y����]�p�����[�^( -1000�`1000 )
    int		Slider[2];	// �X���C�_�[���
    unsigned int	POV[4];	// �n�b�g�X�C�b�`�S��
                    // ( 0xffffffff:���͂Ȃ� 0:�� 4500:�E�� 9000:�E 13500:�E��
                    //		 18000:�� 22500:���� 27000:�� 31500:���� )
    unsigned char	Buttons[32];	// �{�^���R�Q��( �����ꂽ�{�^���� 128 �ɂȂ� )
};

////�����L���O�f�[�^�i�\���́j
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
    {'0','1','2','3','4','5','6','7','8','9','�폜'}
};


/***********************************************
 * �֐��̃v���g�^�C�v�錾
 ***********************************************/
void GameInit(void); //�Q�[����������
void GameMain(void); //�Q�[�����C������

void DrawGameTitle(void); //�^�C�g���`�揈��
void DrawEnd(void); //�Q�[���G���h�`�揈��
void DrawHelp(void); //�Q�[���w���v�`�揈��

void DrawRanking(void); //�����L���O�`�揈��
void InputRanking(void);//�����L���O����
int LoadImages(); //�摜�ǂݍ���
void SortRanking(void); //�����L���O�O���ёւ�
int SaveRanking(void); //�����L���O�f�[�^�̕ۑ�
int ReadRanking(void); //�����L���O�f�[�^�Ǎ���
void BackScrool(); //�w�i�摜�X�N���[������

int LoadSounds(); //�T�E���h�̓ǂݍ��ݏ���

void Pause(); //�|�[�Y���

/***********************************************
 * �v���O�����̊J�n
 ***********************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    //�^�C�g����ݒ�
    SetMainWindowText("Drive&Avoid");
    //�E�B���h�E���[�h�ŋN������ 
    ChangeWindowMode(TRUE);

    // DX ���C�u�����̏���������:�G���[���N�����璼���ɏI�� 
    if (DxLib_Init() == -1) return -1;

    //�`����ʂ𗠂ɂ��� 
    SetDrawScreen(DX_SCREEN_BACK);

    if (ReadRanking() == -1) return -1;//�����L���O�f�[�^�̓Ǎ���


    if (LoadImages() == -1) return -1; //�摜�Ǎ��݊֐����Ăяo��
    if (LoadSounds() == -1) return -1;      //�T�E���h�ǂ݂��݊֐����Ăяo��

      //�Q�[�����[�v 
    while (ProcessMessage() == 0 && g_GameState != 99) {
        //�L�[���͎擾 
        g_OldKey = g_NowKey;
        g_NowKey = GetJoypadInputState(DX_INPUT_PAD1);
        g_KeyFlg = g_NowKey & ~g_OldKey;

        //���E�̃A�i���O���͏�Ԃ��擾����
        GetJoypadAnalogInput(&AX, &AY, DX_INPUT_PAD1);

        //BACK�{�^���ŋ����I��
        if (g_KeyFlg & PAD_INPUT_7) {
            PlaySoundMem(g_SE4, DX_PLAYTYPE_BACK, FALSE);
            g_GameState = 4;
        }

        // ��ʂ̏����� 
        ClearDrawScreen();

        switch (g_GameState) {
        case 0:
            DrawGameTitle(); //�Q�[���^�C�g���`�揈��
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

        //����ʂ̓��e��\��ʂɔ��f���܂� 
        ScreenFlip();
    }
    //DX ���C�u�����g�p�̏I������ 
    DxLib_End();

    //�\�t�g�̏I�� 
    return 0;

}

/***********************************************
 * �Q�[���^�C�g���\��
 ***********************************************/
void DrawGameTitle(void) {

    //Title��BGM�𗬂��B�����Ȃ���
    PlaySoundMem(g_TitleBGM, DX_PLAYTYPE_BACK, FALSE);
    // ���ʂ̐ݒ�
    ChangeVolumeSoundMem(255 * 50 / 100, g_TitleBGM);
    static int MenuNo = 0;

    //���j���[�J�[�\���ړ�����
    if (g_KeyFlg & PAD_INPUT_DOWN) {
        if (++MenuNo > 3)MenuNo = 0;
    }
    if (g_KeyFlg & PAD_INPUT_UP) {
        if (--MenuNo < 0)MenuNo = 3;
    }
    //z�L�[�Ń��j���[�I��
    if (g_KeyFlg & PAD_INPUT_A) {
        PlaySoundMem(g_SE1, DX_PLAYTYPE_BACK, TRUE);
        g_GameState = MenuNo + 1;
    }

    //�^�C�g���摜�\��
    DrawGraph(0, 0, g_TitleImage, FALSE);

    //���j���[
    DrawGraph(310, 220 + MenuNo * 50, g_Applec, TRUE);
    // ���ʂ̐ݒ�
    ChangeVolumeSoundMem(255 * 80 / 100, g_SE1);
    //�����L���OBGM���~�߂�@�����L���O��ʂ���^�C�g���ɖ߂�Ƃ��Ƀ����L���OBGM������Ȃ��悤��
    StopSoundMem(g_RankingBGM);
    //�w���vBGM���~�߂�@�w���v��ʂ���^�C�g���ɖ߂�Ƃ��Ƀw���vBGM������Ȃ��悤��
    StopSoundMem(g_HelpBGM);
    //�G���hBGM���~�߂�@�G���h��ʂ���^�C�g���ɖ߂�Ƃ��ɃG���hBGM������Ȃ��悤��
    StopSoundMem(g_EndBGM);
}

/***********************************************
 * �Q�[����������
 ***********************************************/
void GameInit(void)
{
    //�X�R�A�̏�������
    //g_Score = 0;

    //���s������������

    g_MileageB = 0;

    ////�G�P����������̏����ݒ�
    //g_EnemyCount1 = 0;
    //g_EnemyCount2 = 0;
    //g_EnemyCount3 = 0;

    player.PlayerInit();
    enemy.InitEnemy();

    ////���݂̌o�ߎ��Ԃ𓾂�
    //g_StartTime = GetNowCount();

    g_Time2 = GetNowCount();

    ui.UIInit();

    //�Q�[�����C��������
    g_GameState = 5;
}
/***********************************************
 * �Q�[�������L���O�\��
 ***********************************************/
void DrawRanking(void)
{
    //�����L���O�T�E���h
    PlaySoundMem(g_RankingBGM, DX_PLAYTYPE_BACK, FALSE);
    // ���ʂ̐ݒ�
    ChangeVolumeSoundMem(255 * 50 / 100, g_RankingBGM);
    //�X�y�[�X�L�[�Ń��j���[�ɖ߂�
    if (g_KeyFlg & PAD_INPUT_M) g_GameState = 0;

    //�����L���O�摜�\��
    DrawGraph(0, 0, g_RankingImage, FALSE);

    //�����L���O�ꗗ��\��
    SetFontSize(18);
    for (int i = 0; i < RANKING_DATA; i++) {
        DrawFormatString(50, 170 + i * 25, 0xffffff, "%2d %-10s %10d", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
    }
    DrawString(100, 450, "----B�{�^�������ă^�C�g���ɖ߂� ----", 0xffffff, 0);
    //B�{�^���Ń^�C�g���ɖ߂�
    if (g_KeyFlg & PAD_INPUT_B) g_GameState = 0;
    StopSoundMem(g_TitleBGM); //�Q�[���I�[�o�[�ɒǉ�����
}

/***********************************************
 * �Q�[���w���v�`�揈��
 ***********************************************/
void DrawHelp(void)
{
    //�w���v��BGM
    PlaySoundMem(g_HelpBGM, DX_PLAYTYPE_BACK, FALSE);
    // ���ʂ̐ݒ�
    ChangeVolumeSoundMem(255 * 30 / 100, g_HelpBGM);
    //B�{�^���Ń^�C�g���ɖ߂�
    if (g_KeyFlg & PAD_INPUT_B) g_GameState = 0;
    //A�{�^���ŃQ�[�����C��
    if (g_KeyFlg & PAD_INPUT_A) g_GameState = 1;
    //�^�C�g���摜�\��//
    DrawGraph(0, 0, g_HelpImage, FALSE);
    StopSoundMem(g_TitleBGM); //�Q�[���I�[�o�[�ɒǉ�����
    SetFontSize(30);
    DrawString(100, 110, "�Q�[�������Ă�", 0xFFFFFF);

}

/***********************************************
 * �Q�[���G���h�`�揈��
 ***********************************************/
void DrawEnd(void)
{
    //�G���hBGM
    PlaySoundMem(g_EndBGM, DX_PLAYTYPE_BACK, FALSE);
    // ���ʂ̐ݒ�
    ChangeVolumeSoundMem(255 * 50 / 100, g_EndBGM);
    //�G���h�摜�\��
    DrawGraph(0, 0, g_EndImage, FALSE);

    //�g�p�C���X�g��BGM�ASE�̕`�揈��
    SetFontSize(35);
    DrawString(100, 110, "�g�p�C���X�g", 0xFFFFFF);
    SetFontSize(25);
    DrawString(100, 150, "���炷�Ƃₓ��", 0xFFFFFF);
    DrawString(100, 180, "freepik����", 0xFFFFFF);
    DrawString(100, 210, "illstAC����", 0xFFFFFF);
    DrawString(100, 240, "�p�u���b�N�h���C��Q����", 0xFFFFFF);
    SetFontSize(35);
    DrawString(100, 280, "�g�pBGM��SE", 0xFFFFFF);
    SetFontSize(25);
    DrawString(100, 320, "MUSMUS����", 0xFFFFFF);
    DrawString(100, 350, "�Ò��̉��y���[����", 0xFFFFFF);
    DrawString(100, 380, "�������ʉ��ŗV�ڂ��I����", 0xFFFFFF);
    DrawString(100, 410, "���ʉ����{����", 0xFFFFFF);






    //�^�C���̉��Z�������I���i3�b��j
    if (++g_WaitTime > 180)g_GameState = 99;

    StopSoundMem(g_StageBGM); //�Q�[���I�[�o�[�ɒǉ�����
    StopSoundMem(g_TitleBGM); //�Q�[���I�[�o�[�ɒǉ�����
}
/***********************************************
 * �Q�[�����C��
 ***********************************************/
void GameMain(void)
{
    //main��BGM�𗬂��B�����Ȃ���
    PlaySoundMem(g_StageBGM, DX_PLAYTYPE_BACK, FALSE);
    // ���ʂ̐ݒ�
    ChangeVolumeSoundMem(255 * 50 / 100, g_StageBGM);

    BackScrool();
    player.PlayerControl();

    ui.UIView();
    ui.TimeCount();

    enemy.EnemyDraw();
    enemy.EnemyMove();


    fpscount++;
    SetFontSize(18);
    DrawFormatString(0, 50, 0x00ffff, "fps�@ = %d", fps);
    if ((GetNowCount() - g_Time2) >= 1000)
    {
        fps = fpscount;
        fpscount = 0;
        g_Time2 = GetNowCount();
    }

    StopSoundMem(g_TitleBGM);
    SetFontSize(16);

    //START�{�^���Ń|�[�Y��ʂ�
    if (g_KeyFlg & PAD_INPUT_8) {
        PlaySoundMem(g_SE2, DX_PLAYTYPE_BACK, TRUE);
        g_GameState = 7;
    }
   
    // �|�[�YSE"�̉��ʂ̐ݒ�
    ChangeVolumeSoundMem(255 * 80 / 100, g_SE2);
    // �|�[�YSE"�̉��ʂ̐ݒ�
    ChangeVolumeSoundMem(255 * 80 / 100, g_SE4);
}


//�|�[�Y��ʒǉ�
void Pause(void) {
    BackScrool();
    DrawGraph(player.g_player.x, player.g_player.y, player.g_PlayerImage[player.image], TRUE);
    enemy.EnemyDraw();
    StopSoundMem(g_StageBGM); //�Q�[���I�[�o�[�ɒǉ�����
    ui.UIView();
    if (g_KeyFlg & PAD_INPUT_2) {
        PlaySoundMem(g_SE3, DX_PLAYTYPE_BACK, TRUE);
        ui.StopTime();
        g_GameState = 5;
    }
    SetFontSize(30);
    DrawString(225, 250, "---Pause��---", GetColor(255, 0, 0), 0);
    DrawString(100, 300, "---B�{�^���������ăQ�[����---", GetColor(255, 0, 0), 0);

    // ���ʂ̐ݒ�
    ChangeVolumeSoundMem(255 * 80 / 100, g_SE3);
}

/***********************************************
 * �����L���O���͏���
 ***********************************************/
void InputRanking(void)
{

   // //�����L���O�摜�\��
   DrawGraph(0, 0, g_RankingImage, FALSE);

   //�t�H���g�T�C�Y�̐ݒ�
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


    if (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_A/* && g_OldKey == 0*/)//����
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

    if (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_B /*&& g_OldKey == 0*/)//����
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
        g_Ranking[4].score = enemy.g_Score;	// �����L���O�f�[�^��5�ԖڂɃX�R�A��o�^
        SortRanking();		// �����L���O���בւ�
        SaveRanking();		// �����L���O�f�[�^�̕ۑ�
        g_GameState = 2;		// �Q�[�����[�h�̕ύX
    }
}
/***********************************************
 * �摜�ǂݍ���
 ***********************************************/
int LoadImages()
{
    //�^�C�g�� �^�C�g���摜�ւ��܂����B
    if ((g_TitleImage = LoadGraph("images/Chapter5/Title.png")) == -1) return -1;

    //���j���[
    if ((g_Applec = LoadGraph("images/Chapter5/Applec.png")) == -1) return -1;

    //�����L���O�f�[�^�̓Ǎ���
    if ((g_RankingImage = LoadGraph("images/Chapter5/ranking.png")) == -1)return-1;

    //�G���f�B���O�摜�̓Ǎ���  
    if ((g_EndImage = LoadGraph("images/Chapter5/GameEnd.png")) == -1)return -1;

    //�G
    if (LoadDivGraph("images/Chapter5/apple.png", 4, 4, 1, 50, 50, g_Teki) == -1)return-1;

    //�X�e�[�W�w�i
    if ((g_StageImage = LoadGraph("images/Chapter5/haikei_abcd.png")) == -1)return -1;

    //�w���v���
    if ((g_HelpImage = LoadGraph("images/Chapter5/Help2.png")) == -1)return -1;

    //�v���C���[
    if (LoadDivGraph("images/Chapter5/Player_1.png", 16, 4, 4, 76, 100, player.g_PlayerImage) == -1) return -1; //���@�摜


}
//�T�E���h�ǂݍ���
int LoadSounds() {

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
}
/***********************************************
 * �����L���O���ёւ�
 ***********************************************/
void SortRanking(void)
{
    int i, j;
    RankingData work;

    // �I��@�\�[�g
    for (i = 0; i < 4; i++) {
        for (j = i + 1; j < 5; j++) {
            if (g_Ranking[i].score <= g_Ranking[j].score) {
                work = g_Ranking[i];
                g_Ranking[i] = g_Ranking[j];
                g_Ranking[j] = work;
            }
        }
    }

    //���ʕt��
    for (i = 0; i < 4; i++) {
        g_Ranking[i].no = 1;
    }
    //���_�������ꍇ�́A�������ʂƂ���
    //�����ʂ��������ꍇ�̎��̏��ʂ̓f�[�^�������Z���ꂽ���ʂƂ���
    for (i = 0; i < 4; i++) {
        for (j = i + 1; j < 5; j++) {
            if (g_Ranking[i].score > g_Ranking[j].score) {
                g_Ranking[j].no++;
            }
        }
    }
}
/***********************************************
 * �����L���O�f�[�^�̕ۑ�
 ***********************************************/
int  SaveRanking(void)
{
    FILE* fp;
#pragma warning(disable:4996)

   
    if ((fp = fopen("dat/rankingdata.txt", "w")) == NULL) {
        /* �G���[���� */
        printf("Ranking Data Error\n");
        return -1;
    }

    //�����L���O�f�[�^���z��f�[�^����������
    for (int i = 0; i < 5; i++) {
        fprintf(fp, "%2d %10s %10d\n", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
    }

    //�t�@�C���N���[�Y
    fclose(fp);

    return 0;
}
/***********************************************
 * �����L���O�f�[�^�Ǎ���
 ***********************************************/
int ReadRanking(void)
{
    FILE* fp;
#pragma warning(disable:4996)

    //�t�@�C���I�[�v��
    if ((fp = fopen("dat/rankingdata.txt", "r")) == NULL) {
        //�G���[����
        printf("Ranking Data Error\n");
        return -1;
    }

    //�����L���O�f�[�^�z����f�[�^��ǂݍ���
    for (int i = 0; i < 5; i++) {
        fscanf(fp, "%2d %10s %10d\n", &g_Ranking[i].no, g_Ranking[i].name, &g_Ranking[i].score);//���₵��
    }

    //�t�@�C���N���[�Y
    fclose(fp);

    return 0;
}

/***********************************************
 * �w�i�摜�X�N���[������
 * ��  ��:�Ȃ�
 * �߂�l:�Ȃ�
 ***********************************************/
void BackScrool()
{
    DrawGraph(0, 0, g_StageImage, FALSE);
}
