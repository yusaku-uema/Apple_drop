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
#include"BGMandSE.h"
#include"Ranking.h"

ENEMY enemy;
PLAYER player;
UI ui;
BGMSE bgmse;
Ranking ranking;

/***********************************************
 * �萔��錾
 ***********************************************/

 //���@�̋@��
const int PLAYER_HP = 1000;
const int PLAYER_FUEL = 20000;
const int PLAYER_BARRIER = 3;
const int PLAYER_BARRIERUP = 10;
//�A�C�e���̍ő吔
const int ITEM_MAX = 3;
/***********************************************
 * �ϐ��̐錾
 ***********************************************/
int g_OldKey;  // �O��̓��̓L�[ 
int g_NowKey;  // ����̓��̓L�[ 
int g_KeyFlg;  // ���̓L�[��� 
int g_GameState = 0;  // �Q�[�����[�h 
int g_TitleImage; // �摜�p�ϐ� 
int g_Menu; //���j���[���
int g_Item[2]; //�A�C�e���摜�ϐ�
int g_WaitTime = 0; //�҂�����
int g_EndImage; //�Q�[���G���h
int g_Mileage; //���s����
int g_MileageB; //�~��
int g_StartTime;   // �X�^�[�g����
int g_PauseImage;
int g_Time2; //�X�^�[�g����2 ���݂������g����
int fpscount = 0;  //���݂������g����
int fps = 0;  //���݂������g����
int g_Teki[4]; //�L�����摜�ϐ�
int g_StageImage;
int g_teki;
int g_Applec; //�^�C�g���J�[�\���ϐ��@�����Ȃ���
//�ǉ����܂�
int g_ky;
int AX, AY; //�R���g���[�����X�e�B�b�N���W�����Ȃ���
int g_HelpImage;
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

/***********************************************
 * �֐��̃v���g�^�C�v�錾
 ***********************************************/
void GameInit(void); //�Q�[����������
void GameMain(void); //�Q�[�����C������
void DrawGameTitle(void); //�^�C�g���`�揈��
void DrawEnd(void); //�Q�[���G���h�`�揈��
void DrawHelp(void); //�Q�[���w���v�`�揈��
int LoadImages(); //�摜�ǂݍ���
void BackScrool(); //�w�i�摜�X�N���[������
void Pause(); //�|�[�Y���

/***********************************************
 * �v���O�����̊J�n
 ***********************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    //�^�C�g����ݒ�
    SetMainWindowText("�����S���Ƃ��Q�[��");
    //�E�B���h�E���[�h�ŋN������ 
    ChangeWindowMode(TRUE);

    // DX ���C�u�����̏���������:�G���[���N�����璼���ɏI�� 
    if (DxLib_Init() == -1) return -1;

    //�`����ʂ𗠂ɂ��� 
    SetDrawScreen(DX_SCREEN_BACK);

    if (ranking.ReadRanking() == -1) return -1;//�����L���O�f�[�^�̓Ǎ���

    if (LoadImages() == -1) return -1; //�摜�Ǎ��݊֐����Ăяo��
    if (bgmse.LoadSounds() == -1) return -1; //�T�E���h�ǂ݂��݊֐����Ăяo��

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
            PlaySoundMem(bgmse.g_SE4, DX_PLAYTYPE_BACK, FALSE);
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
            ranking.DrawRanking();
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
            ranking.InputRanking();
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
    PlaySoundMem(bgmse.g_TitleBGM, DX_PLAYTYPE_BACK, FALSE);
    // ���ʂ̐ݒ�
    ChangeVolumeSoundMem(255 * 50 / 100, bgmse.g_TitleBGM);
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
        PlaySoundMem(bgmse.g_SE1, DX_PLAYTYPE_BACK, TRUE);
        g_GameState = MenuNo + 1;
    }

    //�^�C�g���摜�\��
    DrawGraph(0, 0, g_TitleImage, FALSE);

    //���j���[
    DrawGraph(330, 232 + MenuNo * 52, g_Applec, TRUE);
    // ���ʂ̐ݒ�
    ChangeVolumeSoundMem(255 * 80 / 100, bgmse.g_SE1);
    //�����L���OBGM���~�߂�@�����L���O��ʂ���^�C�g���ɖ߂�Ƃ��Ƀ����L���OBGM������Ȃ��悤��
    StopSoundMem(bgmse.g_RankingBGM);
    //�w���vBGM���~�߂�@�w���v��ʂ���^�C�g���ɖ߂�Ƃ��Ƀw���vBGM������Ȃ��悤��
    StopSoundMem(bgmse.g_HelpBGM);
    //�G���hBGM���~�߂�@�G���h��ʂ���^�C�g���ɖ߂�Ƃ��ɃG���hBGM������Ȃ��悤��
    StopSoundMem(bgmse.g_EndBGM);
}

/***********************************************
 * �Q�[����������
 ***********************************************/
void GameInit(void)
{
    g_MileageB = 0;

    player.PlayerInit();
    enemy.InitEnemy();

    g_Time2 = GetNowCount();

    ui.UIInit();
    //�Q�[�����C��������
    g_GameState = 5;
}

