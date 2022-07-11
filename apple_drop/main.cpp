/********************************************************************
** ��5�� �~�j�Q�[�������
** �i2)�@���[�X�������Q�[
********************************************************************/
#include "DxLib.h" 
#define _USE_MATH_DEFINES
#include<math.h>
#define RANKING_DATA 5
/***********************************************
 * �ϐ��̐錾
 ***********************************************/
int g_OldKey;  // �O��̓��̓L�[ 
int g_NowKey;  // ����̓��̓L�[ 
int g_KeyFlg;  // ���̓L�[��� 

int g_GameState = 0;  // �Q�[�����[�h 

int g_TitleImage; // �摜�p�ϐ� 
int g_Menu; //g//_Cone; //���j���[���
int g_Score = 0; //�X�R�A
int g_RankingImage; //�摜�p�ϐ�
int g_Item[2]; //�A�C�e���摜�ϐ�
int g_WaitTime = 0; //�҂�����
int g_EndImage; //�Q�[���G���h
int g_Mileage; //���s����
int g_MileageB; //�~��
int g_EnemyCount1, g_EnemyCount2, g_EnemyCount3; //�G�J�E���g
int g_StartTime;   // �X�^�[�g����
int Time;   // ���ݎ���

int g_Teki[4]; //�L�����摜�ϐ�

int g_StageImage;
int g_teki;
int g_PlayerImage[16];  //���@�摜 //�L�����摜�ϐ�
int g_Barrier; //�o���A�摜

int g_Applec; //�^�C�g���J�[�\���ϐ��@�����Ȃ���

int g_StageBGM; //main��BGM�ǉ����܂�
//�ǉ����܂�
int g_ky;

/***********************************************
 * �萔��錾
 ***********************************************/
 //��ʗ̈�̑傫��
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
//���@�̋@��
const int PLAYER_POS_X = SCREEN_WIDTH / 2;
const int PLAYER_POS_Y = SCREEN_HEIGHT - 100;
const int PLAYER_WIDTH = 65; //�{����76
const int PLAYER_HEIGHT = 90; //�{����100
const int PLAYER_SPEED = 5;
const int PLAYER_HP = 1000;
const int PLAYER_FUEL = 20000;
const int PLAYER_BARRIER = 3;
const int PLAYER_BARRIERUP = 10;
//��������
const int TIMELIMIT = 30000;

//�����S
const int APPLE_WIDTH = 40; //�{����50
const int APPLE_HEIGHT = 40; //�{����50

//�G�@�̍ő吔
const int ENEMY_MAX = 10;//�`�������W1�ύX 20
//�A�C�e���̍ő吔
const int ITEM_MAX = 3;

//�X�e�b�N�H
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

//�����L���O�f�[�^�i�\���́j
struct RankingData {
    int no;
    char name[11];
    long score;
};
struct RankingData g_Ranking[RANKING_DATA];

//���@�̍\����
struct PLAYER
{
    int flg;       //�g�p�t���O
    int x, y;      //���Wx,y
    int w, h;      //��w, ����h
    //double angle;  //�@�̂̌���
    int count;     //�^�C�~���O�p
    int speed;     //�ړ����x

    int image = 3;  //�v���C���[�̕����摜��ύX����Ƃ��̕ϐ�
    int walkspeed;
    int oldkey;

    int ATARI_HANTEI = 0;
};
//���@
struct PLAYER g_player;


//�G�̍\����
struct ENEMY {
    int flg; //�g�p�t���O
    int type; //�^�C�v
    int img; //�摜
    int x, y, w, h; //���Wx,y,��w,����h
    int speed; //�ړ����x
    int point; //�X�R�A���Z
};
//�G�@
struct ENEMY g_enemy[ENEMY_MAX];
struct ENEMY g_enemy00 = { TRUE,0,0,0,-50,APPLE_WIDTH,APPLE_HEIGHT,0,1 };

//�A�C�e��
struct ENEMY g_item[ITEM_MAX];
struct ENEMY g_item00 = { TRUE,0,0,0,-50,50,50,0,1 };

/***********************************************
 * �֐��̃v���g�^�C�v�錾
 ***********************************************/
void GameInit(void); //�Q�[����������
void GameMain(void); //�Q�[�����C������

