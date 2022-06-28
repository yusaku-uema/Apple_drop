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
int g_Menu, g_Cone; //���j���[���
int g_Score = 0; //�X�R�A
int g_RankingImage; //�摜�p�ϐ�
int g_Item[2]; //�A�C�e���摜�ϐ�
int g_WaitTime = 0; //�҂�����
int g_EndImage; //�Q�[���G���h
int g_Mileage; //���s����
int g_MileageB; //�~��
int g_EnemyCount1, g_EnemyCount2, g_EnemyCount3; //�G�J�E���g

int g_Teki[3]; //�L�����摜�ϐ�

int g_StageImage;
int g_teki;
int g_Car, g_Barrier; //�L�����摜�ϐ�
/***********************************************
 * �萔��錾
 ***********************************************/
 //��ʗ̈�̑傫��
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
//���@�̋@��
const int PLAYER_POS_X = SCREEN_WIDTH / 2;
const int PLAYER_POS_Y = SCREEN_HEIGHT - 100;
const int PLAYER_WIDTH = 63;
const int PLAYER_HEIGHT = 120;
const int PLAYER_SPEED = 5;
const int PLAYER_HP = 1000;
const int PLAYER_FUEL = 20000;
const int PLAYER_BARRIER = 3;
const int PLAYER_BARRIERUP = 10;

//�G�@�̍ő吔
const int ENEMY_MAX = 8;//�`�������W1�ύX 20
//�A�C�e���̍ő吔
const int ITEM_MAX = 3;

//�����L���O�f�[�^�i�\���́j
struct RankingData {
    int no;
    char name[11];
    long score;
};
struct RankingData g_Ranking[RANKING_DATA];