/***********************************************
 * �Q�[���w���v�`�揈��
 ***********************************************/
void DrawHelp(void)
{
    //�w���v��BGM
    PlaySoundMem(bgmse.g_HelpBGM, DX_PLAYTYPE_BACK, FALSE);
    // ���ʂ̐ݒ�
    ChangeVolumeSoundMem(255 * 30 / 100, bgmse.g_HelpBGM);
    //B�{�^���Ń^�C�g���ɖ߂�
    if (g_KeyFlg & PAD_INPUT_B) g_GameState = 0;
    //A�{�^���ŃQ�[�����C��
    if (g_KeyFlg & PAD_INPUT_A) g_GameState = 1;
    //�^�C�g���摜�\��//
    DrawGraph(0, 0, g_HelpImage, FALSE);
    StopSoundMem(bgmse.g_TitleBGM); //�Q�[���I�[�o�[�ɒǉ�����
    SetFontSize(30);
    DrawString(160, 110, "L�X�e�B�b�N�c�ړ�����", 0xFFFFFF);
    SetFontSize(30);
    DrawString(160, 160, "A�{�^���c����", 0xFFFFFF);
    SetFontSize(30);
    DrawString(160, 210, "B�{�^���c�߂�", 0xFFFFFF);
    SetFontSize(30);
    DrawString(100, 260, "START�{�^���c�|�[�Y��ʂɈړ�����", 0xFFFFFF);
    SetFontSize(30);
    DrawString(140, 310, "BACK�{�^���c�Q�[�����I������", 0xFFFFFF);
    SetFontSize(30);
    DrawString(20, 390, "---A�{�^�������ăQ�[�����X�^�[�g����---", 0xffffff, 0);
    SetFontSize(30);
    DrawString(300, 420, "OR", 0xffffff, 0);
    SetFontSize(30);
    DrawString(50, 450, "----B�{�^�������ă^�C�g���ɖ߂�----", 0xffffff, 0);

}

/***********************************************
 * �Q�[���G���h�`�揈��
 ***********************************************/
void DrawEnd(void)
{
    //�G���hBGM
    PlaySoundMem(bgmse.g_EndBGM, DX_PLAYTYPE_BACK, FALSE);
    // ���ʂ̐ݒ�
    ChangeVolumeSoundMem(255 * 50 / 100, bgmse.g_EndBGM);
    //�G���h�摜�\��
    DrawGraph(0, 0, g_EndImage, FALSE);

    //�g�p�C���X�g��BGM�ASE�̕`�揈��
    SetFontSize(35);
    DrawString(100, 110, "�g�p�C���X�g", 0xFFFFFF);
    SetFontSize(25);
    DrawString(100, 150, "���炷�Ƃ₳��", 0xFFFFFF);
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

    StopSoundMem(bgmse.g_StageBGM); //�Q�[���I�[�o�[�ɒǉ�����
    StopSoundMem(bgmse.g_TitleBGM); //�Q�[���I�[�o�[�ɒǉ�����
}
/***********************************************
 * �Q�[�����C��
 ***********************************************/
void GameMain(void)
{
    //main��BGM�𗬂��B�����Ȃ���
    PlaySoundMem(bgmse.g_StageBGM, DX_PLAYTYPE_BACK, FALSE);
    // ���ʂ̐ݒ�
    ChangeVolumeSoundMem(255 * 50 / 100, bgmse.g_StageBGM);

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

    StopSoundMem(bgmse.g_TitleBGM);
    SetFontSize(16);

    //START�{�^���Ń|�[�Y��ʂ�
    if (g_KeyFlg & PAD_INPUT_8) {
        PlaySoundMem(bgmse.g_SE2, DX_PLAYTYPE_BACK, TRUE);
        g_GameState = 7;
    }
   
    // �|�[�YSE"�̉��ʂ̐ݒ�
    ChangeVolumeSoundMem(255 * 80 / 100, bgmse.g_SE2);
    // �|�[�YSE"�̉��ʂ̐ݒ�
    ChangeVolumeSoundMem(255 * 80 / 100, bgmse.g_SE4);
}


//�|�[�Y��ʒǉ�
void Pause(void) {
    BackScrool();
    DrawGraph(player.g_player.x, player.g_player.y, player.g_PlayerImage[player.image], TRUE);
    enemy.EnemyDraw();
    StopSoundMem(bgmse.g_StageBGM); //�Q�[���I�[�o�[�ɒǉ�����
    ui.UIView();
    // �`��u�����h���[�h���A���t�@�u�����h�i�T�O���j�ɂ���
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    if (g_KeyFlg & PAD_INPUT_2) {
        // �`��u�����h���[�h���A���t�@�u�����h�i�T�O���j�ɂ���
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
        DrawString(225, 250, "---Pause��---", GetColor(255, 255, 50), 0);
        DrawString(100, 300, "---B�{�^���������ăQ�[����---", GetColor(255, 255, 0), 0);
    }
    // ���ʂ̐ݒ�
    ChangeVolumeSoundMem(255 * 80 / 100, bgmse.g_SE3);
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

    //�|�[�Y���
    if ((g_PauseImage = LoadGraph("images/Chapter5/black_00073.jpg")) == -1)return -1;

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