void DrawGameTitle(void); //�^�C�g���`�揈��
void DrawGameOver(void); //�Q�[���I�[�o�[��ʕ`�揈��
void DrawEnd(void); //�Q�[���G���h�`�揈��
void DrawHelp(void); //�Q�[���w���v�`�揈��

void DrawRanking(void); //�����L���O�`�揈��
void InputRanking(void);//�����L���O����
int LoadImages(); //�摜�ǂݍ���
void UIView();
void TimeCount();
void SortRanking(void); //�����L���O�O���ёւ�
int SaveRanking(void); //�����L���O�f�[�^�̕ۑ�
int ReadRanking(void); //�����L���O�f�[�^�Ǎ���
void BackScrool(); //�w�i�摜�X�N���[������
void PlayerControl(); //���@����
void EnemyControl(); //�G�@����
int CreateEnemy(); //�G�@��������
int HitBoxPlayer(PLAYER* p, ENEMY* e); //�����蔻��

int LoadSounds(); //�T�E���h�̓ǂݍ��ݏ���
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
    while (ProcessMessage() == 0 && g_GameState != 99) {// && !(g_KeyFlg & PAD_INPUT_START)
        //�L�[���͎擾 
        g_OldKey = g_NowKey;
        g_NowKey = GetJoypadInputState(DX_INPUT_PAD1);
        g_KeyFlg = g_NowKey & ~g_OldKey;

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
            DrawGameOver();
            break;
        case 7:
            InputRanking();
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
    static int MenuNo = 0;

    //���j���[�J�[�\���ړ�����
    if (g_KeyFlg & PAD_INPUT_DOWN) {
        if (++MenuNo > 3)MenuNo = 0;
    }
    if (g_KeyFlg & PAD_INPUT_UP) {
        if (--MenuNo < 0)MenuNo = 3;
    }
    //z�L�[�Ń��j���[�I��
    if (g_KeyFlg & PAD_INPUT_A)g_GameState = MenuNo + 1;

    //�^�C�g���摜�\��
    DrawGraph(0, 0, g_TitleImage, FALSE);

    //���j���[
    //DrawGraph(120, 200, g_Menu, TRUE);

      //���j���[
    DrawGraph(310, 220 + MenuNo * 50, g_Applec, TRUE);

    ////���j���[�J�[�\��
    //DrawRotaGraph(90, 220 + MenuNo * 40, 0.7f, M_PI / 2, g_Cone, TRUE);
}

/***********************************************
 * �Q�[����������
 ***********************************************/
void GameInit(void)
{
    //�X�R�A�̏�������
    g_Score = 0;

    //���s������������
    g_Mileage = 0;
    g_MileageB = 0;

    //�G�P����������̏����ݒ�
    g_EnemyCount1 = 0;
    g_EnemyCount2 = 0;
    g_EnemyCount3 = 0;


    //�v���C���[�̏����ݒ�
    g_player.flg = TRUE;
    g_player.x = PLAYER_POS_X;
    g_player.y = PLAYER_POS_Y;
    g_player.w = PLAYER_WIDTH;
    g_player.h = PLAYER_HEIGHT;
    //g_player.angle = 0.0;
    g_player.count = 0;
    g_player.speed = PLAYER_SPEED;

    //�G�l�~�[�̏�������
    for (int i = 0; i < ENEMY_MAX; i++) {
        g_enemy[i].flg = FALSE;
    }

    //�A�C�e���̏�������
    for (int i = 0; i < ITEM_MAX; i++) {
        g_item[i].flg = FALSE;
    }

    //���݂̌o�ߎ��Ԃ𓾂�
    g_StartTime = GetNowCount();

    //�Q�[�����C��������
    g_GameState = 5;
}
/***********************************************
 * �Q�[�������L���O�\��
 ***********************************************/