//���@�̍\����
struct PLAYER {
    int flg; //�g�p�t���O
    int x, y;  //���W x,y
    int w, h; //��w ����h
    double angle; //�@�̂̌���   
    int count; //�^�C�~���O�p
    int speed; //�ړ����x
    int hp;    //�̗�
    int fuel; //�R��
    int bari; //�o���A��
    int baricht; //�o���A�p������
    int bariup; //�o���A�񐔁i���s�����A�b�v�j
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
struct ENEMY g_enemy00 = { TRUE,0,0,0,-50,63,120,0,1 };
struct ENEMY g_enemyCn = { TRUE,4,0,0,-50,18,18,0,1 };

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
void SortRanking(void); //�����L���O�O���ёւ�
int SaveRanking(void); //�����L���O�f�[�^�̕ۑ�
int ReadRanking(void); //�����L���O�f�[�^�Ǎ���
void BackScrool(); //�w�i�摜�X�N���[������
void PlayerControl(); //���@����
void EnemyControl(); //�G�@����
int CreateEnemy(); //�G�@��������
int HitBoxPlayer(PLAYER* p, ENEMY* e); //�����蔻��
void ItemControl(); //�A�C�e������
int CreateItem(); //�A�C�e����������
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
    //�A�C�e���摜�̓Ǎ���
    if ((g_Item[0] = LoadGraph("images/Chapter5/gasoline.bmp")) == -1)return -1;
    if ((g_Item[1] = LoadGraph("images/Chapter5/supana.bmp")) == -1)return -1;
    //�����L���O�f�[�^�̓Ǎ���
    if ((g_RankingImage = LoadGraph("images/Chapter5/Ranking.bmp")) == -1)return-1;
    //�G���f�B���O�摜�̓Ǎ���  
    if ((g_EndImage = LoadGraph("images/Chapter5/End.bmp")) == -1)return -1;
    //�G
    if (LoadDivGraph("images/Chapter5/car.bmp", 3, 3, 1, 63, 120, g_Teki) == -1)return-1;
    //�X�e�[�W�w�i
    if ((g_StageImage = LoadGraph("images/Chapter5/back.bmp")) == -1)return -1;
    //�v���C���[
    if ((g_Car = LoadGraph("images/Chapter5/car1pol.bmp")) == -1)return -1;
    if ((g_Barrier = LoadGraph("images/Chapter5/barrier.png")) == -1)return -1;
    if (LoadImages() == -1) return -1; //�摜�Ǎ��݊֐����Ăяo��
      //�Q�[�����[�v 
    while (ProcessMessage() == 0 && g_GameState != 99 && !(g_KeyFlg & PAD_INPUT_START)) {
        //�L�[���͎擾 
        g_OldKey = g_NowKey;
        g_NowKey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
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
    DrawGraph(120, 200, g_Menu, TRUE);

    //���j���[�J�[�\��
    DrawRotaGraph(90, 220 + MenuNo * 40, 0.7f, M_PI / 2, g_Cone, TRUE);
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
    g_player.angle = 0.0;
    g_player.count = 0;
    g_player.speed = PLAYER_SPEED;
    g_player.hp = PLAYER_HP;
    g_player.fuel = PLAYER_FUEL;
    g_player.bari = PLAYER_BARRIER;
    g_player.bariup = PLAYER_BARRIERUP;

    //�G�l�~�[�̏�������
    for (int i = 0; i < ENEMY_MAX; i++) {
        g_enemy[i].flg = FALSE;
    }

    //�A�C�e���̏�������
    for (int i = 0; i < ITEM_MAX; i++) {
        g_item[i].flg = FALSE;
    }

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
    //�X�y�[�X�L�[�Ń��j���[�ɖ߂�
    if (g_KeyFlg & PAD_INPUT_M) g_GameState = 0;

    //�^�C�g���摜�\��
    DrawGraph(0, 0, g_TitleImage, FALSE);
    SetFontSize(16);
    DrawString(20, 120, "�w���v���", 0xffffff, 0);

    DrawString(20, 160, "����͏�Q��������Ȃ���", 0xffffff, 0);
    DrawString(20, 180, "���葱����Q�[���ł�", 0xffffff);
    DrawString(20, 200, "�R�����s���邩��Q����", 0xffffff, 0);
    DrawString(20, 220, "���񓖂���ƃQ�[���I�[�o�[�ł��B", 0xffffff, 0);
    DrawString(20, 250, "�A�C�e���ꗗ", 0xffffff, 0);
    DrawGraph(20, 260, g_Item[0], TRUE);
    DrawString(20, 315, "���ƔR�����񕜂����B", 0xffffff, 0);
    DrawGraph(20, 335, g_Item[1], TRUE);
    DrawString(20, 385, "�_���[�W���󂯂Ă��鎞�Ɏ��Ƒϋv��", 0xffffff, 0);
    DrawString(20, 405, "�ϋv�������Ă��Ȃ�������R���������񕜂��܂���B", 0xffffff, 0);
    DrawString(150, 450, "---- �X�y�[�X�L�[�������ă^�C�g���֖߂� ----", 0xffffff, 0);

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
    BackScrool();
    PlayerControl();
    EnemyControl();
    ItemControl();
    //�X�y�[�X�L�[�Ń��j���ɖ߂�
    //if (g_KeyFlg & PAD_INPUT_M)g_GameState = 6;

    //SetFontSize(16);
    //DrawString(20, 20, "�Q�[�����C��", 0xffffff, 0);
    //DrawString(150, 450, "--- �X�y�[�X�L�[�������ăQ�[���I�[�o�[�� ---", 0xffffff, 0);
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
    //�^�C�g��
    if ((g_TitleImage = LoadGraph("images/Chapter5/Title.bmp")) == -1)return -1;
    //���j���[
    if ((g_Menu = LoadGraph("images/Chapter5/menu.bmp")) == -1)return -1;
    if ((g_Cone = LoadGraph("images/Chapter5/cone.bmp")) == -1)return -1;

    return 0;
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

    g_Mileage += g_player.speed;
    if (g_GameState == 5) {
        g_MileageB += g_player.speed;
    }
    //�X�e�[�W�摜�\��

    //�`��\�G���A��ݒ�
    SetDrawArea(0, 0, 500, 480);

    DrawGraph(0, g_Mileage % 480 - 480, g_StageImage, FALSE);

    DrawGraph(0, g_Mileage % 480, g_StageImage, FALSE);

    //�G���A��߂�
    SetDrawArea(0, 0, 640, 480);

    //�X�R�A���\���̈�
    DrawBox(500, 0, 640, 480, 0x009900, TRUE);
}

/***********************************************
 * �v���C���[�̈ړ�����
 * ��  ��:�Ȃ�
 * �߂�l:�Ȃ�
 ***********************************************/
void PlayerControl()
{
    //�R���̏���
    g_player.fuel -= g_player.speed;
    //�Q�[���I�[�o�[������
    if (g_player.fuel <= 0)  g_GameState = 6;

    //Z�L�[�ŉ���
    if (g_KeyFlg & PAD_INPUT_A && g_player.speed < 10) g_player.speed += 1;

    //X�L�[�Ō���
    if (g_KeyFlg & PAD_INPUT_B && g_player.speed > 1)g_player.speed -= 1;

    //�㉺���E�ړ�
    if (g_player.flg == TRUE) {
        if (g_NowKey & PAD_INPUT_UP) g_player.y -= g_player.speed;
        if (g_NowKey & PAD_INPUT_DOWN) g_player.y += g_player.speed;
        if (g_NowKey & PAD_INPUT_LEFT) g_player.x -= g_player.speed;
        if (g_NowKey & PAD_INPUT_RIGHT) g_player.x += g_player.speed;
    }
    //��ʂ��͂ݏo���Ȃ��悤�ɂ���B
    if (g_player.x < 32)  g_player.x = 32;

    if (g_player.x > SCREEN_WIDTH - 180) g_player.x = SCREEN_WIDTH - 180;

    if (g_player.y < 60) g_player.y = 60;

    if (g_player.y > SCREEN_HEIGHT - 60) g_player.y = SCREEN_HEIGHT - 60;

    //�v���C���[�̕\��
    if (g_player.flg == TRUE) {
        if (g_NowKey & PAD_INPUT_LEFT) {
            DrawRotaGraph(g_player.x, g_player.y, 1.0f, -M_PI / 18, g_Car, TRUE, FALSE);

        }
        else if (g_NowKey & PAD_INPUT_RIGHT) {
            DrawRotaGraph(g_player.x, g_player.y, 1.0f, M_PI / 18, g_Car, TRUE, FALSE);

        }
        else {
            DrawRotaGraph(g_player.x, g_player.y, 1.0f, 0, g_Car, TRUE, FALSE);
        }

        if (g_KeyFlg & PAD_INPUT_C && g_player.bari > 0 && g_player.baricht <= 0) {
            g_player.bari--;
            g_player.baricht = 1000;
        }
        if (g_player.baricht > 0) {
            g_player.baricht -= g_player.speed;
            DrawRotaGraph(g_player.x, g_player.y, 1.0f, 0, g_Barrier, TRUE, FALSE);
        }
        else {
            g_player.baricht = 0;
        }


    }
    else {
        DrawRotaGraph(g_player.x, g_player.y, 1.0f, M_PI / 8 * (++g_player.count / 5), g_Car, TRUE, FALSE);
        if (g_player.count >= 80) g_player.flg = TRUE;
    }

    //�G�����������\��
    SetFontSize(16);
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
    DrawFormatString(555, 220, 0xFFFFFF, "%08d", g_MileageB / 10);
    DrawFormatString(510, 240, 0x00000, "�X�s�[�h");
    DrawFormatString(555, 260, 0xFFFFFF, "%08d", g_player.speed);

    //�o���A�̕\��
    for (int i = 0; i < g_player.bari; i++) {
        DrawRotaGraph(520 + i * 25, 340, 0.2f, 0, g_Barrier, TRUE, FALSE);
    }

    //�R���Q�[�W�̕\��
    int F_X = 510; int F_Y = 390; int F_W = 100; int F_H = 20;
    DrawString(F_X, F_Y, "FUEL METER", 0x000000, 0);
    //�����̃Q�[��
    DrawBox(F_X, F_Y + 20, F_X + (int)(g_player.fuel * F_W / PLAYER_FUEL), F_Y + 20 + F_H, 0x0066cc, TRUE);
    //�O���̃Q�[�W�g
    DrawBox(F_X, F_Y + 20, F_X + F_W, F_Y + 20 + F_H, 0x000000, FALSE);

    //�̗̓Q�[�W�̕\��
    int X = 510; int Y = 430; int W = 100; int H = 20;
    DrawString(X, Y, "PLAYER HP", 0x000000, 0);
    //�����̃Q�[��
    DrawBox(X, Y + 20, X + (int)(g_player.hp * W / PLAYER_HP), Y + 20 + H, 0xff0000, TRUE);
    //�O���̃Q�[�W�g
    DrawBox(X, Y + 20, X + W, Y + 20 + H, 0x000000, FALSE);
}

/***********************************************
 * �G�l�~�[�̈ړ�
 * ��  ��:�Ȃ�
 * �߂�l:�Ȃ�
 ***********************************************/
void EnemyControl()
{
    for (int i = 0; i < ENEMY_MAX; i++) {
        if (g_enemy[i].flg == TRUE) {

            //�G�̕\��
            DrawRotaGraph(g_enemy[i].x, g_enemy[i].y, 1.0f, 0, g_enemy[i].img, TRUE, FALSE);
            if (g_player.flg == FALSE)continue;

            //�^���������Ɉړ�
            g_enemy[i].y += g_enemy[i].speed + g_player.speed - PLAYER_SPEED + 1;

            //��ʂ��͂ݏo������폜
            if (g_enemy[i].y > SCREEN_HEIGHT + g_enemy[i].h) g_enemy[i].flg = FALSE;

            //�G�@��ǂ��z������J�E���g����
            if (g_enemy[i].y > g_player.y && g_enemy[i].point == 1) {
                g_enemy[i].point = 0;
                if (g_enemy[i].type == 0)g_EnemyCount1++;
                if (g_enemy[i].type == 1)g_EnemyCount2++;
                if (g_enemy[i].type == 2)g_EnemyCount3++;

            }
            //�����蔻��
            if (HitBoxPlayer(&g_player, &g_enemy[i]) == TRUE && g_player.baricht <= 0) {

                g_player.flg = FALSE;
                g_player.speed = PLAYER_SPEED;
                g_player.count = 0;
                g_player.hp -= 100;
                g_enemy[i].flg = FALSE;
                if (g_player.hp <= 0) g_GameState = 6;
            }
        }
    }

    //���s�������ƂɓG�o���p�^���𐧌䂷��
    if (g_Mileage / 10 % 50 == 0) {
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
    for (int i = 0; i < ENEMY_MAX; i++) {
        if (g_enemy[i].flg == FALSE) {
            g_enemy[i] = g_enemy00;
            g_enemy[i].type = GetRand(2);
            g_enemy[i].img = g_Teki[g_enemy[i].type];
            g_enemy[i].x = GetRand(4) * 105 + 40;
            g_enemy[i].speed = g_enemy[i].type * 2;
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
    int sx1 = p->x - (p->w / 2);
    int sy1 = p->y - (p->h / 2);
    int sx2 = sx1 + 4 + p->w;//�`�������W2�ύX+40
    int sy2 = sy1 + 40 + p->h;

    int dx1 = e->x - (e->w / 2);
    int dy1 = e->y - (e->h / 2);
    int dx2 = dx1 + e->w;//int dx2 = dx1 + e->w,�`�������W2�ύX
    int dy2 = dy1 + e->h;//int dy2 = dy1 + e->h

    //�Z�`���d�Ȃ��Ă���΂�����
    if (sx1 < dx2 && dx1 < sx2 && sy1 < dy2 && dy1 < sy2) {
        return TRUE;
    }
    return FALSE;
}

/***********************************************
 * �A�C�e���̐���
 * ��  ��:�Ȃ�
 * �߂�l:�Ȃ�
 ***********************************************/
void ItemControl()
{
    for (int i = 0; i < ITEM_MAX; i++) {
        if (g_item[i].flg == TRUE) {

            //�A�C�e���̕\��
            DrawRotaGraph(g_item[i].x, g_item[i].y, 1.0f, 0, g_item[i].img, TRUE);
            if (g_player.flg == FALSE)continue;

            //�^���������Ɉړ�
            g_item[i].y += g_item[i].speed + g_player.speed - PLAYER_SPEED;
            //�͂ݏo������폜
            if (g_item[i].y > SCREEN_HEIGHT) g_item[i].flg = FALSE;

            //�����蔻��
            if (HitBoxPlayer(&g_player, &g_item[i]) == TRUE) {
                g_item[i].flg = FALSE;
                if (g_item[i].type == 0) g_player.fuel += g_item[i].point;
                if (g_item[i].type == 1) {
                    g_player.hp += g_item[i].point;
                    if (g_player.hp > PLAYER_HP)
                        g_player.hp = PLAYER_HP;
                }
            }
        }
    }
    //���s�������ƂɓG�o���p�^�[���𐧌�
    if (g_Mileage / 10 % 500 == 0) {
        CreateItem();
    }
}
/***********************************************
 * �A�C�e���̐���
 * ��  ��:�Ȃ�
 * �߂�l:TRUE�����AFALSE���s
 ***********************************************/
int CreateItem()
{
    for (int i = 0; i < ITEM_MAX; i++) {
        if (g_item[i].flg == FALSE) {
            g_item[i] = g_item00;
            g_item[i].type = GetRand(1);
            g_item[i].img = g_Item[g_item[i].type];
            g_item[i].x = GetRand(4) * 105 + 40;
            g_item[i].speed = 1 + g_item[i].type * 3;
            if (g_item[i].type == 0) g_item[i].point = 500;
            if (g_item[i].type == 1) g_item[i].point = 50;

            //����
            return TRUE;
        }
    }
    //���s
    return FALSE;
}