void DrawRanking(void)
{
    //�X�y�[�X�L�[�Ń��j���[�ɖ߂�
    if (g_KeyFlg & PAD_INPUT_M) g_GameState = 0;

    //�����L���O�摜�\��
    DrawGraph(0, 0, g_RankingImage, FALSE);

    //�����L���O�ꗗ��\��
    SetFontSize(18);
    for (int i = 0; i < RANKING_DATA; i++) {
        DrawFormatString(50, 170 + i * 25, 0xffffff, "%2d %-10s %10d", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
    }
    DrawString(100, 450, "----�X�y�[�X�L�[�������ă^�C�g���ɖ߂� ----", 0xffffff, 0);
}
/***********************************************
 * �Q�[���w���v�`�揈��
 ***********************************************/
void DrawHelp(void)
{
    ////�X�y�[�X�L�[�Ń��j���[�ɖ߂�
    //if (g_KeyFlg & PAD_INPUT_M) g_GameState = 0;

    ////�^�C�g���摜�\��
    //DrawGraph(0, 0, g_TitleImage, FALSE);
    //SetFontSize(16);
    //DrawString(20, 120, "�w���v���", 0xffffff, 0);

    //DrawString(20, 160, "����͏�Q��������Ȃ���", 0xffffff, 0);
    //DrawString(20, 180, "���葱����Q�[���ł�", 0xffffff);
    //DrawString(20, 200, "�R�����s���邩��Q����", 0xffffff, 0);
    //DrawString(20, 220, "���񓖂���ƃQ�[���I�[�o�[�ł��B", 0xffffff, 0);
    //DrawString(20, 250, "�A�C�e���ꗗ", 0xffffff, 0);
    //DrawGraph(20, 260, g_Item[0], TRUE);
    //DrawString(20, 315, "���ƔR�����񕜂����B", 0xffffff, 0);
    //DrawGraph(20, 335, g_Item[1], TRUE);
    //DrawString(20, 385, "�_���[�W���󂯂Ă��鎞�Ɏ��Ƒϋv��", 0xffffff, 0);
    //DrawString(20, 405, "�ϋv�������Ă��Ȃ�������R���������񕜂��܂���B", 0xffffff, 0);
    //DrawString(150, 450, "---- �X�y�[�X�L�[�������ă^�C�g���֖߂� ----", 0xffffff, 0);



    //int Pad;        //�W���C�p�b�h�̓��͏�Ԋi�[�p�ϐ�

    //// while( ����ʂ�\��ʂɔ��f, ���b�Z�[�W����, ��ʃN���A )
    //while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
    //    Pad = GetJoypadInputState(DX_INPUT_PAD1);        //���͏�Ԃ�Pad�Ɋi�[
    //    if (Pad & PAD_INPUT_A) {        //�{�^��1�̓��̓t���O�������Ă�����
    //        DrawFormatString(0, 0, GetColor(255, 255, 255), "���͒��ł�A�{�^��");
    //    }
    //    if (Pad & PAD_INPUT_B) {        //�{�^��1�̓��̓t���O�������Ă�����
    //        DrawFormatString(0, 0, GetColor(255, 255, 255), "���͒��ł�B�{�^��");
    //    }
    //    if (Pad & PAD_INPUT_DOWN) {        //�{�^��1�̓��̓t���O�������Ă�����
    //        DrawFormatString(0, 0, GetColor(255, 255, 255), "���͒��ł����{�^��");
    //    }
    //}

    /*DINPUT_JOYSTATE input;
    int i;
    int Color;*/


}
/***********************************************
 * �Q�[���G���h�`�揈��
 ***********************************************/
void DrawEnd(void)
{
    //�G���h�摜�\��
    DrawGraph(0, 0, g_EndImage, FALSE);

    SetFontSize(24);
    DrawString(360, 480 - 24, "Thank you for Playing", 0xffffff, 0);

    //�^�C���̉��Z�������I���i3�b��j
    if (++g_WaitTime > 180)g_GameState = 99;
}
/***********************************************
 * �Q�[�����C��
 ***********************************************/
void GameMain(void)
{
    //main��BGM�𗬂��B�����Ȃ���
    PlaySoundMem(g_StageBGM, DX_PLAYTYPE_BACK, FALSE);

    BackScrool();
    PlayerControl();
    EnemyControl();
    UIView();
    TimeCount();

    //�X�y�[�X�L�[�Ń��j���[�ɖ߂�@�Q�[�����C������^�C�g���ɖ߂�ǉ�
    if (g_KeyFlg & PAD_INPUT_M)g_GameState = 6;
    SetFontSize(16);
    DrawString(150, 450, "---�X�y�[�X�L�[�������ăQ�[���I�[�o�[��---", 0xffffff, 0);
}
/***********************************************
 *�Q�[���I�[�o�[��ʕ`�揈��
 ***********************************************/
void DrawGameOver(void)
{
    BackScrool();//�`�������W3

    g_Score = (g_MileageB / 10 * 10) + g_EnemyCount3 * 50 + g_EnemyCount2 * 100 + g_EnemyCount1 * 200;


    //�X�y�[�X�L�[�Ń��j���[�ɖ߂�
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
    DrawString(220, 170, "�Q�[���I�[�o�[", 0xcc0000);
    SetFontSize(16);
    DrawString(180, 200, "���s����   ", 0x000000);
    DrawRotaGraph(230, 230, 0.3f, M_PI / 2, g_Teki[2], TRUE, FALSE);

    DrawRotaGraph(230, 250, 0.3f, M_PI / 2, g_Teki[1], TRUE, FALSE);

    DrawRotaGraph(230, 270, 0.3f, M_PI / 2, g_Teki[0], TRUE, FALSE);

    DrawFormatString(260, 200, 0xFFFFFF, "%6d x 10=%6d", g_MileageB / 10, g_MileageB / 10 * 10);

    DrawFormatString(260, 222, 0xFFFFFF, "%6d x 50=%6d", g_EnemyCount3, g_EnemyCount3 * 50);

    DrawFormatString(260, 243, 0xFFFFFF, "%6dx 100=%6d", g_EnemyCount2, g_EnemyCount2 * 100);

    DrawFormatString(260, 264, 0xFFFFFF, "%6dx 200=%6d", g_EnemyCount1, g_EnemyCount1 * 200);

    DrawString(300, 290, "�X�R�A", 0x000000);

    DrawFormatString(260, 290, 0xFFFFFF, "           =%6d", g_Score);

    DrawString(150, 450, "---�X�y�[�X�L�[�������ă^�C�g���֖߂� ---", 0xffffff, 0);

    StopSoundMem(g_StageBGM); //�Q�[���I�[�o�[�ɒǉ�����
    
}
/***********************************************
 * �����L���O���͏���
 ***********************************************/
void InputRanking(void)
{
    //�����L���O�摜�\��
    DrawGraph(0, 0, g_RankingImage, FALSE);

    //�t�H���g�T�C�Y�̐ݒ�
    SetFontSize(20);

    //���O���͎w��������̕`��
    DrawString(150, 240, "�����L���O�ɓo�^���܂�", 0xFFFFFF);
    DrawString(150, 270, "���O���p���œ��͂��Ă�������", 0xFFFFFF);

    //���O�̓���
    DrawString(150, 310, "> ", 0xFFFFFF);
    DrawBox(160, 305, 300, 335, 0x000055, TRUE);
    if (KeyInputSingleCharString(170, 310, 10, g_Ranking[4].name, FALSE) == 1) {
        g_Ranking[4].score = g_Score;	// �����L���O�f�[�^��5�ԖڂɃX�R�A��o�^
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
    //�A�C�e���摜�̓Ǎ���
    if ((g_Item[0] = LoadGraph("images/Chapter5/gasoline.bmp")) == -1)return -1;
    if ((g_Item[1] = LoadGraph("images/Chapter5/supana.bmp")) == -1)return -1;

    //�����L���O�f�[�^�̓Ǎ���
    if ((g_RankingImage = LoadGraph("images/Chapter5/Ranking.bmp")) == -1)return-1;

    //�G���f�B���O�摜�̓Ǎ���  
    if ((g_EndImage = LoadGraph("images/Chapter5/End.bmp")) == -1)return -1;

    //�G
    if (LoadDivGraph("images/Chapter5/apple.png", 4, 4, 1, 50, 50, g_Teki) == -1)return-1;

    //�X�e�[�W�w�i
    if ((g_StageImage = LoadGraph("images/Chapter5/haikei.png")) == -1)return -1;

    //�v���C���[
    if (LoadDivGraph("images/Chapter5/Player_1.png", 16, 4, 4, 76, 100, g_PlayerImage) == -1) return -1; //���@�摜
    //if ((g_Barrier = LoadGraph("images/Chapter5/barrier.png")) == -1)return -1;
    return 0;
}
//�T�E���h�ǂݍ���
int LoadSounds() {

    //�X�e�[�WBGM�f�[�^�̓ǂݍ���
    if ((g_StageBGM = LoadSoundMem("sounds/Chapter9/MusMus-BGM-104.wav")) == -1)return -1;
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

    //�t�@�C���I�[�v��
    if ((fp = fopen("dat/Chapter5/rankingdata.txt", "w")) == NULL) {
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
    if ((fp = fopen("dat/Chapter5/rankingdata.txt", "r")) == NULL) {
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

/***********************************************
 * �v���C���[�̈ړ�����
 * ��  ��:�Ȃ�
 * �߂�l:�Ȃ�
 ***********************************************/
void PlayerControl()
{
    //���E�ړ�
    if (g_player.flg == TRUE)
    {
        if (g_NowKey & PAD_INPUT_LEFT)
        {
            g_player.x -= g_player.speed;
            g_player.oldkey = 0;
        }
        if (g_NowKey & PAD_INPUT_RIGHT)
        {
            g_player.x += g_player.speed;
            g_player.oldkey = 1;
        }
    }

    //��ʂ��͂ݏo���Ȃ��悤�ɂ���
    if (g_player.x < 15) g_player.x = 15;
    if (g_player.x > 420) g_player.x = 420;


    //�v���C���[�̕\��
    if (g_player.flg == TRUE)
    {
        if (g_NowKey & PAD_INPUT_LEFT)
        {
            if (g_player.oldkey != 0)
            {
                g_player.walkspeed = 0;
                g_player.oldkey = 0;
            }

            if (g_player.walkspeed >= 5)
            {
                if (g_player.image >= 8 && g_player.image <= 10)
                {
                    g_player.image++;
                }

                else
                {
                    g_player.image = 8;
                }

                g_player.walkspeed = 0;
            }

            g_player.walkspeed++;
        }

        else if (g_NowKey & PAD_INPUT_RIGHT)
        {
            if (g_player.oldkey != 1)
            {
                g_player.walkspeed = 0;
                g_player.oldkey = 1;
            }

            if (g_player.walkspeed >= 5)
            {
                if (g_player.image >= 4 && g_player.image <= 6)
                {
                    g_player.image++;
                }

                else
                {
                    g_player.image = 4;
                }

                g_player.walkspeed = 0;
            }

            g_player.walkspeed++;
        }
    }

    DrawGraph(g_player.x, g_player.y, g_PlayerImage[g_player.image], TRUE);

    //�G�����������\��
   /* SetFontSize(16);

    DrawFormatString(510, 20, 0x000000, "�n�C�X�R�A");
    DrawFormatString(560, 40, 0xFFFFFF, "%08d", g_Ranking[0].score);
    DrawFormatString(510, 80, 0x000000, "��������");
    DrawRotaGraph(523, 120, 0.3f, 0, g_Teki[0], TRUE, FALSE);
    DrawRotaGraph(573, 120, 0.3f, 0, g_Teki[1], TRUE, FALSE);
    DrawRotaGraph(623, 120, 0.3f, 0, g_Teki[2], TRUE, FALSE);

    DrawFormatString(510, 140, 0xFFFFFF, "%03d", g_EnemyCount1);
    DrawFormatString(560, 140, 0xFFFFFF, "%03d", g_EnemyCount2);
    DrawFormatString(610, 140, 0xFFFFFF, "%03d", g_EnemyCount3);
    DrawFormatString(510, 200, 0x000000, "���s����");
    DrawFormatString(555, 220, 0xFFFFFF, "%08d", g_Mileage / 10);
    DrawFormatString(510, 240, 0x000000, "�X�s�[�h");
    DrawFormatString(555, 260, 0xFFFFFF, "%08d", g_player.speed);*/


}

/***********************************************
 * �G�l�~�[�̈ړ�
 * ���@���F�Ȃ�
 * �߂�l�F�Ȃ�
 ***********************************************/
void EnemyControl()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (g_enemy[i].flg == TRUE)
        {
            //�G�̕\��
            DrawGraph(g_enemy[i].x, g_enemy[i].y, g_enemy[i].img, TRUE);

            if (g_player.flg == FALSE)continue;

            //�܂��������Ɉړ�
            g_enemy[i].y += g_enemy[i].speed;// + g_player.speed - PLAYER_SPEED + 1;

            //��ʂ��͂ݏo���������
            if (g_enemy[i].y > SCREEN_HEIGHT + g_enemy[i].h) g_enemy[i].flg = FALSE;

            //�����蔻��
            if (HitBoxPlayer(&g_player, &g_enemy[i]) == TRUE)
            {
                /*g_player.flg = FALSE;
                g_player.speed = PLAYER_SPEED;
                g_player.count = 0;
                g_player.hp -= 100;
                g_enemy[i].flg = FALSE;
                if (g_player.hp <= 0) g_GameState = 6;*/

                g_enemy[i].flg = FALSE;

                g_player.ATARI_HANTEI += 1;
            }

            //g_player.ATARI_HANTEI = 0;
        }

        DrawFormatString(0, 33, 0x00ffff, "�����蔻�� = %d", g_player.ATARI_HANTEI);
    }

    //���s�������ƂɓG�o���p�^�[���𐧌䂷��
    if (g_Mileage / 10 % 50 == 0)
    {
        CreateEnemy();
    }
}

/***********************************************
 * �G�̐���
 * ��  ��:�Ȃ�
 * �߂�l:TRUE�����AFALSE���s
 ***********************************************/
int CreateEnemy()
{
    for (int i = 0; i < ENEMY_MAX; i++) 
    {
        if (g_enemy[i].flg == FALSE) 
        {
            g_enemy[i] = g_enemy00;
            g_enemy[i].type = GetRand(3);
            g_enemy[i].img = g_Teki[g_enemy[i].type];
            g_enemy[i].x = GetRand(6) * 70 + 30;

            if (g_enemy[i].type == 0)
            {
                g_enemy[i].speed = 2;
            }
            if (g_enemy[i].type == 1)
            {
                g_enemy[i].speed = 5;
            }
            if (g_enemy[i].type == 2)
            {
                g_enemy[i].speed = 10;
            }
            if (g_enemy[i].type == 3)
            {
                g_enemy[i].speed = 1;
            }
   
            //����
            return TRUE;
        }
    }

    //���s
    return FALSE;
}

/***********************************************
 * ���@�ƓG�@�̓����蔻��i�l�p�j
 * ��  ��:PLAYER�|�C���^
 * �߂�l:TRUE������AFALSE�Ȃ�
 ***********************************************/
int HitBoxPlayer(PLAYER* p, ENEMY* e)
{

    //x,y�͒��S���W�Ƃ���
    int sx1 = p->x;
    int sy1 = p->y;
    int sx2 = sx1 + p->w;
    int sy2 = sy1 + p->h;

    int dx1 = e->x;
    int dy1 = e->y;
    int dx2 = dx1 + e->w;
    int dy2 = dy1 + e->h;

    //�Z�`���d�Ȃ��Ă���΂�����
    if (sx1 < dx2 && dx1 < sx2 && sy1 < dy2 && dy1 < sy2) {
        return TRUE;
    }
    return FALSE;
}

void TimeCount(void)
{
    //�������Ԃ��߂�����Q�[���I�[�o�[
    int Time = TIMELIMIT - (GetNowCount() - g_StartTime);
    if (Time <= 0)
    {
        if (g_Ranking[RANKING_DATA - 1].score >= g_Score) {
            g_GameState = 2;
        }
        else {
            g_GameState = 7;
        }
    }
    SetFontSize(50);
    DrawFormatString(570, 100, 0xffffff, "%2d", Time / 1000);
}

void UIView(void)
{
    //UI�uTIME�v�\��
    SetFontSize(50);
    DrawString(520, 40, "TIME", 0xffffff, 0);

    //�E��������\��
    SetFontSize(16);
    DrawRotaGraph(523, 220, 0.5f, 0, g_Teki[0], TRUE, FALSE);
    DrawRotaGraph(573, 220, 0.5f, 0, g_Teki[1], TRUE, FALSE);
    DrawRotaGraph(623, 220, 0.5f, 0, g_Teki[2], TRUE, FALSE);
    DrawFormatString(510, 240, 0xFFFFFF, "%03d", g_EnemyCount1);
    DrawFormatString(560, 240, 0xFFFFFF, "%03d", g_EnemyCount2);
    DrawFormatString(610, 240, 0xFFFFFF, "%03d", g_EnemyCount3);

    //UI�uSCORE�v�\��
    SetFontSize(45);
    DrawString(510, 320, "SCORE", 0xffffff, 0);
}