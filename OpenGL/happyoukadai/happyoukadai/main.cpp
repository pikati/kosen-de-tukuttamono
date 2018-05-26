#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#pragma comment(lib,"winmm.lib")

#include <GL/freeglut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <random>
#include <mmsystem.h>
#include <windows.h>
#include "DxLib.h"

#define WIDTH 640
#define HEIGHT 480
#define TITLE 0
#define DES 1//説明
#define GAME 2
#define WIN 3
#define LOSE 4

void fullscreen();
void DrawString(const char *str, void *font, float x, float y, float z);
void create_erina();
void create_enemy();
void create_floor();
void stand_by1(GLdouble i);
void stand_by2(GLdouble i);
void stand_by3(GLdouble i);
void run1(GLdouble i);
void run2(GLdouble i);
void run3(GLdouble i);
void jump1(GLdouble i);
void jump2(GLdouble i);
void attack1(GLdouble i);
void attack2(GLdouble i);
void attack3(GLdouble i);
void attack4(GLdouble i);
void atk_combo1();
void atk_combo2();
void stand_by();
void run();
void my_hit_hantei();
void my_attack_hit_hantei();
void enemy_hit_hantei();
void beam_hit_hantei(int n);
void attack_hit(int n);//自分の攻撃が敵に当たったか判定
void e_hit();//敵が自分に当たったか判定
void e_attack_damage();
void hammer_damage(int n); 
void e_body_damage();
void create_beam(float n);
void ear_outside(GLdouble i, GLdouble j);
void hair_shadow(GLdouble i, GLdouble j);
void hair(GLdouble i, GLdouble j);
void bunny_suit_shadow(GLdouble i, GLdouble j);
void bunny_suit(GLdouble i, GLdouble j);
void skin_shadow_thick(GLdouble i, GLdouble j);
void skin_shadow_thin(GLdouble i, GLdouble j);
void skin(GLdouble i, GLdouble j);
void eyelash(GLdouble i, GLdouble j);
void eye_thick(GLdouble i, GLdouble j);
void eye(GLdouble i, GLdouble j);
void eye_thin(GLdouble i, GLdouble j);
void mouth(GLdouble i, GLdouble j);
void shadow1(GLdouble i, GLdouble j);
void shadow2(GLdouble i, GLdouble j);
void reg_shadow_thick(GLdouble i, GLdouble j);
void reg_shadow_thin(GLdouble i, GLdouble j);
void toe(GLdouble i, GLdouble j);
void hammer_scarlet(GLdouble i, GLdouble j);
void hammer_dark_red(GLdouble i, GLdouble j);
void hammer_yellow(GLdouble i, GLdouble j);
void hammer_gray(GLdouble i, GLdouble j);
void hammer_effect(GLdouble i, GLdouble j);
void black(GLdouble i, GLdouble j);
void white(GLdouble i, GLdouble j);

typedef struct {
	int hp;
	int atk;
	double appearX;
	double appearY;
	double appearZ;

public:
	void Init(int myHp, int myAtk, double appX, double appY, double appZ) {
		hp = myHp;
		atk = myAtk;
		appearX = appX;
		appearY = appY;
		appearZ = appZ;
	}

}charactor;

bool onMoveKeyPress = false;
bool right = false;
bool left = false;
bool rightward = false;
bool leftwrad = true;
bool Change = false;
bool jump = false;
bool atk1 = false;
bool atk2 = false;
bool atk2start = false;
bool hit_hammer = false;
bool enemy_beam = false;
bool my_invincible = false;//無敵かどうか
bool enemy_invincible = false;
double myHit[4] = { 10000 };//0…zの左端　1…zの右端　2…xの上端　3…xの下端　当たり判定は正方形にするので4つの座標でおｋ 初期値を大きくずらすことで開始直後に当たらないようにする
double myAtkHit[4] = { 20000 };
double enemyHit[4] = { 30000 };
double enemyAtkHit[4] = { 40000 };
double x = 0;
double y = 0;
double z = 0;
double moveX = 0;
double moveZ = 0;
double enemyX = 0;
double enemyZ = 0;

int scene = 0;
int beamTimer = 0;
int beamLV = 1;
int bgm;
int se;
int hidame;
int time = 0;

charactor erina;
charactor enemy1;
int isCOLORON = 1;
//0:GL_COLORオフ ，1:GL_COLORオン
int SHADINGMODE = 0;
//0:スムーズシェーディング，1:フラットシェーディング

void init(void) {
	SetNotWinFlag(true);
	DxLib_Init();

	if (isCOLORON) {
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	}
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	if (SHADINGMODE)glShadeModel(GL_FLAT);//フラットシェーディング
	else glShadeModel(GL_SMOOTH);//スムーズシェーディング
								 
	bgm = LoadSoundMem("WASTE.ogg");
	se = LoadSoundMem("HammerHit.ogg");
	hidame = LoadSoundMem("hidame.ogg");
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);

	erina.Init(500, 150, 0, 0, 0);
	enemy1.Init(3000, 20, 0, 0, 0);

	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
}

void reshape(int width, int height)
{
	static GLfloat lightPosition[4] = { 30.0f, 10.0f, 0.25f, 1.0f };
	static GLfloat lightDiffuse[3] = { 1.0f, 1.0f, 1.0f };
	static GLfloat lightAmbient[3] = { 0.25f, 0.25f, 0.25f };
	static GLfloat lightSpecular[3] = { 1.0f, 1.0f, 1.0f };

	glEnable(GL_LIGHT0);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)width / (double)height, 0.1, 200.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/*こいつでカメラをいじるカメラ位置のx,y,z　カメラの向く座標のx,y,z　画面の上方向を指すベクトル*/
	gluLookAt(0.1,200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}

void display()
{
	void *font = GLUT_BITMAP_HELVETICA_18;
	char title[30];
	char erinahp[10];
	char enemyhp[20];
	char z[20];
	char c[20];
	char f[20];
	char exit[20];
	char right[30];
	char left[30];
	char des[30];
	char start[20];
	char win[10];
	char lose[10];
	char Score[20];
	char Time[50];
	char Score2[20];
	char Time2[50];
	int score;
	static float n = 0;
	
	glClearColor(0.3f, 0.7f, 0.8f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (scene == TITLE) {
		glTranslatef(0.0, 0.0, 20.0);
		stand_by();
		glTranslatef(0.0, 0.0, -20.0);
		glColor3d(0.0, 0.0, 0.0);
		sprintf_s(title, "%s", "bunny action");
		DrawString(title, font, -10.0, 2.0, 0.0);
		sprintf_s(des, "%s", "press d description");
		DrawString(des, font, 0.0, 2.0, 0.0);
		sprintf_s(start, "%s", "press s game start");;
		DrawString(start, font, 10.0, 2.0, 0.0);
		moveX = 0;
		moveZ = 0;
	}
	else if (scene == DES) {
		glColor3d(0.0, 0.0, 0.0);
		sprintf_s(z, "%s", "z---jump");

		DrawString(z, font, -10.0, 2.0, 60.0 );
		sprintf_s(c, "%s", "c---attack");
		DrawString(c, font, -10.0, 2.0, -30.0);
		sprintf_s(f, "%s", "f---full screen");
		DrawString(f, font, 10.0, 2.0, 60.0);
		sprintf_s(exit, "%s", "q esc---exit game");
		DrawString(exit, font, 10.0, 2.0, -30.0);
		sprintf_s(right, "%s", "right arrow---move right");
		DrawString(right, font, 0.0, 2.0, 60.0);
		sprintf_s(left, "%s", "left arrow---move left");
		DrawString(left, font, 0.0, 2.0, -30.0);
		moveX = 0;
		moveZ = 0;
	}
	else if (scene == GAME) {

		create_erina();
		create_enemy(); 
		create_floor();

		if (enemy_beam) {
			
			if (beamTimer == 10 || beamTimer == 40 || beamTimer == 60 || beamTimer == 90 || beamTimer == 110 || beamTimer == 150 || beamTimer == 180 || beamTimer == 210) {
				std::random_device rnd;
				std::mt19937 mt(rnd());
				std::uniform_real_distribution<> rand(-68, 0);//この範囲内のランダムな場所にビームを撃つ
				n = rand(mt);
				beam_hit_hantei(n);
			}
			
			switch (beamLV) {
			case 1:
				if (beamTimer >= 10 && beamTimer < 30 || beamTimer >= 110 && beamTimer < 130 || beamTimer >= 210 && beamTimer < 230) {
					create_beam(n);
				}
				else {
					beam_hit_hantei(1000);//ビームないときは判定を遠くにする
				}
				break;
			case 2:
				if (beamTimer >= 10 && beamTimer < 30 || beamTimer >= 60 && beamTimer < 80 || beamTimer >= 110 && beamTimer < 130 || beamTimer >= 210 && beamTimer < 230) {
					create_beam(n);
				}
				else {
					beam_hit_hantei(1000);
				}
				break;
			case 3:
				if (beamTimer >= 10 && beamTimer < 20 || beamTimer >= 40 && beamTimer < 50 || beamTimer >= 60 && beamTimer < 70 || beamTimer >= 90 && beamTimer < 100 || beamTimer >= 110 && beamTimer < 120 || beamTimer >= 150 && beamTimer < 160 || beamTimer >= 180 && beamTimer < 190 || beamTimer >= 210 && beamTimer < 240) {
					create_beam(n);
				}
				else {
					beam_hit_hantei(1000);
				}
				break;
			}
		}
		sprintf_s(erinahp, "%d", erina.hp);
		glColor3d(0.0, 0.0, 0.0);
		DrawString(erinahp, font, -60.0 - moveX + x, 2.0, 60.0 - moveZ);

		sprintf_s(enemyhp, "%d", enemy1.hp);
		glColor3d(1.0, 0.0, 1.0);
		DrawString(enemyhp, font, -60.0 - moveX + x, 2.0, -60.0 - moveZ);

		sprintf_s(Time, "%d", time);
		DrawString(Time, font, 50.0 - moveX + x, 2.0, 60.0 - moveZ);

	}
	else if (scene == WIN) {
		glColor3d(0.0, 0.0, 0.0);
		score = ((10000 - time) + erina.hp * 10) * 1.5;
		sprintf_s(win, "%s", "win");
		DrawString(win, font, -50.0, 2.0, 0.0);
		sprintf_s(Time2, "%s", "time");
		DrawString(Time2, font, -20.0, 2.0, 10.0);
		sprintf_s(Score2, "%s", "score");
		DrawString(Score2, font, 0.0, 2.0, 10.0);
		sprintf_s(Time, "%d", time);
		DrawString(Time, font, -20.0, 2.0, -10.0);
		sprintf_s(Score, "%d", score);
		DrawString(Score, font,0.0, 2.0, -10.0);
		moveX = 0;
		moveZ = 0;
	}
	else if (scene == LOSE) {
		glColor3d(0.0, 0.0, 0.0);
		score = ((10000 - time) + erina.hp * 10);
		sprintf_s(lose, "%s", "lose");
		DrawString(win, font, -50.0, 2.0, 0.0);
		sprintf_s(Time2, "%s", "time");
		DrawString(Time2, font, -20.0, 2.0, 10.0);
		sprintf_s(Score2, "%s", "score");
		DrawString(Score2, font, 0.0, 2.0, 10.0);
		sprintf_s(Time, "%d", time);
		DrawString(Time, font, -20.0, 2.0, -10.0);
		sprintf_s(Score, "%d", score);
		DrawString(Score, font, 0.0, 2.0, -10.0);
		moveX = 0;
		moveZ = 0;
	}

	glutSwapBuffers();
}

void fullscreen() {
	int nMode = 0;
	DEVMODE devMode;
	HWND hWnd;
	hWnd = GetActiveWindow();
	if (Change) {
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
		while (EnumDisplaySettings(NULL, nMode++, &devMode)) {
			//640×480をサポートしているか
			if (devMode.dmPelsWidth != WIDTH || devMode.dmPelsHeight != HEIGHT)continue;
			//32ビットカラーをサポートしているか
			if (devMode.dmBitsPerPel != 32)continue;
			//テストして切り替えれるなら
			if (ChangeDisplaySettings(&devMode, CDS_TEST) == DISP_CHANGE_SUCCESSFUL)break;
		}
		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
		glutFullScreen();
	}
	else {
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		ChangeDisplaySettings(NULL, 0);
		glutPositionWindow(100, 100);
		glutReshapeWindow(WIDTH, HEIGHT);
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
	case 'Q':
	case '\033': /* '\033' は ESC の ASCII コード */
		exit(0);
	case 'f':
		if (Change == false) { Change = true; }
		else { Change = false; }
		fullscreen();
		break;
	case 'c':
		atk1 = true;
		if (atk2start == true) {
			atk2start = false;
			atk2 = true;
		}
		break;
	case 'd':
		scene = 1;
		break;
	case 's':
		scene = 2;
		break;
	case 'z':
		jump = true;
	default:
		break;
	}
}

void keyboardup(unsigned char key, int x, int y) {
	
	switch (key) {
	case'z':
		jump = false;
		break;
	default:
		break;
	}
}

void specialkeybord(int key, int x, int y) {

	switch (key) {
	
	case GLUT_KEY_RIGHT:
		right = true;
		rightward = true;
		leftwrad = false;
		break;
	case GLUT_KEY_LEFT:
		left = true;
		rightward = false;
		leftwrad = true;
		break;
	default:
		break;
	}
} 

void specialkeyup(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_RIGHT:
		right = false;
		break;
	case GLUT_KEY_LEFT:
		left = false;
		break;
	default:
		break;
	}
}

void timer(int value) {
	static int count1 = 0;
	static int count2 = 0;
	static int enemycount = 0;
	int moveEnemyZ = 0;
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_real_distribution<> rand(-10, 10);
	moveEnemyZ = rand(mt);
	
	if (right) {
		z -= 1.0;
		if (z <= -5.0) {
			z = -5.0;
		}
	}
	if (left) {
		z += 1.0;
		if (z >= 6.5) {
			z = 6.5;
		}
	}
	if (z < -0.01) {
		z += 0.5;
	}
	else if (z > 0.01) {
		z -= 0.5;
	}
	if (moveZ >= 100 && leftwrad) {
		z = 0;
	}
	if (moveZ <= -90 && rightward) {
		z = 0;
	}
	moveZ += z;
	
	if (my_invincible) {
		count1++;
	}
	if (count1 >= 16) {
		my_invincible = false;
		count1 = 0;
	}
	if (enemy_invincible) {
		count2++;
	}
	if (count2 >= 2) {
		enemy_invincible = false;
		count2 = 0;
	}
	if (scene == GAME) {
		if (enemycount > 50) {
			beamTimer++;
		}
		if (enemycount > 50 && enemycount < 60) {
			enemyX -= 3.0;
		}
		else if (enemycount > 61 && enemycount < 290) {
			enemy_beam = true;
		}
		if (enemycount > 290 && enemycount < 300) {
			enemy_beam = false;
			enemyX += 3.0;
			beamTimer = 0;
			beamLV++;
			if (beamLV > 3) {
				beamLV = 3;
			}
		}
		if (enemycount >= 300) {
			enemycount = 0;
		}
		if (enemyZ > 90) {
			moveEnemyZ = -10;
		}
		if (enemyZ < -80) {
			moveEnemyZ = 10;
		}
		enemyZ += moveEnemyZ;
		enemycount++;
	}
	if (scene == GAME) {
		time++;
	}
	
	glutPostRedisplay();
	glutTimerFunc(120, timer, 0);
}

int main(int argc, char* argv[])
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("rabi-ribi");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardup);
	glutSpecialFunc(specialkeybord);
	glutSpecialUpFunc(specialkeyup);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutTimerFunc(100, timer, 0);
	init();
	glutMainLoop();

	return 0;
}



void DrawString(const char *str, void *font, float x, float y, float z)
{
	glRasterPos3f(x, y, z);
	while (*str) {
		glutBitmapCharacter(font, *str);
		++str;
	}
}

void create_erina()
{
	GLdouble i;
	my_hit_hantei();
	enemy_hit_hantei();
	e_hit();
	
	glTranslated(x, y, z);
	if (right == false && left == false || right == true && left == true) {//立ち状態の時
		atk2start = false;
		if (moveX >= 0) {
			x = 0;//自分が0まで下がったら絶対に着地する
		}
		if (jump == true) {//ジャンプした時
			for (i = -10.5; i < 10.5; i += 0.5) {
				if (leftwrad == true) {//左向いていたらこっち
					jump1(i);
				}
				else if (rightward == true) {//右向いていたらこっち
					glRotatef(180, 1, 0, 0);
					jump1(i);
					glRotatef(-180, 1, 0, 0);
				}
			}
			x -= 2.0;
			if (x <= -6.0) {
				x = -6.0;
			}
		}
		else if (moveX < 0) {//降下中
			for (i = -10.5; i < 10.5; i += 0.5) {
				if (leftwrad == true) {
					jump2(i);
				}
				else if (rightward == true) {
					glRotatef(180, 1, 0, 0);
					jump2(i);
					glRotatef(-180, 1, 0, 0);
				}
			}
			x += 2.0;
			if (x >= 6.0) {
				x = 6.0;
			}
		}
		else if (atk1 == true) {//攻撃したとき
			my_attack_hit_hantei();//攻撃しているときの当たり判定の処理
			attack_hit(1);
			if (leftwrad == true) {
				atk_combo1();
			}
			else if (rightward == true) {
				glRotatef(180, 1, 0, 0);
				atk_combo1();
				glRotatef(-180, 1, 0, 0);
			}
			enemy_invincible = true;
		}
		else if (atk2 == true) {//連続で攻撃したとき
			my_attack_hit_hantei();
			attack_hit(2);
			if (leftwrad == true) {
				atk_combo2();
			}
			else if (rightward == true) {
				glRotatef(180, 1, 0, 0);
				atk_combo2();
				glRotatef(-180, 1, 0, 0);
			}
			enemy_invincible = true;
		}
		else {//何もしていないとき
			if (leftwrad == true) {
				stand_by();
			}
			else if (rightward == true) {
				glRotatef(180, 1, 0, 0);
				stand_by();
				glRotatef(-180, 1, 0, 0);
			}
		}
	}
	else if (left == true) {//左に移動しているとき
		atk2start = false;
		if (jump == false && moveX >= 0) {
			x = 0;
		}
		if (jump == true) {
			for (i = -10.5; i < 10.5; i += 0.5) {
				jump1(i);
			}
			x -= 2.0;
			if (x <= -6.0) {
				x = -6.0;
			}
		}
		else if (moveX < 0) {
			for (i = -10.5; i < 10.5; i += 0.5) {
				jump2(i);
			}
			x += 2.0;
			if (x >= 6.0) {
				x = 6.0;
			}
		}
		else {
			run();
		}
	}
	else if (right == true) {//右に移動しているとき
		atk2start = false;
		if (jump == false && moveX >= 0) {
			x = 0;
		}
		if (jump == true) {
			for (i = -10.5; i < 10.5; i += 0.5) {
				glRotatef(180, 1, 0, 0);
				jump1(i);
				glRotatef(-180, 1, 0, 0);
			}
			x -= 2.0;
			if (x <= -6.0) {
				x = -6.0;
			}
		}
		else if (moveX < 0) {
			for (i = -10.5; i < 10.5; i += 0.5) {
				glRotatef(180, 1, 0, 0);
				jump2(i);
				glRotatef(-180, 1, 0, 0);
			}
			x += 2.0;
			if (x >= 6.0) {
				x = 6.0;
			}
		}
		else {
			glRotatef(180, 1, 0, 0);
			run();
			glRotatef(-180, 1, 0, 0);
		}
	}
	if (moveX <= -68 && jump) {
		x = 0;
	}
	moveX += x;//縦方向の移動量を保存
}

void my_hit_hantei() {
	myHit[0] = 6 + moveZ;
	myHit[1] = 0 + moveZ;
	myHit[2] = 6 + moveX;
	myHit[3] = 0 + moveX;
}

void my_attack_hit_hantei() {

	if (leftwrad) {
		myAtkHit[0] = 18 + moveZ;
		myAtkHit[1] = 0 + moveZ;
		myAtkHit[2] = 10 + moveX;
		myAtkHit[3] = -10 + moveX;
	}
	else if (rightward) {
		myAtkHit[0] = 0 + moveZ;
		myAtkHit[1] = -18 + moveZ;
		myAtkHit[2] = 10 + moveX;
		myAtkHit[3] = -10 + moveX;
	}
	
}

void enemy_hit_hantei() {
	enemyHit[0] = 8 + enemyX;
	enemyHit[1] = 0 + enemyX;
	enemyHit[2] = 5 + enemyZ;
	enemyHit[3] = 0 + enemyZ;
}

void beam_hit_hantei(int n) {
	enemyAtkHit[0] = 1 + n;
	enemyAtkHit[1] = -1 + n;
	enemyAtkHit[2] = 100;
	enemyAtkHit[3] = -100;
}

void attack_hit(int n) {
	double i;
	double j;

	for (i = myAtkHit[3]; i < myAtkHit[2]; i++) {
		for (j = myAtkHit[1]; j < myAtkHit[0]; j++) {
			if (enemyHit[1] < i && i < enemyHit[0] && enemyHit[3] < j && j < enemyHit[2]) {
				if (enemy_invincible == false) {
					hammer_damage(n);
					PlaySoundMem(se, DX_PLAYTYPE_BACK);
				}
			}
			if (hit_hammer) {
				break;
			}
		}
		if (hit_hammer) {
			hit_hammer = false;
			break;
		}
	}
}

void e_hit() {
	double i;
	double j;

	for (i = myHit[3]; i < myHit[2]; i++) {
		for (j = myHit[1]; j < myHit[0]; j++) {
			if (my_invincible) {
				break;
			}
			if (enemyHit[1] < i && i < enemyHit[0] && enemyHit[3] < j && j < enemyHit[2]) {
				e_body_damage();
				PlaySoundMem(hidame, DX_PLAYTYPE_BACK);
				my_invincible = true;
			}
			if (my_invincible) {
				break;
			}
			if (enemyAtkHit[1] < i && i < enemyAtkHit[0] && enemyAtkHit[3] < j && j < enemyAtkHit[2]) {
				e_attack_damage();
				PlaySoundMem(hidame, DX_PLAYTYPE_BACK);
				my_invincible = true;
			}
		}
		if (my_invincible) {
			break;
		}
	}
}

void hammer_damage(int n){

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_real_distribution<> rand(0.8, 1.2);

	switch (n)
	{
	case 1:
		enemy1.hp -= int(erina.atk / 35 * 1.0 * rand(mt));
		break;
	case 2:
		enemy1.hp -= int(erina.atk / 35 * 1.5 * rand(mt));
		break;
	default:
		break;
	}
	if (enemy1.hp <= 0) {
		scene = 3;
		enemy1.hp = 0;
	}
}

void e_body_damage() {

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_real_distribution<> rand(1.0, 1.5);

	erina.hp -= int(enemy1.atk * 2.0 *  rand(mt));
	if (erina.hp <= 0) {
		scene = 4;
		erina.hp = 0;
	}
}

void e_attack_damage() {

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_real_distribution<> rand(0.8, 1.4);

	erina.hp -= int(enemy1.atk * 1.5 * rand(mt));
	if (erina.hp <- 0) {
		scene = 4;
		erina.hp = 0;
	}
}


void stand_by() {
	static int n = 0;
	GLdouble i;

	switch (n) {
	case 0:
		for (i = -10.5; i < 10.5; i += 0.5) {
			stand_by1(i);
		}
		n = 1;
		break;
	case 1:
		for (i = -10.5; i < 10.5; i += 0.5) {
			stand_by2(i);
		}
		n = 2;
		break;
	case 2:
		for (i = -10.5; i < 10.5; i += 0.5) {
			stand_by3(i);
		}
		n = 3;
		break;
	case 3:
		for (i = -10.5; i < 10.5; i += 0.5) {
			stand_by2(i);
		}
		n = 0;
		break;
	}
}

void run() {
	static int n = 0;
	GLdouble i;

	switch (n) {
	case 0:
		for (i = -10.5; i < 10.5; i += 0.5) {
			run1(i);
		}
		n = 1;
		break;
	case 1:
		for (i = -10.5; i < 10.5; i += 0.5) {
			run2(i);
		}
		n = 2;
		break;
	case 2:
		for (i = -10.5; i < 10.5; i += 0.5) {
			run3(i);
		}
		n = 3;
		break;
	case 3:
		for (i = -10.5; i < 10.5; i += 0.5) {
			run2(i);
		}
		n = 0;
		break;
	}
}

void atk_combo1() {//右に移動しているとき
	static int n = 0;
	GLdouble i;

	switch (n) {
	case 0:
		for (i = -15.0; i < 15.0; i += 0.5) {
			attack1(i);
		}
		n = 1;
		break;
	case 1:
		for (i = -15.0; i < 15.0; i += 0.5) {
			attack2(i);
		}
		atk2start = true;
		atk1 = false;
		n = 0;
		break;
	}
}

void atk_combo2() {
	static int n = 0;
	GLdouble i;

	switch (n) {
	case 0:
		for (i = -15.0; i < 15.0; i += 0.5) {
			attack3(i);
		}
		n = 1;
		break;
	case 1:
		for (i = -15.0; i < 15.0; i += 0.5) {
			attack4(i);
		}
		atk2 = false;
		n = 0;
		break;
	}
}

void create_enemy() {
	GLdouble i;
	GLdouble j;

	for (i = 0.0 - moveX + x + enemyX ; i < 5.0 - moveX + x + enemyX; i += 0.5) {
		for (j = 0.0 - moveZ + enemyZ; j < 8.0 - moveZ + enemyZ; j += 0.5) {
			glPushMatrix();
			glColor3d(1.0, 0, 0);
			glTranslated(i, 0.0, j);
			glutSolidCube(0.4999);
			glPopMatrix();
		}
	}
}

void create_beam(float n) {

	glBegin(GL_LINE_LOOP);
	glColor3d(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3d(-1 + n - moveX + x, 1, -100 - moveZ );
	glVertex3d(1 + n - moveX + x, 1, -100 - moveZ);
	glVertex3d(1 + n - moveX + x, 1, 100 - moveZ);
	glVertex3d(-1 + n -moveX + x, 1, 100 - moveZ);
	glEnd();
	glFlush();
}

void create_floor() {
	GLdouble i;
	GLdouble j;

	for (i = 13.0 - moveX + x; i < 15.0 - moveX + x; i += 1.0) {
		for (j = -150.0 - moveZ; j < 150.0 - moveZ; j += 1.0) {
			glPushMatrix();
			glColor3d(.0, 0, 0);
			glTranslated(i, 0.0, j);
			glutSolidCube(0.9999);
			glPopMatrix();
		}
	}
}
//色を指定してcubeを置く
void ear_outside(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(1.0, 0.84, 0.84);//耳の外側のピンクっぽいところ
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void hair_shadow(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.81, 0.56, 1.0);//髪の影の部分
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void hair(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.86, 0.79, 1.0);//髪の影じゃない部分
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void bunny_suit_shadow(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.45, 0.26, 1.0);//バニースーツの影
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void bunny_suit(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.72, 0.50, 1.0);//バニースーツ
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void skin_shadow_thin(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(1.0, 0.82, 0.73);//肌の部分
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void skin_shadow_thick(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.74, 0.6, 0.52);//肌の影の色
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void skin(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.98, 0.90, 0.86);//肌の部分
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void eyelash(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.39, 0.07, 0.9);//まつ毛とか
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void eye_thick(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.39, 0.07, 0.9);//目の濃いところ
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void eye(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.65, 0.46, 0.85);//目の真ん中の濃さのところ
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void eye_thin(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.86, 0.66, 1.0);//目の薄いところ
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void mouth(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.9, 0.72, 0.63);//口
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void shadow1(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.55, 0.40, 1.0);//ちょこっとの影(胸とか)
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void shadow2(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.63, 0.57, 0.80);//ちょこっとの影(手袋)
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void reg_shadow_thick(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.75, 0.60, 0.78);//ちょこっとの影(足らへん)
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void reg_shadow_thin(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.90, 0.79, 0.93);//ちょこっとの影(足らへんで薄いとこ)
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void toe(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.35, 0.23, 0.6);//つまさき
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void hammer_scarlet(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.96, 0.16, 0.16);//ハンマーの明るい赤
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void hammer_dark_red(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.49, 0.07, 0.06);//ハンマーの暗い赤
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void hammer_yellow(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.96, 0.88, 0.45);//ハンマーの黄色
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void hammer_gray(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.45, 0.49, 0.46);//ハンマーの黄色
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void hammer_effect(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.92, 0.62, 0.26);//ハンマーの黄色
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void black(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(0.3, 0.3, 0.3);//黒
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}
void white(GLdouble i, GLdouble j) {
	glPushMatrix();
	glColor3d(1.0, 1.0, 1.0);//白
	glTranslated(i, 0.0, j);
	glutSolidCube(0.4999);
	glPopMatrix();
}

//以下プレイヤーキャラクター描画
void stand_by1(GLdouble i) {
	GLdouble j;
	for (j = -10.0; j < 10.0; j += 0.5) {
		//上から下に置いていく
		if (i == -10.0) {
			if (j == 3.5 || j == 3.0 || j == -4.0 || j == -4.5) {
				black(i, j);
			}
		}
		if (i == -9.5) {
			if (j == 3.5 || j == 3.0 || j == -4.0 || j == -4.5) {
				ear_outside(i, j);
			}
			if (j == 4.0 || j == 2.5 || j == -3.5 || j == -5.0) {
				black(i, j);
			}
		}
		if (i == -9.0) {
			if (j == 3.5 || j == 3.0 || j == -4.0 || j == -4.5) {
				white(i, j);
			}
			if (j == 4.0 || j == 2.5 || j == -3.5 || j == -5.0) {
				ear_outside(i, j);
			}
			if (j == 4.5 || j == 2.0 || j == -3.0 || j == -5.5) {
				black(i, j);
			}
		}
		if (i == -8.5) {
			if (j <= 4.0 && j >= 2.5 || j <= -3.5 && j >= -5.0) {
				white(i, j);
			}
			if (j == 4.5 || j == 2.0 || j == -3.0 || j == -5.5) {
				ear_outside(i, j);
			}
			if (j == 5.0 || j == 1.5 || j == -2.5 || j == -6.0) {
				black(i, j);
			}
		}
		if (i == -8.0) {
			if (j <= 4.5 && j >= 2.0 || j <= -3.0 && j >= -5.5) {
				white(i, j);
			}
			if (j == 5.0 || j == 1.5 || j == -2.5 || j == -6.0) {
				ear_outside(i, j);
			}
			if (j == 5.5 || j <= 1.5 && j >= -2.5 || j == -6.5) {
				black(i, j);
			}
		}
		if (i == -7.5) {
			if (j <= 4.5 && j >= 2.0 || j <= -3.0 && j >= -5.5) {
				white(i, j);
			}
			if (j == 5.0 || j == 1.5 || j == -2.5 || j == -6.0) {
				ear_outside(i, j);
			}
			if (j <= 1.5 && j >= -2.5) {
				hair_shadow(i, j);
			}
			if (j == 5.5 || j == -6.5) {
				black(i, j);
			}
		}
		if (i == -7.0) {
			if (j <= 5.0 && j >= 2.5 || j <= -3.5 && j >= -6.0) {
				white(i, j);
			}
			if (j == 5.5 || j == 2.0 || j == -3.0 || j == -6.5) {
				ear_outside(i, j);
			}
			if (j <= 2.0 && j >= -3.0) {
				hair(i, j);
			}
			if (j == 6.0 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == -6.5) {
			if (j <= 5.5 && j >= 3.0 || j <= -4.0 && j >= -6.5) {
				white(i, j);
			}
			if (j == 6.0 || j == 2.5 || j == -3.5 || j == -7.0) {
				ear_outside(i, j);
			}
			if (j <= 2.5 && j >= -3.5) {
				hair(i, j);
			}
			if (j == 6.5 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == -6.0) {
			if (j <= 6.0 && j >= 3.5 || j <= -4.5 && j >= -7.0) {
				white(i, j);
			}
			if (j == 6.5 || j == 3.0 || j == -4.0 || j == -7.5) {
				ear_outside(i, j);
			}
			if (j <= 3.0 && j >= -4.0) {
				hair(i, j);
			}
			if (j == 7.0 || j == -8.0) {
				black(i, j);
			}
		}
		if (i == -5.5) {
			if (j <= 6.0 && j >= 3.5 || j <= -4.5 && j >= -7.0) {
				white(i, j);
			}
			if (j == 6.5 || j == 3.0 || j == -4.0 || j == -7.5) {
				ear_outside(i, j);
			}
			if (j <= 3.0 && j >= -4.0) {
				hair(i, j);
			}
			if (j == 7.0 || j == -8.0) {
				black(i, j);
			}
		}
		if (i == -5.0) {
			if (j <= 6.5 && j >= 4.0 || j <= -5.0 && j >= -7.5) {
				white(i, j);
			}
			if (j == 7.0 || j == 3.5 || j == -4.5 || j == -8.0) {
				ear_outside(i, j);
			}
			if (j <= 3.5 && j >= -4.0) {
				hair(i, j);
			}
			if (j == 7.5 || j == -8.5) {
				black(i, j);
			}
		}
		if (i == -4.5) {
			if (j == 0.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= 7.0 && j >= 4.5 || j <= -5.5 && j >= -8.0) {
				white(i, j);
			}
			if (j == 7.5 || j == 4.0 || j == -5.0 || j == -8.5) {
				ear_outside(i, j);
			}
			if (j <= 3.0 && j >= -4.5) {
				hair(i, j);
			}
			if (j == 3.5) {
				hair_shadow(i, j);
			}
			if (j == 8.0 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == -4.0) {
			if (j <= 7.0 && j >= 4.5 || j <= -5.5 && j >= -8.0) {
				white(i, j);
			}
			if (j == 7.5 || j == 4.0 || j == -5.0 || j == -8.5) {
				ear_outside(i, j);
			}
			if (j == 3.5 || j == 2.5 || j == 1.0 || j == 0.5 || j == -1.0 || j == -1.5 || j <= -2.5 && j >= -4.5) {
				hair(i, j);
			}
			if (j == 3.0 || j == -2.0) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == 1.5 || j == -0.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 0.0) {
				skin(i, j);
			}
			if (j == 8.0 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == -3.5) {
			if (j <= 7.5 && j >= 5.0 || j <= -6.0 && j >= -8.5) {
				white(i, j);
			}
			if (j == 8.0 || j == 4.5 || j == -5.5 || j == -9.0) {
				ear_outside(i, j);
			}
			if (j == 4.0 || j == 3.5 || j == 0.5 || j == -1.5 || j <= -2.5 && j >= -5.0) {
				hair(i, j);
			}
			if (j == 3.0 || j == -2.0) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == -1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.5 || j == 1.0 || j == 0.0 || j == -0.5) {
				skin(i, j);
			}
			if (j == 8.5 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == -3.0) {
			if (j <= 7.5 && j >= 5.0 || j <= -6.0 && j >= -8.5) {
				white(i, j);
			}
			if (j == 8.0 || j == 4.5 || j == -5.5 || j == -9.0) {
				ear_outside(i, j);
			}
			if (j <= 4.0 && j >= 3.0 || j == -3.5 || j == -4.0 || j == -5.0) {
				hair(i, j);
			}
			if (j == -4.5) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= -0.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 1.5 || j <= -1.0 && j >= -3.0) {
				eyelash(i, j);
			}
			if (j == 8.5 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == -2.5) {
			if (j <= 7.5 && j >= 5.5 || j == 1.5 || j == -2.0 || j == -2.5 || j <= -6.5 && j >= -8.5) {
				white(i, j);
			}
			if (j == 8.0 || j == 5.0 || j == -6.0 || j == -9.0) {
				ear_outside(i, j);
			}
			if (j <= 4.5 && j >= 3.0 || j <= -3.0 && j >= -4.0 || j == -5.0 || j == -5.5) {
				hair(i, j);
			}
			if (j == -4.5) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= -0.5) {
				skin(i, j);
			}
			if (j == -1.0 || j == -1.5) {
				eye_thick(i, j);
			}
			if (j == 2.0) {
				eye(i, j);
			}
			if (j == 8.5 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == -2.0) {
			if (j <= 8.0 && j >= 6.0 || j == -2.5 || j <= -7.0 && j >= -8.5) {
				white(i, j);
			}
			if (j == 8.5 || j == 5.5 || j == -6.5 || j == -9.0) {
				ear_outside(i, j);
			}
			if (j <= 4.5 && j >= 3.0 || j <= -3.0 && j >= -4.0 || j == -5.0 || j == -5.5) {
				hair(i, j);
			}
			if (j == -4.5) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= -0.5) {
				skin(i, j);
			}
			if (j == 1.5 || j == -1.5) {
				eye_thick(i, j);
			}
			if (j == 2.0 || j == -1.0 || j == -2.0) {
				eye(i, j);
			}
			if (j == 9.0 || j == 5.0 || j == -6.0 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == -1.5) {
			if (j <= 8.0 && j >= 6.5 || j == 2.5 || j == -2.5 || j <= -7.0 && j >= -8.5) {
				white(i, j);
			}
			if (j == 8.5 || j == 6.0 || j == 5.5 || j == -6.5 || j == -9.0) {
				ear_outside(i, j);
			}
			if (j <= 4.5 && j >= 3.5 || j <= -3.0 && j >= -4.0 || j == -5.0 || j == -5.5) {
				hair(i, j);
			}
			if (j == -4.5) {
				hair_shadow(i, j);
			}
			if (j == 3.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= -0.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 1.5 || j <= -1.0 && j >= -2.0) {
				eye(i, j);
			}
			if (j == 9.0 || j == 5.0 || j == -6.0 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == -1.0) {
			if (j <= 8.0 && j >= 6.5 || j <= -7.5 && j >= -8.0) {
				white(i, j);
			}
			if (j == 8.5 || j == 6.0 || j == -7.0 || j == -8.5) {
				ear_outside(i, j);
			}
			if (j <= 4.5 && j >= 3.5 || j <= -3.0 && j >= -4.0 || j == -5.0 || j == -5.5) {
				hair(i, j);
			}
			if (j == -4.5) {
				hair_shadow(i, j);
			}
			if (j == 3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.5 || j <= 1.0 && j >= -0.5 || j == -2.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 1.5 || j <= -1.0 && j >= -2.0) {
				eye_thin(i, j);
			}
			if (j == 9.0 || j == 5.5 || j == 5.0 || j == -6.0 || j == -6.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == -0.5) {
			if (j == 7.5 || j == 7.0) {
				white(i, j);
			}
			if (j == 8.0 || j == 6.5 || j == -7.5 || j == -8.0) {
				ear_outside(i, j);
			}
			if (j <= 4.5 && j >= 3.5 || j == -3.0 || j == -3.5 || j <= -4.5 && j >= -5.5) {
				hair(i, j);
			}
			if (j == -2.5 || j == -4.0) {
				hair_shadow(i, j);
			}
			if (j == 3.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.5 && j >= -2.0) {
				skin(i, j);
			}
			if (j == 2.0 || j == 1.5 || j <= -1.0 && j >= -2.0) {
				eye_thin(i, j);
			}
			if (j == 8.5 || j == 6.0 || j == 5.0 || j == -6.0 || j == -7.0 || j == -8.5) {
				black(i, j);
			}
		}
		if (i == 0.0) {
			if (j == 7.5) {
				white(i, j);
			}
			if (j == 8.0 || j == 7.0 || j == 6.5) {
				ear_outside(i, j);
			}
			if (j <= 4.5 && j >= 3.0 || j == -3.0 || j == -3.5 || j <= -5.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.5 || j == -4.0 || j == -4.5) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= -2.0) {
				skin(i, j);
			}
			if (j == 2.0 || j == 1.5 || j <= -1.0 && j >= -2.0) {
				eye_thin(i, j);
			}
			if (j == 8.5 || j == 6.0 || j == 5.0 || j == -6.5 || j == -7.5 || j == -8.0) {
				black(i, j);
			}
		}
		if (i == 0.5) {
			if (j == 7.5 || j == 7.0) {
				ear_outside(i, j);
			}
			if (j <= 4.5 && j >= 2.5 || j == -3.0 || j == -3.5 || j <= -4.5 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.5 || j == -4.0) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == 1.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= 0.0 || j <= -1.0 && j >= -2.0) {
				skin(i, j);
			}
			if (j == -0.5) {
				mouth(i, j);//口
			}
			if (j == 8.0 || j == 6.5 || j == 5.0 || j == -6.5) {
				black(i, j);
			}
		}
		if (i == 1.0) {
			if (j <= 4.5 && j >= 1.5 || j == -3.0 || j == -3.5 || j <= -4.5 && j >= -6.0) {
				hair(i, j);
			}
			if (j == 5.0 || j == -2.5 || j == -4.0) {
				hair_shadow(i, j);
			}
			if (j <= 1.0 && j >= -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.5 || j == -2.0) {
				skin(i, j);
			}
			if (j == 7.5 || j == 7.0 || j == 5.5 || j == -6.5) {
				black(i, j);
			}
		}
		if (i == 1.5) {
			if (j <= 4.5 && j >= 2.5 || j <= -4.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == 5.0 || j == 2.0 || j == 1.5 || j <= -2.5 && j >= -3.5) {
				hair_shadow(i, j);
			}
			if (j == 1.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 0.5 && j >= -1.0) {
				skin(i, j);
			}
			if (j == -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 7.5 || j == 7.0 || j == 5.5 || j == -6.5) {
				black(i, j);
			}
		}
		if (i == 2.0) {
			if (j <= 4.5 && j >= 2.5 || j <= -4.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == 5.0 || j == 2.0 || j == -3.0 || j == -3.5 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 1.5 || j == -1.5 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				skin(i, j);
			}
			if (j == 1.0 || j == -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 0.5 || j == -1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 5.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == 2.5) {
			if (j <= 4.5 && j >= 2.5 || j <= -4.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == 5.5 || j == 5.0 || j == -3.5 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.5 || j == -2.0 || j == -2.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.0 || j == 0.0 || j == -0.5 || j == -1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 0.5 && j >= -1.0) {
				bunny_suit(i, j);
			}
			if (j == 6.0 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == 3.0) {
			if (j <= 4.5 && j >= 2.5 || j <= -3.5 && j >= -6.0) {
				hair(i, j);
			}
			if (j == 5.5 || j == 5.0 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.5) {
				skin_shadow_thin(i, j);
			}
			if (j == -2.0 || j == -2.5) {
				skin(i, j);
			}
			if (j == 1.0 || j == -1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 0.5 && j >= -1.0) {
				bunny_suit(i, j);
			}
			if (j == 6.0 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == 3.5) {
			if (j <= 4.5 && j >= 2.5 || j <= -3.5 && j >= -6.0) {
				hair(i, j);
			}
			if (j == 5.5 || j == 5.0 || j == -6.5 || j == -7.0) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.5 || j == -2.5) {
				skin(i, j);
			}
			if (j == 1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				bunny_suit(i, j);
			}
			if (j == 0.5 || j == -1.0 || j == -1.5) {
				shadow1(i, j);
			}
			if (j == -3.0) {
				shadow2(i, j);
			}
			if (j == 6.0 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == 4.0) {
			if (j <= 4.5 && j >= 3.0 || j <= -4.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == 5.5 || j == 5.0 || j == -6.5 || j == -7.0) {
				hair_shadow(i, j);
			}
			if (j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -2.5) {
				skin(i, j);
			}
			if (j == 1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 0.5 && j >= -1.5) {
				bunny_suit(i, j);
			}
			if (j == 2.5 || j == 2.0 || j == -3.5) {
				shadow2(i, j);
			}
			if (j == 6.0 || j == -7.5) {
				black(i, j);
			}
			if (j == 1.5 || j == -3.0) {
				white(i, j);
			}
		}
		if (i == 4.5) {
			if (j <= 4.5 && j >= 3.0 || j <= -4.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j <= 6.0 && j >= 5.0 || j <= -6.5 && j >= -7.5) {
				hair_shadow(i, j);
			}
			if (j == 1.0 || j == -2.0) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 0.5 && j >= -1.5) {
				bunny_suit(i, j);
			}
			if (j == 2.5 || j == -3.5) {
				shadow2(i, j);
			}
			if (j == 6.5 || j == -8.0) {
				black(i, j);
			}
			if (j == 2.0 || j == 1.5 || j == -2.5 || j == -3.0) {
				white(i, j);
			}
		}
		if (i == 5.0) {
			if (j == 4.5 || j == 4.0 || j == 3.5 || j <= -4.5 && j >= -6.0) {
				hair(i, j);
			}
			if (j == 6.0 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 2.5 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0 || j == -2.0 || j == -4.0) {
				skin_shadow_thin(i, j);
			}
			if (j == -3.5) {
				skin(i, j);
			}
			if (j <= 0.0 && j >= -1.0) {
				bunny_suit(i, j);
			}
			if (j == 0.5 || j == -1.5) {
				shadow1(i, j);
			}
			if (j == 2.0 || j == -3.0) {
				shadow2(i, j);
			}
			if (j == 6.5 || j == 5.5 || j == 5.0 || j == 3.0 || j == -7.0 || j == -7.5) {
				black(i, j);
			}
			if (j == 1.5 || j == -2.5 || j == -3.0) {
				white(i, j);
			}
		}
		if (i == 5.5) {
			if (j == 4.0 || j <= -4.5 && j >= -6.0) {
				hair(i, j);
			}
			if (j == 2.5 || j == 1.5 || j == 0.5 || j == -1.5 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0 || j == -2.0 || j == -3.5 || j == -4.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 2.0 || j == -3.0) {
				skin(i, j);
			}
			if (j == 0.0 || j == -1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == -0.5) {
				bunny_suit(i, j);
			}
			if (j == -3.0) {
				shadow2(i, j);
			}
			if (j == 6.0 || j == 4.5 || j == 3.5 || j == 3.0 || j == -6.5) {
				black(i, j);
			}
		}
		if (i == 6.0) {
			if (j == -4.0) {
				hair(i, j);
			}
			if (j == 2.0 || j == - 1.0 || j == -2.5 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5 || j == -1.5 || j == -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == -0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 4.0 || j == 2.5 || j == -3.0 || j == -3.5 || j <= -4.5 && j >= -6.0) {
				black(i, j);
			}
		}
		if (i == 6.5) {
			if (j == -1.0 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.5 || j == -2.0) {
				skin(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -3.0 || j == -4.0) {
				black(i, j);
			}
		}
		if (i == 7.0) {
			if (j == -1.0 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.5 || j == -2.0) {
				skin(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				white(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -3.0) {
				black(i, j);
			}
		}
		if (i == 7.5) {
			if (j == -1.0 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.5 || j == -2.0) {
				skin(i, j);
			}
			if (j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.5 || j == 0.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 1.0) {
				white(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -3.0) {
				black(i, j);
			}
		}
		if (i == 8.0) {
			if (j == -1.0 || j == -1.5 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -2.0 || j == -2.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 0.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				white(i, j);
			}
			if (j == 2.5 || j == 0.0 || j == -0.5 || j == -3.5) {
				black(i, j);
			}
		}
		if (i == 8.5) {
			if (j == -1.5 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -2.0 || j == -2.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 0.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				white(i, j);
			}
			if (j == 2.5 || j == 0.0 || j == -1.0 || j == -3.5) {
				black(i, j);
			}
		}
		if (i == 9.0) {
			if (j == -1.5 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -2.0 || j == -2.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 0.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				white(i, j);
			}
			if (j == 2.5 || j == 0.0 || j == -1.0 || j == -3.5) {
				black(i, j);
			}
		}
		if (i == 9.5) {
			if (j == 1.5 || j == 1.0 || j == -2.0 || j == -2.5) {
				toe(i, j);
			}
			if (j == 2.0 || j == 0.5 || j == -1.5 || j == -3.0) {
				black(i, j);
			}
		}
	}
}

void stand_by2(GLdouble i) {
	GLdouble j;
	for (j = -12.0; j < 12.0; j += 0.5) {
		//上から下に置いていく
		if (i == -10.5) {
			if (j == 3.5 || j == 3.0 || j == -4.0 || j == -4.5) {
				black(i, j);
			}
		}
		if (i == -10.0) {
			if (j == 3.5 || j == 3.0 || j == -4.0 || j == -4.5) {
				ear_outside(i, j);
			}
			if (j == 4.0 || j == 2.5 || j == -3.5 || j == -5.0) {
				black(i, j);
			}
		}
		if (i == -9.5) {
			if (j == 3.5 || j == 3.0 || j == -4.0 || j == -4.5) {
				white(i, j);
			}
			if (j == 4.0 || j == 2.5 || j == -3.5 || j == -5.0) {
				ear_outside(i, j);
			}
			if (j == 4.5 || j == 2.0 || j == -3.0 || j == -5.5) {
				black(i, j);
			}
		}
		if (i == -9.0) {
			if (j <= 4.0 && j >= 2.5 || j <= -3.5 && j >= -5.0) {
				white(i, j);
			}
			if (j == 4.5 || j == 2.0 || j == -3.0 || j == -5.5) {
				ear_outside(i, j);
			}
			if (j == 5.0 || j == 1.5 || j == -2.5 || j == -6.0) {
				black(i, j);
			}
		}
		if (i == -8.5) {
			if (j <= 4.5 && j >= 2.0 || j <= -3.0 && j >= -5.5) {
				white(i, j);
			}
			if (j == 5.0 || j == 1.5 || j == -2.5 || j == -6.0) {
				ear_outside(i, j);
			}
			if (j == 5.5 || j <= 1.5 && j >= -2.5 || j == -6.5) {
				black(i, j);
			}
		}
		if (i == -8.0) {
			if (j <= 5.0 && j >= 2.0 || j <= -3.0 && j >= -6.0) {
				white(i, j);
			}
			if (j == 5.5 || j == 1.5 || j == -2.5 || j == -6.5) {
				ear_outside(i, j);
			}
			if (j <= 1.5 && j >= -2.5) {
				hair_shadow(i, j);
			}
			if (j == 6.0 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == -7.5) {
			if (j <= 5.5 && j >= 2.5 || j <= -3.5 && j >= -6.5) {
				white(i, j);
			}
			if (j == 6.0 || j == 2.0 || j == -3.0 || j == -7.0) {
				ear_outside(i, j);
			}
			if (j <= 2.0 && j >= -3.0) {
				hair(i, j);
			}
			if (j == 6.5 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == -7.0) {
			if (j <= 6.0 && j >= 3.0 || j <= -4.0 && j >= -7.0) {
				white(i, j);
			}
			if (j == 6.5 || j == 2.5 || j == -3.5 || j == -7.5) {
				ear_outside(i, j);
			}
			if (j <= 2.5 && j >= -3.5) {
				hair(i, j);
			}
			if (j == 7.0 || j == -8.0) {
				black(i, j);
			}
		}
		if (i == -6.5) {
			if (j <= 6.5 && j >= 3.5 || j <= -4.5 && j >= -7.5) {
				white(i, j);
			}
			if (j == 7.0 || j == 3.0 || j == -4.0 || j == -8.0) {
				ear_outside(i, j);
			}
			if (j <= 3.0 && j >= -4.0) {
				hair(i, j);
			}
			if (j == 7.5 || j == -8.5) {
				black(i, j);
			}
		}
		if (i == -6.0) {
			if (j <= 7.0 && j >= 3.5 || j <= -4.5 && j >= -8.0) {
				white(i, j);
			}
			if (j == 7.5 || j == 3.0 || j == -4.0 || j == -8.5) {
				ear_outside(i, j);
			}
			if (j <= 3.0 && j >= -4.0) {
				hair(i, j);
			}
			if (j == 8.0 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == -5.5) {
			if (j <= 7.5 && j >= 4.0 || j <= -5.0 && j >= -8.5) {
				white(i, j);
			}
			if (j == 8.0 || j == 3.5 || j == -4.5 || j == -9.0) {
				ear_outside(i, j);
			}
			if (j <= 3.5 && j >= -4.0) {
				hair(i, j);
			}
			if (j == 9.0 || j == 8.5 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == -5.0) {
			if (j == 0.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= 8.0 && j >= 4.5 || j <= -5.5 && j >= -9.0) {
				white(i, j);
			}
			if (j == 9.0 || j == 8.5 || j == 4.0 || j == -5.0 || j == -9.5) {
				ear_outside(i, j);
			}
			if (j <= 3.0 && j >= -4.5) {
				hair(i, j);
			}
			if (j == 3.5) {
				hair_shadow(i, j);
			}
			if (j == 9.5 || j == -10.0 || j == -10.5) {
				black(i, j);
			}
		}
		if (i == -4.5) {
			if (j <= 9.0 && j >= 4.5 || j <= -5.5 && j >= -9.5) {
				white(i, j);
			}
			if (j == 9.5 || j == 4.0 || j == -5.0 || j == -10.0 || j == -10.5) {
				ear_outside(i, j);
			}
			if (j == 3.5 || j == 2.5 || j == 1.0 || j == 0.5 || j == -1.0 || j == -1.5 || j <= -2.5 && j >= -4.5) {
				hair(i, j);
			}
			if (j == 3.0 || j == -2.0) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == 1.5 || j == -0.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 0.0) {
				skin(i, j);
			}
			if (j == 10.0 || j == -11.0) {
				black(i, j);
			}
		}
		if (i == -4.0) {
			if (j <= 9.5 && j >= 5.0 || j <= -6.0 && j >= -10.5) {
				white(i, j);
			}
			if (j == 10.0 || j == 4.5 || j == -5.5 || j == -11.0) {
				ear_outside(i, j);
			}
			if (j == 4.0 || j == 3.5 || j == 0.5 || j == -1.5 || j <= -2.5 && j >= -5.0) {
				hair(i, j);
			}
			if (j == 3.0 || j == -2.0) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == -1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.5 || j == 1.0 || j == 0.0 || j == -0.5) {
				skin(i, j);
			}
			if (j == 10.5 || j == -11.5) {
				black(i, j);
			}
		}
		if (i == -3.5) {
			if (j <= 9.5 && j >= 5.5 || j <= -6.5 && j >= -11.0) {
				white(i, j);
			}
			if (j == 10.0 || j == 5.0 || j == -6.0 || j == -11.5) {
				ear_outside(i, j);
			}
			if (j <= 4.5 && j >= 3.0 || j == -3.5 || j == -4.0 || j == -5.0 || j == -5.5) {
				hair(i, j);
			}
			if (j == -4.5) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= -0.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 1.5 || j <= -1.0 && j >= -3.0) {
				eyelash(i, j);
			}
			if (j == 10.5 || j == -12.0) {
				black(i, j);
			}
		}
		if (i == -3.0) {
			if (j <= 9.0 && j >= 6.0 || j == 1.5 || j == -2.0 || j == -2.5 || j <= -7.0 && j >= -11.0) {
				white(i, j);
			}
			if (j == 9.5 || j == 5.5 || j == -6.5 || j == -11.5) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= 3.0 || j <= -3.0 && j >= -4.0 || j <= -5.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -4.5) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= -0.5) {
				skin(i, j);
			}
			if (j == -1.0 || j == -1.5) {
				eye_thick(i, j);
			}
			if (j == 2.0) {
				eye(i, j);
			}
			if (j == 10.0 || j == -12.0) {
				black(i, j);
			}
		}
		if (i == -2.5) {
			if (j <= 8.5 && j >= 6.5 || j == -2.5 || j <= -8.0 && j >= -10.5) {
				white(i, j);
			}
			if (j == 9.0 || j == 6.0 || j == -7.0 || j == -7.5 || j == -11.0) {
				ear_outside(i, j);
			}
			if (j <= 4.5 && j >= 3.0 || j <= -3.0 && j >= -4.0 || j == -5.0 || j == -5.5) {
				hair(i, j);
			}
			if (j == -4.5) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= -0.5) {
				skin(i, j);
			}
			if (j == 1.5 || j == -1.5) {
				eye_thick(i, j);
			}
			if (j == 2.0 || j == -1.0 || j == -2.0) {
				eye(i, j);
			}
			if (j == 9.5 || j == 5.5 || j == 5.0 || j == -6.0  || j == -6.5 || j == -11.5) {
				black(i, j);
			}
		}
		if (i == -2.0) {
			if (j == 2.5 || j == -2.5 || j <= -9.0 && j >= -10.0) {
				white(i, j);
			}
			if (j <= 8.5 && j >= 6.5 || j <= -7.5 && j >= -8.5 || j == -10.5) {
				ear_outside(i, j);
			}
			if (j <= 4.5 && j >= 3.5 || j <= -3.0 && j >= -4.0 || j == -5.0 || j == -5.5) {
				hair(i, j);
			}
			if (j == -4.5) {
				hair_shadow(i, j);
			}
			if (j == 3.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= -0.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 1.5 || j <= -1.0 && j >= -2.0) {
				eye(i, j);
			}
			if (j == 9.0 || j == 6.0 || j == 5.0 || j == -6.0 || j == -7.0|| j == -11.0) {
				black(i, j);
			}
		}
		if (i == -1.5) {
			if (j <= -9.0 && j >= -10.0) {
				ear_outside(i, j);
			}
			if (j <= 4.5 && j >= 3.5 || j <= -3.0 && j >= -4.0 || j == -5.0 || j == -5.5) {
				hair(i, j);
			}
			if (j == -4.5) {
				hair_shadow(i, j);
			}
			if (j == 3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.5 || j <= 1.0 && j >= -0.5 || j == -2.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 1.5 || j <= -1.0 && j >= -2.0) {
				eye_thin(i, j);
			}
			if (j <= 8.5 && j >= 6.5 || j == 5.0 || j == -6.0 || j <= -7.5 && j >= -8.5 || j == -10.5) {
				black(i, j);
			}
		}
		if (i == -1.0) {
			if (j <= 5.0 && j >= 4.0 || j <= -3.0 && j >= -4.0 || j == -5.0 || j == -5.5) {
				hair(i, j);
			}
			if (j == 3.5 || j == -2.5 || j == -4.5) {
				hair_shadow(i, j);
			}
			if (j == 3.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.5 && j >= -2.0) {
				skin(i, j);
			}
			if (j == 2.0 || j == 1.5 || j <= -1.0 && j >= -2.0) {
				eye_thin(i, j);
			}
			if (j == 5.5 || j == -6.0 || j <= -9.0 && j >= -10.0) {
				black(i, j);
			}
		}
		if (i == -0.5) {
			if (j <= 5.0 && j >= 4.0 || j == 3.0 || j <= -3.0 && j >= -4.0 || j <= -5.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == 3.5 || j == -2.5 || j == -4.5) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= -2.0) {
				skin(i, j);
			}
			if (j == 2.0 || j == 1.5 || j <= -1.0 && j >= -2.0) {
				eye_thin(i, j);
			}
			if (j == 5.5 || j == -6.5) {
				black(i, j);
			}
		}
		if (i == 0.0) {
			if (j == 5.0 || j == 4.5 || j == 3.5 || j == 3.0 || j == -3.5 || j == -4.0 || j <= -5.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == 4.0 || j == 2.5 || j == -2.5 || j == -3.0 || j == -4.5 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == 1.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= 0.0 || j <= -1.0 && j >= -2.0) {
				skin(i, j);
			}
			if (j == -0.5) {
				mouth(i, j);
			}
			if (j == 5.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == 0.5) {
			if (j == 5.0 || j <= 2.5 && j >= 1.5 || j == -3.0 || j == -4.0 ||  j == -4.5 || j <= -5.5 && j >= -6.5) {
				hair(i, j);
			}
			if (j == 5.5 || j <= 4.5  && j >= 3.0 || j == -2.5 || j == -3.5 || j == -5.0 || j == -7.0) {
				hair_shadow(i, j);
			}
			if (j <= 1.0 && j >= -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.5 || j == -2.0) {
				skin(i, j);
			}
			if (j == 6.0 || j == -7.5 || j == -8.0) {
				black(i, j);
			}
		}
		if (i == 1.0) {
			if (j <= 5.0 && j >= 2.5 || j == -3.0 || j == -3.5 || j == -4.5 || j == -5.0 || j == -6.0 || j == -6.5) {
				hair(i, j);
			}
			if (j == 6.0 || j == 5.5 || j == 2.0 || j == 1.5 || j == -2.5 || j == -4.0 || j == -5.5 || j <= -7.0 && j >= -8.0) {
				hair_shadow(i, j);
			}
			if (j == 1.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 0.5 && j >= -1.0) {
				skin(i, j);
			}
			if (j == -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 6.5 || j == -8.5) {
				black(i, j);
			}
		}
		if (i == 1.5) {
			if (j <= 5.0 && j >= 2.5 || j == -3.5 ||  j == -4.0 || j <= -5.5 && j >= -6.5) {
				hair(i, j);
			}
			if (j <= 6.5 && j >= 5.5 || j == 2.0 || j == -3.0 || j == -4.5 || j == -5.0 || j <= -7.0 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j == 1.5 || j == -1.5 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				skin(i, j);
			}
			if (j == 1.0 || j == -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 0.5 || j == -1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 7.5 || j == 7.0 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 2.0) {
			if (j == 7.5 || j == 7.0 || j <= 5.5 && j >= 2.5 || j <= -3.5 && j >= -6.5) {
				hair(i, j);
			}
			if (j == 6.5 || j == 6.0 || j == -7.0 || j <= -8.0 && j >= -9.0) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.5 || j == -2.0 || j == -2.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.0 || j == 0.0 || j == -0.5 || j == -1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 0.5 && j >= -1.0) {
				bunny_suit(i, j);
			}
			if (j <= 9.0 && j >= 8.0 || j == -7.5 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == 2.5) {
			if (j <= 9.0 && j >= 8.0 || j <= 5.5 && j >= 4.0 || j == 3.0 || j <= -4.0 && j >= -6.5) {
				hair(i, j);
			}
			if (j <= 7.5 && j >= 6.0 || j == -7.0 || j == -7.5 || j <= -8.5 && j >= -9.5) {
				hair_shadow(i, j);
			}
			if (j == 2.5 || j == -3.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.5 || j == -2.5 || j == -3.0) {
				skin(i, j);
			}
			if (j == 1.0 || j == -1.5 || j == -2.0) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 0.5 && j >= -1.0) {
				bunny_suit(i, j);
			}
			if (j == 9.5 || j == 3.5 || j == -8.0 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == 3.0) {
			if (j == 8.0 || j <= 6.0 && j >= 4.5 || j <= -5.0 && j >= -6.5) {
				hair(i, j);
			}
			if (j <= 7.5 && j >= 6.5 || j == -7.0 || j == -7.5 || j <= -9.0 && j >= -10.0) {
				hair_shadow(i, j);
			}
			if (j == -4.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0 || j == 1.5 || j == -3.0) {
				skin(i, j);
			}
			if (j == 1.0 || j == -2.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				bunny_suit(i, j);
			}
			if (j == 0.5 || j == -1.0 || j == -1.5) {
				shadow1(i, j);
			}
			if (j == 3.0 || j == 2.5 || j == -2.5 || j == -3.5) {
				shadow2(i, j);
			}
			if (j == 9.0 || j == 8.5 || j == 4.0 || j == 3.5 || j == -4.5 || j == -8.0 || j == -8.5 || j == -10.5) {
				black(i, j);
			}
		}
		if (i == 3.5) {
			if (j == 6.5 || j == 6.0 || j == -6.5 || j == -7.0) {
				hair(i, j);
			}
			if (j == 8.0 || j == 7.5 || j == -7.5 || j == -8.0) {
				hair_shadow(i, j);
			}
			if (j == 1.5|| j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0 || j == -2.0) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 0.5 && j >= -1.5) {
				bunny_suit(i, j);
			}
			if (j == 3.5 || j == -3.0 || j == -4.5) {
				shadow2(i, j);
			}
			if (j == 8.5 || j == 7.0 || j <= 5.5 && j >= 4.0 || j == 6.0 || j <= -5.0 && j >= -6.0 || j <= -8.5 && j >= -10.0) {
				black(i, j);
			}
			if (j <= 3.0 && j >= 1.5 || j == -3.5  || j == -4.0) {
				white(i, j);
			}
		}
		if (i == 4.0) {
			if (j == 6.5 || j == -8.0) {
				hair(i, j);
			}
			if (j == 8.5 || j == 8.0 || j == -8.5) {
				hair_shadow(i, j);
			}
			if (j == 4.0 || j == 3.5) {
				skin_shadow_thin(i, j);
			}
			if (j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0 || j == -2.0) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 0.5 && j >= -1.5) {
				bunny_suit(i, j);
			}
			if (j == 1.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 3.0 || j== 2.0 || j == -3.5 || j == -4.5) {
				shadow2(i, j);
			}
			if (j == 9.0 || j == 7.5 || j == 7.0 || j == 6.0 || j == 4.5 || j == -3.0 || j == -5.0 ||  j == -7.5 || j == -9.0) {
				black(i, j);
			}
			if (j == 2.5 || j == 1.5 || j == -4.0) {
				white(i, j);
			}
		}
		if (i == 4.5) {
			if (j == 3.0 || j == -2.5 || j == -3.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 4.5 || j == 1.0 || j == -2.0 || j == -5.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 4.0 || j == 3.5 || j == -4.0 || j == -4.5) {
				skin(i, j);
			}
			if (j <= 0.0 && j >= -1.0) {
				bunny_suit(i, j);
			}
			if (j == 1.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 0.5 || j == -1.5) {
				shadow1(i, j);
			}
			if (j == 2.5) {
				shadow2(i, j);
			}
			if (j == 8.5 || j == 8.0 || j == 6.5 || j == 5.0 || j == 2.0 ||j == -3.0 || j == -5.5 || j == -8.0 || j == -8.5) {
				black(i, j);
			}
			if (j == -2.5) {
				white(i, j);
			}
		}
		if (i == 5.0) {
			if (j == 3.5 || j == 0.5 || j == -1.5 || j == -2.5 || j == -4.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 4.0 || j == 1.0 || j == -2.0 || j == -4.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 0.0 || j == -1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == -0.5) {
				bunny_suit(i, j);
			}
			if (j == 1.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 4.5 || j <= 3.0 && j >= 2.0 || j == 1.5 || j == -3.0 || j == -3.5 || j == -5.0) {
				black(i, j);
			}
		}
		if (i == 5.5) {
			if (j == -1.0 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5 || j == -1.5 || j == -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == -0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 4.0 || j == 3.5 || j == 2.0 || j == -3.0 || j == -4.0 || j == -4.5) {
				black(i, j);
			}
		}
		if (i == 6.0) {
			if (j == -1.0 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.5 || j == -2.0) {
				skin(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -3.0 || j == -4.0) {
				black(i, j);
			}
		}
		if (i == 6.5) {
			if (j == -1.0 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.5 || j == -2.0) {
				skin(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				white(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -3.0) {
				black(i, j);
			}
		}
		if (i == 7.0) {
			if (j == -1.0 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.5 || j == -2.0) {
				skin(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				white(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -3.0) {
				black(i, j);
			}
		}
		if (i == 7.5) {
			if (j == -1.0 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.5 || j == -2.0) {
				skin(i, j);
			}
			if (j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.5 || j == 0.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 1.0) {
				white(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -3.0) {
				black(i, j);
			}
		}
		if (i == 8.0) {
			if (j == -1.0 || j == -1.5 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -2.0 || j == -2.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 0.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				white(i, j);
			}
			if (j == 2.5 || j == 0.0 || j == -0.5 || j == -3.5) {
				black(i, j);
			}
		}
		if (i == 8.5) {
			if (j == -1.5 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -2.0 || j == -2.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 0.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				white(i, j);
			}
			if (j == 2.5 || j == 0.0 || j == -1.0 || j == -3.5) {
				black(i, j);
			}
		}
		if (i == 9.0) {
			if (j == -1.5 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -2.0) {
				skin(i, j);
			}
			if (j == 2.0 || j == 0.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 1.0) {
				white(i, j);
			}
			if (j == 1.5 || j == -2.5) {
				toe(i, j);
			}
			if (j == 2.5 || j == 0.0 || j == -1.0 || j == -3.5) {
				black(i, j);
			}
		}
		if (i == 9.5) {
			if (j == 1.5 || j == 1.0 || j == -2.0 || j == -2.5) {
				toe(i, j);
			}
			if (j == 2.0 || j == 0.5 || j == -1.5 || j == -3.0) {
				black(i, j);
			}
		}
		if (i == 10.0) {
			if (j == 1.5 || j == 1.0 || j == -2.0 || j == -2.5) {
				black(i, j);
			}
		}
	}
}

void stand_by3(GLdouble i) {
	GLdouble j;
	for (j = -15.0; j < 15.0; j += 0.5) {
		//上から下に置いていく
		if (i == -10.5) {
			if (j == 3.5 || j == 3.0 || j == -4.0 || j == -4.5) {
				black(i, j);
			}
		}
		if (i == -10.0) {
			if (j == 3.5 || j == 3.0 || j == -4.0 || j == -4.5) {
				ear_outside(i, j);
			}
			if (j == 4.0 || j == 2.5 || j == -3.5 || j == -5.0 || j == -5.5) {
				black(i, j);
			}
		}
		if (i == -9.5) {
			if (j == 3.5 || j == 3.0 || j == -4.0 || j == -4.5) {
				white(i, j);
			}
			if (j == 4.0 || j == 2.5 || j == -3.5 || j == -5.0 || j == -5.5) {
				ear_outside(i, j);
			}
			if (j == 5.0 || j == 4.5 || j == 2.0 || j == -3.0 || j == -6.0) {
				black(i, j);
			}
		}
		if (i == -9.0) {
			if (j <= 4.0 && j >= 2.5 || j <= -3.5 && j >= -5.5) {
				white(i, j);
			}
			if (j == 5.0 || j== 4.5 || j == 2.0 || j == -3.0 || j == -6.0) {
				ear_outside(i, j);
			}
			if (j == 5.5 || j == 1.5 || j == -2.5 || j == -6.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == -8.5) {
			if (j <= 5.0 && j >= 2.0 || j <= -3.0 && j >= -6.0) {
				white(i, j);
			}
			if (j == 5.5 || j == 1.5 || j == -2.5 || j == -6.5 || j == -7.0) {
				ear_outside(i, j);
			}
			if (j == 6.0 || j <= 1.5 && j >= -2.5 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == -8.0) {
			if (j <= 5.5 && j >= 2.0 || j <= -3.0 && j >= -7.0) {
				white(i, j);
			}
			if (j == 6.0 || j == 1.5 || j == -2.5 || j == -7.5) {
				ear_outside(i, j);
			}
			if (j <= 1.5 && j >= -2.5) {
				hair_shadow(i, j);
			}
			if (j == 7.0 || j == 6.5 || j == -8.0) {
				black(i, j);
			}
		}
		if (i == -7.5) {
			if (j <= 6.0 && j >= 3.0 || j <= -4.5 && j >= -7.5) {
				white(i, j);
			}
			if (j == 7.0 || j == 6.5 || j == 2.0 || j == 2.5 || j <= -3.0 && j >= -4.0 || j == -8.0) {
				ear_outside(i, j);
			}
			if (j == 1.5) {
				hair_shadow(i, j);
			}
			if (j <= 1.0 && j >= -2.5) {
				hair(i, j);
			}
			if (j == 7.5 || j == -8.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == -7.0) {
			if (j <= 7.0 && j >= 4.0 || j <= -5.5 && j >= -8.0) {
				white(i, j);
			}
			if (j == 7.5 || j == 3.5 || j == 3.0 || j <= -4.0 && j >= -5.0 || j == -8.5 || j == -9.0) {
				ear_outside(i, j);
			}
			if (j <= 2.5 && j >= -3.5) {
				hair(i, j);
			}
			if (j <= 9.0 && j >= 8.0 || j == -9.5 || j== -10.0) {
				black(i, j);
			}
		}
		if (i == -6.5) {
			if (j <= 7.5 && j >= 4.5 || j <= -6.0 && j >= -9.0) {
				white(i, j);
			}
			if (j <= 9.0 && j >= 8.0 || j == 4.0 || j == 3.5 || j == -5.0 || j == -5.5 || j == -9.5 || j == -10.0) {
				ear_outside(i, j);
			}
			if (j <= 3.0 && j >= -4.5) {
				hair(i, j);
			}
			if (j == 9.5 || j == -10.5 || j == -11.0) {
				black(i, j);
			}
		}
		if (i == -6.0) {
			if (j <= 9.0 && j >= 5.0 || j <= -7.5 && j >= -10.0) {
				white(i, j);
			}
			if (j == 9.5 || j == 4.5 || j <= -6.0 && j >= -7.0 || j == -9.0 || j == -10.5 || j == -11.0) {
				ear_outside(i, j);
			}
			if (j <= 4.0 && j >= -5.0) {
				hair(i, j);
			}
			if (j == 10.0 || j == -5.5 || j == -11.5) {
				black(i, j);
			}
		}
		if (i == -5.5) {
			if (j <= 9.5 && j >= 5.5 || j <= -8.0 && j >= -11.0) {
				white(i, j);
			}
			if (j == 10.0 || j == 5.0 || j == -7.5 || j == -8.0 || j == -11.5) {
				ear_outside(i, j);
			}
			if (j <= 4.5 && j >= -5.5) {
				hair(i, j);
			}
			if (j == 10.5 || j <= -6.0 && j >= -7.0 || j == -12.0) {
				black(i, j);
			}
		}
		if (i == -5.0) {
			if (j == 0.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= 10.0 && j >= 6.0 || j <= -8.5 && j >= -11.5) {
				white(i, j);
			}
			if (j == 10.5 || j == 5.5 || j == -8.0 || j == -12.0) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= -5.5) {
				hair(i, j);
			}
			if (j == 3.5) {
				hair_shadow(i, j);
			}
			if (j == 11.0 || j == -6.0 || j == -7.5 || j == -12.5) {
				black(i, j);
			}
		}
		if (i == -4.5) {
			if (j <= 10.5 && j >= 7.0 || j <= -9.0 && j >= -12.0) {
				white(i, j);
			}
			if (j == 11.0 || j == 6.5 ||  j == 6.0 || j == -8.5 || j == -10.0 || j == -12.5) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= 3.5 || j == 2.5 || j == 1.0 || j == 0.5 || j == -1.0 || j == -1.5 || j <= -2.5 && j >= -5.5) {
				hair(i, j);
			}
			if (j == 3.0 || j == -2.0) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == 1.5 || j == -0.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 0.0) {
				skin(i, j);
			}
			if (j == 11.5 ||j == 5.5 || j == -6.0 || j == -8.0 || j == -13.0) {
				black(i, j);
			}
		}
		if (i == -4.0) {
			if (j <= 10.0 && j >= 7.5 || j <= -10.0 && j >= -11.5) {
				white(i, j);
			}
			if (j == 11.0 || j == 10.5 || j == 7.0 || j == -9.0 || j == -9.5 || j == -12.0) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= 3.5 || j == 0.5 || j == -1.5 || j <= -2.5 && j >= -5.5) {
				hair(i, j);
			}
			if (j == 3.0 || j == -2.0) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == -1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.5 || j == 1.0 || j == 0.0 || j == -0.5) {
				skin(i, j);
			}
			if (j == 11.5 || j <= 6.5 && j >= 5.5 || j == -6.0 || j == -8.5 || j == -12.5) {
				black(i, j);
			}
		}
		if (i == -3.5) {
			if (j <= 9.0 && j >= 8.0) {
				white(i, j);
			}
			if (j == 10.0 || j == 9.5 || j == 7.5 || j <= -10.0 &&j >= -11.5) {
				ear_outside(i, j);
			}
			if (j <= 4.5 && j >= 3.0 || j <= -3.5 && j >= -5.5) {
				hair(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= -0.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 1.5 || j <= -1.0 && j >= -3.0) {
				eyelash(i, j);
			}
			if (j == 11.0 || j == 10.5 || j == 7.0 || j == 5.0 || j == -6.0 || j == -9.0 || j == -9.5 || j == -12.0) {
				black(i, j);
			}
		}
		if (i == -3.0) {
			if (j == 1.5 || j == -2.0 || j == -2.5) {
				white(i, j);
			}
			if (j <= 9.0 && j >= 8.0) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= 3.0 || j <= -3.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= -0.5) {
				skin(i, j);
			}
			if (j == -1.0 || j == -1.5) {
				eye_thick(i, j);
			}
			if (j == 2.0) {
				eye(i, j);
			}
			if (j == 9.5 || j == 7.5 || j == 5.5 || j == -6.5 || j <= -10.0 && j >= -11.5) {
				black(i, j);
			}
		}
		if (i == -2.5) {
			if (j == -2.5) {
				white(i, j);
			}
			if (j <= 5.5 && j >= 3.0 || j <= -3.0 && j >= -6.5 || j == -10.0) {
				hair(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= -0.5) {
				skin(i, j);
			}
			if (j == 1.5 || j == -1.5) {
				eye_thick(i, j);
			}
			if (j == 2.0 || j == -1.0 || j == -2.0) {
				eye(i, j);
			}
			if (j <= 9.0 && j >= 8.0 || j == 6.0 || j == -7.0 || j == -9.0 || j == -9.5 || j == -10.5) {
				black(i, j);
			}
		}
		if (i == -2.0) {
			if (j == 2.5 || j == -2.5) {
				white(i, j);
			}
			if (j <= 5.5 && j >= 3.5 || j <= -3.0 && j >= -7.0 || j == -9.0 || j == -9.5) {
				hair(i, j);
			}
			if (j == 3.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= -0.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 1.5 || j <= -1.0 && j >= -2.0) {
				eye(i, j);
			}
			if (j <= 8.5 && j >= 6.0 || j == 5.0 || j == -7.0 || j <= -7.5 && j >= -9.0 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == -1.5) {
			if (j <= 8.5 && j >= 3.5 || j <= -3.0 && j >= -4.0 || j <= -5.5 && j >= -8.5) {
				hair(i, j);
			}
			if (j == -4.5 ||j == -5.0) {
				hair_shadow(i, j);
			}
			if (j == 3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.5 || j <= 1.0 && j >= -0.5 || j == -2.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 1.5 || j <= -1.0 && j >= -2.0) {
				eye_thin(i, j);
			}
			if (j == 9.0 || j <= -6.0 && j >= 7.0 || j == -9.0 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == -1.0) {
			if (j == 9.0 || j <= 6.0 && j >= 4.0 || j <= -3.0 && j >= -5.0 || j <= -6.0 && j >= -7.0) {
				hair(i, j);
			}
			if (j == 3.5 || j == -2.5 || j == -5.5) {
				hair_shadow(i, j);
			}
			if (j == 3.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.5 && j >= -2.0) {
				skin(i, j);
			}
			if (j == 2.0 || j == 1.5 || j <= -1.0 && j >= -2.0) {
				eye_thin(i, j);
			}
			if (j == 9.5 || j <= 8.5 && j >= 6.5 || j == 5.5 || j <= -7.5 && j >= -8.5) {
				black(i, j);
			}
		}
		if (i == -0.5) {
			if (j <= 8.5 && j >= 4.5 || j == 3.5 || j == 3.0 || j <= -4.0 && j >= -5.5 || j == -6.5) {
				hair(i, j);
			}
			if (j == 4.0 || j <= -2.5 && j >= -3.5 || j == -6.0) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= -2.0) {
				skin(i, j);
			}
			if (j == 2.0 || j == 1.5 || j <= -1.0 && j >= -2.0) {
				eye_thin(i, j);
			}
			if (j  == 9.0 || j == -7.0|| j == -10.5 || j == -11.0) {
				black(i, j);
			}
		}
		if (i == 0.0) {
			if (j <= 8.0 && j >= 5.5 || j <= 4.0 && j >= 3.0 || j == -3.0 || j == -3.5 || j == -5.0 || j <= -5.5 && j >= -6.0) {
				hair(i, j);
			}
			if (j == 5.0 || j == 4.5 || j == 2.5 || j == -2.5 || j == -4.0 || j == -4.5 || j == -6.5 || j == -7.0 || j == -10.5 || j == -11.0) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == 1.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= 0.0 || j <= -1.0 && j >= -2.0) {
				skin(i, j);
			}
			if (j == -0.5) {
				mouth(i, j);
			}
			if (j == 8.5 || j == -7.5 || j == -9.5 || j == -10.0 || j == -11.5) {
				black(i, j);
			}
		}
		if (i == 0.5) {
			if (j <= 7.0 && j >= 6.0 || j <= 5.0 && j >= 3.5 || j <= 2.5 && j >= 1.5 || j <= -3.0 && j >= -4.5 || j <= -6.5 && j >= -7.5) {
				hair(i, j);
			}
			if (j == 5.5 || j == 3.0 || j == -2.5 || j <= -5.0 && j >= -6.0 || j <= -9.5 && j >= -10.5) {
				hair_shadow(i, j);
			}
			if (j <= 1.0 && j >= -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.5 || j == -2.0) {
				skin(i, j);
			}
			if (j <= 9.0 && j >= 7.5 || j <= -8.0 && j >= -9.0 || j == -11.0) {
				black(i, j);
			}
		}
		if (i == 1.0) {
			if (j <= 9.0 && j >= 5.5 ||j == 3.0 || j == 2.5 || j <= -3.0 && j >= -8.0) {
				hair(i, j);
			}
			if (j <= 5.0 && j >= 3.5 || j == 2.0 || j == 1.5 || j == -2.5 || j <= -8.5 && j >= -10.0) {
				hair_shadow(i, j);
			}
			if (j == 1.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 0.5 && j >= -1.0) {
				skin(i, j);
			}
			if (j == -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 9.5 || j == -10.5) {
				black(i, j);
			}
		}
		if (i == 1.5) {
			if (j <= 9.5 && j >= 8.0 ||j <= 7.0 && j >= 2.5 || j <= -3.5 && j >= -7.5 || j <= -5.5 && j >= -6.5) {
				hair(i, j);
			}
			if (j == 7.5 || j == 2.0 || j == -3.0 || j == -4.5 || j == -5.0 || j <= -8.0 && j >= -10.5) {
				hair_shadow(i, j);
			}
			if (j == 1.5 || j == -1.5 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				skin(i, j);
			}
			if (j == 1.0 || j == -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 0.5 || j == -1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 10.0 || j == -11.0) {
				black(i, j);
			}
		}
		if (i == 2.0) {
			if (j <= 6.5 && j >= 2.5 || j <= -5.5 && j >= -7.0) {
				hair(i, j);
			}
			if (j <= 9.5 && j >= 7.0 || j <= -3.5 && j >= -5.0 || j == -7.0 || j <= -7.5 && j >= -9.5) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.5 || j == -2.0 || j == -2.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.0 || j == 0.0 || j == -0.5 || j == -1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 0.5 && j >= -1.0) {
				bunny_suit(i, j);
			}
			if (j == 10.0 || j == -10.0 || j == -10.5) {
				black(i, j);
			}
		}
		if (i == 2.5) {
			if (j == 6.5 || j <= -4.0 && j >= -6.5) {
				hair(i, j);
			}
			if (j <= 8.5 && j >= 7.0 || j == 2.5 || j == -3.5) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == -3.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.5 ||  j== -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == -2.5 || j == -3.0) {
				skin(i, j);
			}
			if (j == 1.0 || j == -1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 0.5 && j >= -1.0) {
				bunny_suit(i, j);
			}
			if (j == 9.5 || j == 9.0 || j <= 6.0 && j >= 3.0 || j <= -4.0 && j >= -9.5) {
				black(i, j);
			}
		}
		if (i == 3.0) {
			if (j == 2.0 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0 || j == 1.5 || j == -3.0) {
				skin(i, j);
			}
			if (j == 1.0 || j == -2.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				bunny_suit(i, j);
			}
			if (j == 0.5 || j == -1.0 || j == -1.5) {
				shadow1(i, j);
			}
			if (j == -3.5) {
				shadow2(i, j);
			}
			if (j <= 8.5 && j >= 6.5 || j == 2.5 || j <= -4.0 && j >= -6.5) {
				black(i, j);
			}
		}
		if (i == 3.5) {
			if (j == -2.5) {
				skin_shadow_thick(i, j);
			}
			
			if (j == 1.0 || j == -2.0) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 0.5 && j >= -1.5) {
				bunny_suit(i, j);
			}
			if (j == 2.5 || j == 2.0 || j == -3.5) {
				shadow2(i, j);
			}
			if (j == 3.0 || j == -4.0) {
				black(i, j);
			}
			if (j == 1.5 || j == -3.0) {
				white(i, j);
			}
		}
		if (i == 4.0) {
			if (j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0 || j == -2.0) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 0.5 && j >= -1.5) {
				bunny_suit(i, j);
			}
			if (j == 2.5 || j == -4.0) {
				shadow2(i, j);
			}
			if (j == 3.0 || j == -4.5) {
				black(i, j);
			}
			if (j == 2.0 || j == 1.5 || j == -3.0 || j == -3.5) {
				white(i, j);
			}
		}
		if (i == 4.5) {
			if (j == 2.5 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0 || j == -2.0 || j == -2.5 || j == -4.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= 0.0 && j >= -1.0) {
				bunny_suit(i, j);
			}
			if (j == 0.5 || j == -1.5) {
				shadow1(i, j);
			}
			if (j == 2.0 || j == -3.5) {
				shadow2(i, j);
			}
			if (j == 3.0 || j == -4.5) {
				black(i, j);
			}
			if (j == 1.5 || j == -2.5) {
				white(i, j);
			}
		}
		if (i == 5.0) {
			if (j == 2.5 || j == 1.5 || j == 0.5 || j == -1.5 || j == -3.0 || j == -3.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0 || j == 1.0 || j == -2.0 || j == -2.5 || j == -4.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 0.0 || j == -1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == -0.5) {
				bunny_suit(i, j);
			}
			if (j == 1.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 3.0 || j == -4.5) {
				black(i, j);
			}
		}
		if (i == 5.5) {
			if (j == 2.0 || j == -1.0 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5 || j == -1.5 || j == -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == -0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 2.5 || j <= -3.0 && j >= -4.0) {
				black(i, j);
			}
		}
		if (i == 6.0) {
			if (j == -1.0 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.5 || j == -2.0) {
				skin(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -3.0) {
				black(i, j);
			}
		}
		if (i == 6.5) {
			if (j == -1.0 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.5 || j == -2.0) {
				skin(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				white(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -3.0) {
				black(i, j);
			}
		}
		if (i == 7.0) {
			if (j == -1.0 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.5 || j == -2.0) {
				skin(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				white(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -3.0) {
				black(i, j);
			}
		}
		if (i == 7.5) {
			if (j == -1.0 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.5 || j == -2.0) {
				skin(i, j);
			}
			if (j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.5 || j == 0.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 1.0) {
				white(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -3.0) {
				black(i, j);
			}
		}
		if (i == 8.0) {
			if (j == -1.5 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -2.0 || j == -2.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 0.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				white(i, j);
			}
			if (j == 2.5 || j == 0.0 || j == -0.5 || j == -1.0 || j == -3.5) {
				black(i, j);
			}
		}
		if (i == 8.5) {
			if (j == -1.5 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -2.0 || j == -2.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 0.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				white(i, j);
			}
			if (j == 2.5 || j == 0.0 || j == -1.0 || j == -3.5) {
				black(i, j);
			}
		}
		if (i == 9.0) {
			if (j == -1.5 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -2.0) {
				skin(i, j);
			}
			if (j == 2.0 || j == 0.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 1.0) {
				white(i, j);
			}
			if (j == 1.5 || j == -2.5) {
				toe(i, j);
			}
			if (j == 2.5 || j == 0.0 || j == -1.0 || j == -3.5) {
				black(i, j);
			}
		}
		if (i == 9.5) {
			if (j == 1.5 || j == 1.0 || j == -2.0 || j == -2.5) {
				toe(i, j);
			}
			if (j == 2.0 || j == 0.5 || j == -1.5 || j == -3.0) {
				black(i, j);
			}
		}
		if (i == 10.0) {
			if (j == 1.5 || j == 1.0 || j == -2.0 || j == -2.5) {
				black(i, j);
			}
		}
	}
}

void run1(GLdouble i) {
	GLdouble j;
	for (j = -13.0; j < 13.0; j += 0.5) {
		//上から下に置いていく
		if (i == -10.0) {
			if (j <= 5.5 && j >= 3.0 || j <= -4.0 && j >= -5.5) {
				black(i, j);
			}
		}
		if (i == -9.5) {
			if (j <= 5.5 && j >= 3.0 || j <= -4.0 && j >= -5.5) {
				ear_outside(i, j);
			}
			if (j <= 7.0 && j >= 6.0 || j == 2.5 || j == -3.5 || j == -6.0) {
				black(i, j);
			}
		}
		if (i == -9.0) {
			if (j <= 5.5 && j >= 3.0 || j <= -4.0 && j >= -5.5) {
				white(i, j);
			}
			if (j <= 7.0 && j >= 6.0 || j == 2.5 || j == -3.5 || j == -6.0) {
				ear_outside(i, j);
			}
			if (j == 8.0 || j == 7.5 || j <= 2.0 && j >= 0.5 || j <= -2.0 && j >= -3.0 || j == -6.5) {
				black(i, j);
			}
		}
		if (i == -8.5) {
			if (j <= 7.0 && j >= 3.0 || j <= -4.0 && j >= -6.0) {
				white(i, j);
			}
			if (j == 8.0 || j == 7.5 || j <= 2.5 && j >= 0.5 || j <= -2.0 && j >= -3.5 || j == -6.5) {
				ear_outside(i, j);
			}
			if (j == 8.5 || j <= 0.0 && j >= -1.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == -8.0) {
			if (j <= 8.0 && j >= 4.5 || j <= -4.0 && j >= -6.5) {
				white(i, j);
			}
			if (j == 8.5 || j <= 4.0 && j >= 2.5 || j == -3.0 || j == -3.5 || j == -7.0) {
				ear_outside(i, j);
			}
			if (j <= 2.0 && j >= -2.5) {
				hair_shadow(i, j);
			}
			if (j == 9.0 || j <= 0.0 && j >= -1.5 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == -7.5) {
			if (j <= 8.5 && j >= 5.5 || j <= -5.0 && j >= -7.0) {
				white(i, j);
			}
			if (j == 9.0 || j == 5.0 || j == 4.5 || j == -4.0 || j == -4.5 || j == -7.5) {
				ear_outside(i, j);
			}
			if (j <= 4.0 && j >= -3.5) {
				hair(i, j);
			}
			if (j == 9.5 || j == -8.0 || j == -8.5) {
				black(i, j);
			}
		}
		if (i == -7.0) {
			if (j <= 9.0 && j >= 6.0 || j <= -5.0 && j >= -7.5) {
				white(i, j);
			}
			if (j == 9.5 || j == 5.5 || j == 5.0 || j == -4.0 || j == -4.5 || j == -8.0 || j == -8.5) {
				ear_outside(i, j);
			}
			if (j <= 4.5 && j >= -3.5) {
				hair(i, j);
			}
			if (j == 10.0 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == -6.5) {
			if (j <= 9.5 && j >= 6.5 || j <= -5.5 && j >= -8.5) {
				white(i, j);
			}
			if (j == 10.0 || j == 6.0 || j == 5.5 || j == -4.5 || j == -5.0 || j == -8.0 || j == -9.0) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= -4.0) {
				hair(i, j);
			}
			if (j == 10.5 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == -6.0) {
			if (j <= 10.0 && j >= 7.0 || j <= -6.0 && j >= -9.0) {
				white(i, j);
			}
			if (j == 10.5 || j <= 6.5 && j >= 5.5 || j == -5.0 || j == -5.5 || j == -8.0 || j == -9.5) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= -4.5) {
				hair(i, j);
			}
			if (j == 11.0|| j == -10.0) {
				black(i, j);
			}
		}
		if (i == -5.5) {
			if (j <= 10.5 && j >= 7.5 || j <= -6.5 && j >= -9.5) {
				white(i, j);
			}
			if (j == 11.0 || j <= 7.0 && j >= 6.0 || j == -5.5 || j == -6.0 || j == -8.0 || j == -10.0) {
				ear_outside(i, j);
			}
			if (j <= 5.5 && j >= -5.0) {
				hair(i, j);
			}
			if (j == 11.5 || j == -10.5) {
				black(i, j);
			}
		}
		if (i == -5.0) {
			if (j <= 10.5 && j >= 7.5 || j <= -6.5 && j >= -10.0) {
				white(i, j);
			}
			if (j == 11.0 || j <= 7.0 && j >= 6.0 || j == -5.5 || j == -6.0 || j == -10.5) {
				ear_outside(i, j);
			}
			if (j <= 5.5 && j >= -5.0) {
				hair(i, j);
			}
			if (j == 11.5 || j == -11.0) {
				black(i, j);
			}
		}
		if (i == -4.5) {
			if (j <= -7.0 && j >= -10.5) {
				white(i, j);
			}
			if (j <= 10.5 && j >= 7.5 || j == -6.0 || j == -6.5 || j == -11.0) {
				ear_outside(i, j);
			}
			if (j <= 6.0 && j >= -5.5) {
				hair(i, j);
			}
			if (j == 7.0 || j == 6.5 || j == 3.5) {
				hair_shadow(i, j);
			}
			if (j == 11.0 || j == -11.5) {
				black(i, j);
			}
		}
		if (i == -4.0) {
			if (j <= -7.5 && j >= -11.0) {
				white(i, j);
			}
			if (j <= 9.0 && j >= 8.0 || j == -6.5 || j == -7.0 || j == -11.5) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == 6.0 || j == 5.5) {
				hair_shadow(i, j);
			}
			if (j <= 10.5 && j >= 9.5 ||  j <= 7.5 && j >= 6.5 || j == -12.0) {
				black(i, j);
			}
		}
		if (i == -3.5) {
			if (j <= -8.5 && j >= -11.0) {
				white(i, j);
			}
			if (j <= -7.0 && j >= -8.0 || j == -11.5) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= 4.0 || j <= 3.0 && j >= 2.0 || j <= 1.0 && j >= -6.5) {
				hair(i, j);
			}
			if (j == 5.5) {
				hair_shadow(i, j);
			}
			if (j == 3.5 || j == 1.5) {
				skin_shadow_thin(i, j);
			}
			if (j <= 9.0 && j >= 8.0 || j == 6.0 || j == -12.0) {
				black(i, j);
			}
		}
		if (i == -3.0) {
			if (j <= -8.5 && j >= -11.0) {
				ear_outside(i, j);
			}
			if (j == 5.5 || j == 2.0 || j == 0.5 || j == 0.0 || j <= -1.0 && j >= -6.5) {
				hair(i, j);
			}
			if (j == 5.0 || j == 2.5 || j == -0.5) {
				hair_shadow(i, j);
			}
			if (j == 4.5 || j == 4.0 || j == 1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 3.5 || j == 3.0 || j == 1.5) {
				skin(i, j);
			}
			if (j == 6.0 || j <= -7.0 && j >= -8.0 || j == -11.5) {
				black(i, j);
			}
		}
		if (i == -2.5) {
			if (j == 6.0 || j == 5.5 || j == -1.0 || j == -1.5 || j <= -2.5 && j >= -6.5) {
				hair(i, j);
			}
			if (j == -2.0) {
				hair_shadow(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 4.5 || j == 4.0 || j <= 1.5 && j >= -0.5) {
				eye_thick(i, j);
			}
			if (j == 6.5 || j == -7.0 || j <= -8.5 && j >= -11.0) {
				black(i, j);
			}
		}
		if (i == -2.0) {
			if (j == 4.0 || j == 0.5 || j== 0.0) {
				white(i, j);
			}
			if (j == 6.0 || j == 5.5 || j <= -0.5 && j >= -1.5 || j <= -2.5 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.0 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				eye_thick(i, j);
			}
			if (j == 4.5) {
				eye(i, j);
			}
			if (j == 6.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == -1.5) {
			if (j == 0.0) {
				white(i, j);
			}
			if (j == 6.0 || j == 5.5 || j <= -0.5 && j >= -1.5 || j <= -2.5 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.0 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 1.0) {
				eye_thick(i, j);
			}
			if (j == 4.5 || j == 4.0 || j == 1.5 || j == 0.5) {
				eye(i, j);
			}
			if (j == 6.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == -1.0) {
			if (j == 5.0 || j == 0.0) {
				white(i, j);
			}
			if (j == 6.0 || j <= -0.5 && j >= -1.5 || j <= -2.5 && j >= -6.5) {
				hair(i, j);
			}
			if (j == -2.0 || j == -7.0) {
				hair_shadow(i, j);
			}
			if (j == 5.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 4.5 || j == 4.0 || j <= 1.5 && j >= 0.5) {
				eye(i, j);
			}
			if (j == 6.5 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == -0.5) {
			if (j == 6.0 || j == -0.5 || j == -1.0 || j <= -2.5 && j >= -6.5) {
				hair(i, j);
			}
			if (j == -1.5 || j == -2.0 || j == -7.0) {
				hair_shadow(i, j);
			}
			if (j == 5.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 5.0 || j <= 3.5 && j >= 2.0 || j == 0.0) {
				skin(i, j);
			}
			if (j == 4.5 || j == 4.0 || j <= 1.5 && j >= 0.5) {
				eye_thin(i, j);
			}
			if (j == 6.5 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == 0.0) {
			if (j == 6.0 || j <= 0.0 && j >= -1.0 || j <= -2.5 && j >= -6.5) {
				hair(i, j);
			}
			if (j == -1.5 || j == -2.0 || j == -7.0) {
				hair_shadow(i, j);
			}
			if (j == 5.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 5.0 && j >= 0.5) {
				skin(i, j);
			}
			if (j == 6.5 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == 0.5) {
			if (j == 6.0 || j == 5.5 || j == -0.5 || j == -1.0 || j <= -2.5 && j >= -7.0) {
				hair(i, j);
			}
			if (j == 0.0 || j == -1.5 || j == -2.0 || j == -7.5) {
				hair_shadow(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 4.5 && j >= 0.5) {
				skin(i, j);
			}
			if (j == 6.5 || j == -8.0) {
				black(i, j);
			}
		}
		if (i == 1.0) {
			if (j <= 6.0 && j >= 5.0 || j == -0.5 || j == -1.0 || j <= -2.5 && j >= -7.0) {
				hair(i, j);
			}
			if (j == 0.0 || j == -1.5 || j == -2.0 || j  == -7.5 || j == -8.0) {
				hair_shadow(i, j);
			}
			if (j == 4.5 || j == 4.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.5 || j <= 1.5 && j >= 0.5) {
				skin(i, j);
			}
			if (j == 2.0) {
				mouth(i, j);
			}
			if (j == 6.5 || j == -8.5) {
				black(i, j);
			}
		}
		if (i == 1.5) {
			if (j <= 5.5 && j >= 4.0 || j == -0.5 || j == -1.0 || j <= -2.0 && j >= -7.5) {
				hair(i, j);
			}
			if (j == 0.0 || j == -1.5 || j == -8.0 || j == -8.5) {
				hair_shadow(i, j);
			}
			if (j <= 3.5 && j >= 1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				skin(i, j);
			}
			if (j == 6.0 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 2.0) {
			if (j <= 5.5 && j >= 2.5 || j <= -2.0 && j >= -7.5) {
				hair(i, j);
			}
			if (j <= -0.5 && j >= -1.5 || j == -8.0 || j == -8.5) {
				hair_shadow(i, j);
			}
			if (j <= 1.5 && j >= 0.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 0.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 6.0 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 2.5) {
			if (j <= 5.0 && j >= 2.5 || j <= -2.0 && j >= -7.5) {
				hair(i, j);
			}
			if (j == 5.5 || j == -1.0 || j == -1.5 || j <= -8.0 && j >= -9.0) {
				hair_shadow(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= 0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 6.0 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == 3.0) {
			if (j <= 5.0 && j >= 3.0 || j <= -2.0 && j >= -7.5) {
				hair(i, j);
			}
			if (j == 5.5 || j == -1.5 || j <= -8.0 && j >= -9.0) {
				hair_shadow(i, j);
			}
			if (j == 0.5 || j == -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				skin(i, j);
			}
			if (j == 2.5) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 2.0 && j >= 1.0) {
				bunny_suit(i, j);
			}
			if (j == 6.0 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == 3.5) {
			if (j <= 4.5 && j >= 3.0 || j == -2.0 || j <= -3.0 && j >=- 7.5) {
				hair(i, j);
			}
			if (j == 5.0 || j == -1.5 || j <= -8.0 && j >= -9.0) {
				hair_shadow(i, j);
			}
			if (j == 1.0 || j == -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 0.5 && j >= -0.5) {
				skin(i, j);
			}
			if (j == 2.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 2.0 || j == 1.5) {
				bunny_suit(i, j);
			}
			if (j == 5.5 || j == -2.5 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == 4.0) {
			if (j <= 4.5 && j >= 3.0 || j == -2.0 || j <= -3.5 && j >= -7.5) {
				hair(i, j);
			}
			if (j == 5.0 || j == -1.5 || j <= -8.0 && j >= -9.0) {
				hair_shadow(i, j);
			}
			if (j == -0.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= 0.0) {
				skin(i, j);
			}
			if (j == 2.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 2.0 || j == 1.5) {
				shadow2(i, j);
			}
			if (j == 5.5 || j == -1.0 || j == -2.5 || j == -3.0 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == 4.5) {
			if (j <= 4.0 && j >= 3.0 || j == -3.5 || j <= -4.5 && j >= -7.0) {
				hair(i, j);
			}
			if (j == 5.0 || j == 4.5 || j == -7.5 || j == -8.0) {
				hair_shadow(i, j);
			}
			if (j == -0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 0.5) {
				skin(i, j);
			}
			if (j == 2.5 || j == 0.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0 || j == 1.0) {
				shadow2(i, j);
			}
			if (j == 5.5 || j <= -1.0 && j >= -2.0 || j == -3.0 || j == -4.0 || j == -8.5 || j == -9.0) {
				black(i, j);
			}
			if (j == 1.5) {
				white(i, j);
			}
		}
		if (i == 5.0) {
			if (j == 3.5 || j == -1.0 || j <= -4.5 && j >= -6.0) {
				hair(i, j);
			}
			if (j <= 5.0 && j >= 4.0 || j == -6.5 || j == -7.0) {
				hair_shadow(i, j);
			}
			if (j == 3.0 || j == 2.5) {
				skin(i, j);
			}
			if (j == 0.0) {
				bunny_suit(i, j);
			}
			if (j == -0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 2.0 || j == 0.5) {
				shadow2(i, j);
			}
			if (j == 5.5 || j == -1.5 || j == -3.5 || j == -4.0 || j == -7.5 || j == -8.0) {
				black(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				white(i, j);
			}
		}
		if (i == 5.5) {
			if (j == 3.5 || j == -1.5 || j == -5.5 || j == -6.0) {
				hair(i, j);
			}
			if (j <= 5.0 && j >= 4.0 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j <= 3.0 && j >= 2.0) {
				skin(i, j);
			}
			if (j == -0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 0.5 || j == 0.0) {
				bunny_suit(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				shadow2(i, j);
			}
			if (j == 5.5 || j == -2.0 || j == -4.5 || j == -5.0 || j == -7.0) {
				black(i, j);
			}
			if (j == -1.0) {
				white(i, j);
			}
		}
		if (i == 6.0) {
			if (j == -1.5 || j == -6.0) {
				hair(i, j);
			}
			if (j <= 5.0 && j >= 3.0) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 1.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= 0.0) {
				bunny_suit(i, j);
			}
			if (j == -0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 5.5 || j == -2.0 || j == -5.5 || j == -6.5) {
				black(i, j);
			}
			if (j == -1.0) {
				white(i, j);
			}
		}
		if (i == 6.5) {
			if (j == 5.5 || j == 5.0 || j == 3.5) {
				hair_shadow(i, j);
			}
			if (j <= -0.5 && j >= -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				bunny_suit(i, j);
			}
			if (j == 2.0 || j == 0.5 || j == 0.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 3.0 || j == 2.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 6.0 || j == 4.5 || j == 4.0 || j == -2.0 || j == -6.0) {
				black(i, j);
			}
		}
		if (i == 7.0) {
			if (j == 2.0 || j <= 0.5 && j >= -0.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.5 || j == 1.0 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.0 || j == -1.5) {
				skin(i, j);
			}
			if (j == 3.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 3.0 || j == 2.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 5.5 || j == 5.0 || j == 4.0 || j == -2.5) {
				black(i, j);
			}
		}
		if (i == 7.5) {
			if (j == 1.0 || j == 0.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 0.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= -0.5 && j >= -2.0) {
				skin(i, j);
			}
			if (j == 4.0 || j == 2.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 4.5 || j == 1.5 || j == -3.0) {
				black(i, j);
			}
			if (j <= 3.5 && j >= 2.5) {
				white(i, j);
			}
			if (j == -2.5) {
				toe(i, j);
			}
		}
		if (i == 8.0) {
			if (j <= 0.0 && j >= -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 4.0 || j == 2.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 4.5 || j == 2.0 || j == 1.0 || j == 0.5 || j == -3.0) {
				black(i, j);
			}
			if (j == 3.5 || j == 3.0) {
				white(i, j);
			}
			if (j == -2.5) {
				toe(i, j);
			}
		}
		if (i == 8.5) {
			if (j == 4.0 || j == 2.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 4.5 || j == 2.0 || j <= 0.0 && j >= -2.5) {
				black(i, j);
			}
			if (j == 3.5 || j == 3.0) {
				white(i, j);
			}
		}
		if (i == 9.0) {
			if (j == 4.0 || j == 3.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 3.5) {
				white(i, j);
			}
			if (j == 4.5 || j == 2.5) {
				black(i, j);
			}
		}
		if (i == 9.5) {
			if (j == 4.0 || j == 3.5) {
				toe(i, j);
			}
			if (j == 4.5 || j == 3.0) {
				black(i, j);
			}
		}
		if (i == 10.0) {
			if (j == 4.0 || j == 3.5) {
				black(i, j);
			}
		}
	}
}

void run2(GLdouble i) {
	GLdouble j;
	for (j = -13.0; j < 13.0; j += 0.5) {
		//上から下に置いていく
		if (i == -10.0) {
			if (j <= 5.5 && j >= 3.0 || j == -4.0 || j == -4.5) {
				black(i, j);
			}
		}
		if (i == -9.5) {
			if (j <= 5.5 && j >= 3.0 || j == -4.0 || j == -4.5) {
				ear_outside(i, j);
			}
			if (j <= 7.0 && j >= 6.0 || j == 2.5 || j == -3.5 || j == -5.0) {
				black(i, j);
			}
		}
		if (i == -9.0) {
			if (j <= 5.5 && j >= 3.0 || j == -4.0 || j == -4.5) {
				white(i, j);
			}
			if (j <= 7.0 && j >= 6.0 || j == 2.5 || j == -3.5 || j == -5.0) {
				ear_outside(i, j);
			}
			if (j == 8.0 || j == 7.5 || j <= 2.0 && j >= 0.5 || j <= -2.0 && j >= -3.0 || j == -5.5) {
				black(i, j);
			}
		}
		if (i == -8.5) {
			if (j <= 7.0 && j >= 3.0 || j <= -3.5 && j >= -5.0) {
				white(i, j);
			}
			if (j == 8.0 || j == 7.5 || j <= 2.5 && j >= 0.5 || j <= -2.0 && j >= -3.0 || j == -5.5) {
				ear_outside(i, j);
			}
			if (j == 8.5 || j <= 0.0 && j >= -1.5 || j == -6.0) {
				black(i, j);
			}
		}
		if (i == -8.0) {
			if (j <= 8.0 && j >= 4.5 || j <= -3.5 && j >= -5.5) {
				white(i, j);
			}
			if (j == 8.5 || j <= 4.0 && j >= 2.5 || j == -3.0 || j == -6.0) {
				ear_outside(i, j);
			}
			if (j <= 2.0 && j >= -2.5) {
				hair_shadow(i, j);
			}
			if (j == 9.0 || j <= 0.0 && j >= -1.5 || j == -6.5) {
				black(i, j);
			}
		}
		if (i == -7.5) {
			if (j <= 8.5 && j >= 5.5 || j <= -4.0 && j >= -6.0) {
				white(i, j);
			}
			if (j == 9.0 || j == 5.0 || j == 4.5 || j == -3.5 || j == -6.5) {
				ear_outside(i, j);
			}
			if (j <= 4.0 && j >= -3.0) {
				hair(i, j);
			}
			if (j == 9.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == -7.0) {
			if (j <= 9.0 && j >= 6.0 || j <= -4.5 && j >= -6.5) {
				white(i, j);
			}
			if (j == 9.5 || j == 5.5 || j == 5.0 || j == -4.0 || j == -7.0){
				ear_outside(i, j);
			}
			if (j <= 4.5 && j >= -3.5) {
				hair(i, j);
			}
			if (j == 10.0 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == -6.5) {
			if (j <= 9.5 && j >= 6.5 || j <= -5.0 && j >= -7.0) {
				white(i, j);
			}
			if (j == 10.0 || j == 6.0 || j == 5.5 || j == -4.5 || j == -7.5) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= -4.0) {
				hair(i, j);
			}
			if (j == 10.5 || j == -8.0) {
				black(i, j);
			}
		}
		if (i == -6.0) {
			if (j <= 10.0 && j >= 7.0 || j <= -5.5 && j >= -7.5) {
				white(i, j);
			}
			if (j == 10.5 || j <= 6.5 && j >= 5.5 || j == -5.0 || j == -8.0) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= -4.5) {
				hair(i, j);
			}
			if (j == 11.0 || j == -8.5) {
				black(i, j);
			}
		}
		if (i == -5.5) {
			if (j <= 10.5 && j >= 7.5 || j <= -6.0 && j >= -8.0) {
				white(i, j);
			}
			if (j == 11.0 || j <= 7.0 && j >= 6.0 || j == -5.5 || j == -8.5) {
				ear_outside(i, j);
			}
			if (j <= 5.5 && j >= -5.0) {
				hair(i, j);
			}
			if (j == 11.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == -5.0) {
			if (j <= 10.5 && j >= 7.5 || j <= -6.0 && j >= -8.5) {
				white(i, j);
			}
			if (j == 11.0 || j <= 7.0 && j >= 6.0 || j == -5.5 || j == -9.0) {
				ear_outside(i, j);
			}
			if (j <= 5.5 && j >= -5.0) {
				hair(i, j);
			}
			if (j == 11.5 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == -4.5) {
			if (j <= -6.5 && j >= -9.0) {
				white(i, j);
			}
			if (j <= 10.5 && j >= 7.5 || j == -6.0 || j == -9.5) {
				ear_outside(i, j);
			}
			if (j <= 6.0 && j >= -5.5) {
				hair(i, j);
			}
			if (j == 7.0 || j == 6.5 || j == 3.5) {
				hair_shadow(i, j);
			}
			if (j == 11.0 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == -4.0) {
			if (j <= -7.0 && j >= -9.5) {
				white(i, j);
			}
			if (j <= 9.0 && j >= 8.0 || j == -6.5 || j == -10.0) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == 6.0 || j == 5.5) {
				hair_shadow(i, j);
			}
			if (j <= 10.5 && j >= 9.5 || j <= 7.5 && j >= 6.5 || j == -10.5) {
				black(i, j);
			}
		}
		if (i == -3.5) {
			if (j <= -8.0 && j >= -10.0) {
				white(i, j);
			}
			if (j == -7.0 || j == -7.5 || j == -10.5) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= 4.0 || j <= 3.0 && j >= 2.0 || j <= 1.0 && j >= -6.5) {
				hair(i, j);
			}
			if (j == 5.5) {
				hair_shadow(i, j);
			}
			if (j == 3.5 || j == 1.5) {
				skin_shadow_thin(i, j);
			}
			if (j <= 9.0 && j >= 8.0 || j == 6.0 || j == -11.0) {
				black(i, j);
			}
		}
		if (i == -3.0) {
			if (j <= -8.5 && j >= -9.5) {
				white(i, j);
			}
			if (j == -8.0 ||j == -10.0) {
				ear_outside(i, j);
			}
			if (j == 5.5 || j == 2.0 || j == 0.5 || j == 0.0 || j <= -1.0 && j >= -6.5) {
				hair(i, j);
			}
			if (j == 5.0 || j == 2.5 || j == -0.5) {
				hair_shadow(i, j);
			}
			if (j == 4.5 || j == 4.0 || j == 1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 3.5 || j == 3.0 || j == 1.5) {
				skin(i, j);
			}
			if (j == 6.0 || j == -7.0 || j == -7.5 || j == -10.5) {
				black(i, j);
			}
		}
		if (i == -2.5) {
			if (j == 6.0 || j == 5.5 || j <= -1.0 && j >= -2.0 || j <= -3.0 && j >= -6.5) {
				hair(i, j);
			}
			if (j <= -8.5 && j >= -9.5) {
				ear_outside(i, j);
			}
			if (j == -2.5) {
				hair_shadow(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 4.5 || j == 4.0 || j <= 1.5 && j >= -0.5) {
				eye_thick(i, j);
			}
			if (j == 6.5 || j == -7.0 || j == -8.0 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == -2.0) {
			if (j == 4.0 || j == 0.5 || j == 0.0) {
				white(i, j);
			}
			if (j == 6.0 || j == 5.5 || j <= -0.5 && j >= -2.0 || j <= -3.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.5 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				eye_thick(i, j);
			}
			if (j == 4.5) {
				eye(i, j);
			}
			if (j == 6.5 || j == -7.0 || j <= -8.5 && j >= -9.5) {
				black(i, j);
			}
		}
		if (i == -1.5) {
			if (j == 0.0) {
				white(i, j);
			}
			if (j == 6.0 || j == 5.5 || j <= -0.5 && j >= -2.0 || j <= -3.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.5 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 1.0) {
				eye_thick(i, j);
			}
			if (j == 4.5 || j == 4.0 || j == 1.5 || j == 0.5) {
				eye(i, j);
			}
			if (j == 6.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == -1.0) {
			if (j == 5.0 || j == 0.0) {
				white(i, j);
			}
			if (j == 6.0 || j <= -0.5 && j >= -2.0 || j <= -3.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.5 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 5.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 4.5 || j == 4.0 || j <= 1.5 && j >= 0.5) {
				eye(i, j);
			}
			if (j == 6.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == -0.5) {
			if (j == 6.0 || j <= -0.5 && j >= -2.0 || j <= -3.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.5 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 5.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 5.0 || j <= 3.5 && j >= 2.0 || j == 0.0) {
				skin(i, j);
			}
			if (j == 4.5 || j == 4.0 || j <= 1.5 && j >= 0.5) {
				eye_thin(i, j);
			}
			if (j == 6.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == 0.0) {
			if (j == 6.0 || j <= 0.0 && j >= -1.5 || j <= -2.5 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.0 || j == -6.5 || j == -7.0) {
				hair_shadow(i, j);
			}
			if (j == 5.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 5.0 && j >= 0.5) {
				skin(i, j);
			}
			if (j == 6.5 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == 0.5) {
			if (j == 6.0 || j == 5.5 || j <= -0.0 && j >= -1.5 || j <= -2.5 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.0 || j == -6.5 ||  j == -7.0) {
				hair_shadow(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 4.5 && j >= 0.5) {
				skin(i, j);
			}
			if (j == 6.5 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == 1.0) {
			if (j <= 6.0 && j >= 5.0 || j <= 0.0 && j >= -1.5 || j <= -2.5 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.0 || j <= -6.5 && j >= -7.5) {
				hair_shadow(i, j);
			}
			if (j == 4.5 || j == 4.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.5 || j <= 1.5 && j >= 0.5) {
				skin(i, j);
			}
			if (j == 2.0) {
				mouth(i, j);
			}
			if (j == 6.5 || j == -8.0) {
				black(i, j);
			}
		}
		if (i == 1.5) {
			if (j <= 5.5 && j >= 4.0 || j <= 0.0 && j >= -1.0 || j <= -2.5 && j >= -6.5) {
				hair(i, j);
			}
			if (j == -1.5 || j == -2.0 || j <= -7.0 && j >= -8.0) {
				hair_shadow(i, j);
			}
			if (j <= 3.5 && j >= 1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				skin(i, j);
			}
			if (j == 6.0 || j == -8.5) {
				black(i, j);
			}
		}
		if (i == 2.0) {
			if (j <= 0.0 && j >= -1.0 || j <= 5.5 && j >= 2.5 || j <= -2.5 && j >= -7.0) {
				hair(i, j);
			}
			if (j == -1.5 || j == -2.0 || j <= -7.5 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j <= 1.5 && j >= 0.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 0.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 6.0 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 2.5) {
			if (j <= 5.0 && j >= 2.5 || j <= -2.5 && j >= -7.0) {
				hair(i, j);
			}
			if (j == 5.5 || j == -1.5 || j == -2.0 || j <= -7.5 && j >= -9.0) {
				hair_shadow(i, j);
			}
			if (j == -0.5 || j == -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= 0.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 6.0 || j == -9.5 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == 3.0) {
			if (j <= 5.0 && j >= 3.0 || j <= -2.5 && j >= -7.5) {
				hair(i, j);
			}
			if (j == 5.5 || j == -2.0 || j <= -8.0 && j >= -10.0) {
				hair_shadow(i, j);
			}
			if (j == 2.5 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == -0.5) {
				skin(i, j);
			}
			if (j <= 2.0 && j >= 0.0) {
				bunny_suit(i, j);
			}
			if (j == 6.0 || j == -10.5) {
				black(i, j);
			}
		}
		if (i == 3.5) {
			if (j <= 4.5 && j >= 3.0 || j <= -3.0 && j >= -7.5) {
				hair(i, j);
			}
			if (j == 5.5 || j == 5.0 || j <= -8.0 && j >= -9.0) {
				hair_shadow(i, j);
			}
			if (j == 2.5 || j == -0.5 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.0 || j == -1.5) {
				skin(i, j);
			}
			if (j <= 2.0 && j >= 0.5) {
				bunny_suit(i, j);
			}
			if (j == 0.0) {
				shadow1(i, j);
			}
			if (j == 6.0 || j == -2.5 || j == -9.5 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == 4.0) {
			if (j <= 4.5 && j >= 3.5 || j <= -3.5 && j >= -7.5) {
				hair(i, j);
			}
			if (j <= 6.0 && j >= 5.0 || j == -8.0 || j == -8.5) {
				hair_shadow(i, j);
			}
			if (j == 3.0 || j == 2.5 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.0 || j == -1.5) {
				skin(i, j);
			}
			if (j == -0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				bunny_suit(i, j);
			}
			if (j == 2.0 || j == 0.5 || j == 0.0) {
				shadow1(i, j);
			}
			if (j == 6.5 || j == -1.0 || j == -2.5 || j == -3.0 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 4.5) {
			if (j == 4.0 || j == -3.5 || j <= -4.5 && j >= -7.5 || j == -8.5) {
				hair(i, j);
			}
			if (j <= 6.0 && j >= 4.5) {
				hair_shadow(i, j);
			}
			if (j <= 2.0 && j >= 0.0) {
				bunny_suit(i, j);
			}
			if (j == -0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 3.5 || j == 2.5 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 3.0) {
				skin_shadow_thin(i, j);
			}
			if (j == -1.0 || j == -1.5) {
				shadow2(i, j);
			}
			if (j == 6.5 || j == -2.5 || j == -3.0 || j == -4.0 || j == -8.0 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 5.0) {
			if (j <= -4.5 && j >= -7.5) {
				hair(i, j);
			}
			if (j <= 6.0 && j >= 4.0) {
				hair_shadow(i, j);
			}
			if (j == 3.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 3.5 || j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= 0.0) {
				bunny_suit(i, j);
			}
			if (j == -0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == -1.0 || j == -2.0) {
				shadow2(i, j);
			}
			if (j == 6.5 || j == -2.5 || j == -3.5 || j == -4.0 || j == -8.0 || j == -8.5) {
				black(i, j);
			}
			if (j == -1.5) {
				white(i, j);
			}
		}
		if (i == 5.5) {
			if (j <= -5.0 && j >= -6.5) {
				hair(i, j);
			}
			if (j == 6.5 || j <= 5.5 && j >= 3.5) {
				hair_shadow(i, j);
			}
			if (j == 3.0 || j == 2.5 || j == -1.0 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= 0.5) {
				bunny_suit(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				shadow1(i, j);
			}
			if (j == 7.0 || j == 6.0 || j == -2.5 || j == -4.5 || j == -7.0 || j == -7.5) {
				black(i, j);
			}
			if (j == -1.5) {
				white(i, j);
			}
		}
		if (i == 6.0) {
			if (j == -5.5 || j <= -6.5 && j >= -7.5) {
				hair(i, j);
			}
			if (j == 6.5 || j <= 5.0 && j >= 3.0) {
				hair_shadow(i, j);
			}
			if (j == -1.0 || j == -1.5) {
				skin(i, j);
			}
			if (j == 2.5 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= 0.5) {
				bunny_suit(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				shadow1(i, j);
			}
			if (j == 7.0 || j == 6.0 || j == 5.5 || j == 2.5 || j == -2.5 ||j == -5.0 || j == -6.0 || j == -8.0) {
				black(i, j);
			}
			if (j == -1.0) {
				white(i, j);
			}
		}
		if (i == 6.5) {
			if (j == 3.5) {
				hair_shadow(i, j);
			}
			if (j == -1.0 || j == -1.5) {
				skin(i, j);
			}
			if (j == 2.5 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= 0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				shadow1(i, j);
			}
			if (j == 6.5 || j <= 5.0 && j >= 4.0 || j == 3.0 || j == 2.5 || j == -2.5 || j == -5.5 || j <= -6.5 && j >= -7.5) {
				black(i, j);
			}
		}
		if (i == 7.0) {
			if (j == 1.0 || j == 0.5 || j == -1.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= 2.5 && j >= 1.5 || j == 0.0 || j == -1.5 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == -1.0 || j == -1.5) {
				skin(i, j);
			}
			if (j == 3.5 || j == 3.0 || j == -2.5) {
				black(i, j);
			}
		}
		if (i == 7.5) {
			if (j == 2.0 || j == 1.5 || j == 0.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				skin_shadow_thin(i, j);
			}
			if (j == -1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == -0.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 2.5 || j == -2.0) {
				black(i, j);
			}
		}
		if (i == 8.0) {
			if (j == 1.0 || j == 0.5) {
				skin(i, j);
			}
			if (j == 1.5 || j == 0.0 || j == -0.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.0) {
				reg_shadow_thin(i, j);
			}
			if (j == 2.0 || j == -2.5) {
				black(i, j);
			}
			if (j == -1.5) {
				white(i, j);
			}
			if (j == -2.0) {
				toe(i, j);
			}
		}
		if (i == 8.5) {
			if (j == 0.5 || j == 0.0) {
				skin(i, j);
			}
			if (j == 1.0 || j == -0.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.0) {
				reg_shadow_thin(i, j);
			}
			if (j == 1.5 || j == -2.5) {
				black(i, j);
			}
			if (j == -1.5) {
				white(i, j);
			}
			if (j == -2.0) {
				toe(i, j);
			}
		}
		if (i == 9.0) {
			if (j == 0.0 || j == -0.5) {
				skin(i, j);
			}
			if (j == 0.5 || j == -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0 || j == -1.5 || j == -2.0) {
				black(i, j);
			}
		}
		if (i == 9.5) {
			if (j == 0.0 || j == -0.5) {
				skin(i, j);
			}
			if (j == 0.5 || j == -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0 || j == -1.5) {
				black(i, j);
			}
		}
		if (i == 10.0) {
			if (j == 0.5 || j == -1.0) {
				black(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				toe(i, j);
			}
		}
		if (i == 10.5) {
			if (j == 0.0 || j == -0.5) {
				black(i, j);
			}
		}
	}
}

void run3(GLdouble i) {
	GLdouble j;
	for (j = -13.0; j < 13.0; j += 0.5) {
		//上から下に置いていく
		if (i == -10.0) {
			if (j <= 5.5 && j >= 3.0 || j == -4.0 || j == -4.5) {
				black(i, j);
			}
		}
		if (i == -9.5) {
			if (j <= 5.5 && j >= 3.0 || j == -4.0 || j == -4.5) {
				ear_outside(i, j);
			}
			if (j <= 7.0 && j >= 6.0 || j == 2.5 || j == -3.5 || j == -5.0) {
				black(i, j);
			}
		}
		if (i == -9.0) {
			if (j <= 5.5 && j >= 3.0 || j == -4.0 || j == -4.5) {
				white(i, j);
			}
			if (j <= 7.0 && j >= 6.0 || j == 2.5 || j == -3.5 || j == -5.0) {
				ear_outside(i, j);
			}
			if (j == 8.0 || j == 7.5 || j <= 2.0 && j >= 0.5 || j <= -2.0 && j >= -3.0 || j == -5.5) {
				black(i, j);
			}
		}
		if (i == -8.5) {
			if (j <= 7.0 && j >= 3.0 || j <= -3.5 && j >= -5.0) {
				white(i, j);
			}
			if (j == 8.0 || j == 7.5 || j <= 2.5 && j >= 0.5 || j <= -2.0 && j >= -3.0 || j == -5.5) {
				ear_outside(i, j);
			}
			if (j == 8.5 || j <= 0.0 && j >= -1.5 || j == -6.0) {
				black(i, j);
			}
		}
		if (i == -8.0) {
			if (j <= 8.0 && j >= 4.5 || j <= -3.5 && j >= -5.5) {
				white(i, j);
			}
			if (j == 8.5 || j <= 4.0 && j >= 2.5 || j == -3.0 || j == -6.0) {
				ear_outside(i, j);
			}
			if (j <= 2.0 && j >= -2.5) {
				hair_shadow(i, j);
			}
			if (j == 9.0 || j <= 0.0 && j >= -1.5 || j == -6.5) {
				black(i, j);
			}
		}
		if (i == -7.5) {
			if (j <= 8.5 && j >= 5.5 || j <= -4.0 && j >= -6.0) {
				white(i, j);
			}
			if (j == 9.0 || j == 5.0 || j == 4.5 || j == -3.5 || j == -6.5) {
				ear_outside(i, j);
			}
			if (j <= 4.0 && j >= -3.0) {
				hair(i, j);
			}
			if (j == 9.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == -7.0) {
			if (j <= 9.0 && j >= 6.0 || j <= -4.5 && j >= -6.5) {
				white(i, j);
			}
			if (j == 9.5 || j == 5.5 || j == 5.0 || j == -4.0 || j == -7.0) {
				ear_outside(i, j);
			}
			if (j <= 4.5 && j >= -3.5) {
				hair(i, j);
			}
			if (j == 10.0 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == -6.5) {
			if (j <= 9.5 && j >= 6.5 || j <= -5.0 && j >= -7.0) {
				white(i, j);
			}
			if (j == 10.0 || j == 6.0 || j == 5.5 || j == -4.5 || j == -7.5) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= -4.0) {
				hair(i, j);
			}
			if (j == 10.5 || j == -8.0) {
				black(i, j);
			}
		}
		if (i == -6.0) {
			if (j <= 10.0 && j >= 7.0 || j <= -5.5 && j >= -7.5) {
				white(i, j);
			}
			if (j == 10.5 || j <= 6.5 && j >= 5.5 || j == -5.0 || j == -8.0) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= -4.5) {
				hair(i, j);
			}
			if (j == 11.0 || j == -8.5) {
				black(i, j);
			}
		}
		if (i == -5.5) {
			if (j <= 10.5 && j >= 7.5 || j <= -6.0 && j >= -8.0) {
				white(i, j);
			}
			if (j == 11.0 || j <= 7.0 && j >= 6.0 || j == -5.5 || j == -8.5) {
				ear_outside(i, j);
			}
			if (j <= 5.5 && j >= -5.0) {
				hair(i, j);
			}
			if (j == 11.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == -5.0) {
			if (j <= 10.5 && j >= 7.5 || j <= -6.0 && j >= -8.5) {
				white(i, j);
			}
			if (j == 11.0 || j <= 7.0 && j >= 6.0 || j == -5.5 || j == -9.0) {
				ear_outside(i, j);
			}
			if (j <= 5.5 && j >= -5.0) {
				hair(i, j);
			}
			if (j == 11.5 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == -4.5) {
			if (j <= -6.5 && j >= -9.0) {
				white(i, j);
			}
			if (j <= 10.5 && j >= 7.5 || j == -6.0 || j == -9.5) {
				ear_outside(i, j);
			}
			if (j <= 6.0 && j >= -5.5) {
				hair(i, j);
			}
			if (j == 7.0 || j == 6.5 || j == 3.5) {
				hair_shadow(i, j);
			}
			if (j == 11.0 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == -4.0) {
			if (j <= -7.0 && j >= -9.5) {
				white(i, j);
			}
			if (j <= 9.0 && j >= 8.0 || j == -6.5 || j == -10.0) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == 6.0 || j == 5.5) {
				hair_shadow(i, j);
			}
			if (j <= 10.5 && j >= 9.5 || j <= 7.5 && j >= 6.5 || j == -10.5) {
				black(i, j);
			}
		}
		if (i == -3.5) {
			if (j <= -8.0 && j >= -10.0) {
				white(i, j);
			}
			if (j == -7.0 || j == -7.5 || j == -10.5) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= 4.0 || j <= 3.0 && j >= 2.0 || j <= 1.0 && j >= -6.5) {
				hair(i, j);
			}
			if (j == 5.5) {
				hair_shadow(i, j);
			}
			if (j == 3.5 || j == 1.5) {
				skin_shadow_thin(i, j);
			}
			if (j <= 9.0 && j >= 8.0 || j == 6.0 || j == -11.0) {
				black(i, j);
			}
		}
		if (i == -3.0) {
			if (j <= -8.5 && j >= -9.5) {
				white(i, j);
			}
			if (j == -8.0 || j == -10.0) {
				ear_outside(i, j);
			}
			if (j == 5.5 || j == 2.0 || j == 0.5 || j == 0.0 || j <= -1.0 && j >= -6.5) {
				hair(i, j);
			}
			if (j == 5.0 || j == 2.5 || j == -0.5) {
				hair_shadow(i, j);
			}
			if (j == 4.5 || j == 4.0 || j == 1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 3.5 || j == 3.0 || j == 1.5) {
				skin(i, j);
			}
			if (j == 6.0 || j == -7.0 || j == -7.5 || j == -10.5) {
				black(i, j);
			}
		}
		if (i == -2.5) {
			if (j == 6.0 || j == 5.5 || j <= -1.0 && j >= -2.0 || j <= -3.0 && j >= -6.5) {
				hair(i, j);
			}
			if (j <= -8.5 && j >= -9.5) {
				ear_outside(i, j);
			}
			if (j == -2.5) {
				hair_shadow(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 4.5 || j == 4.0 || j <= 1.5 && j >= -0.5) {
				eye_thick(i, j);
			}
			if (j == 6.5 || j == -7.0 || j == -8.0 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == -2.0) {
			if (j == 4.0 || j == 0.5 || j == 0.0) {
				white(i, j);
			}
			if (j == 6.0 || j == 5.5 || j <= -0.5 && j >= -2.0 || j <= -3.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.5 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				eye_thick(i, j);
			}
			if (j == 4.5) {
				eye(i, j);
			}
			if (j == 6.5 || j == -7.0 || j <= -8.5 && j >= -9.5) {
				black(i, j);
			}
		}
		if (i == -1.5) {
			if (j == 0.0) {
				white(i, j);
			}
			if (j == 6.0 || j == 5.5 || j <= -0.5 && j >= -2.0 || j <= -3.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.5 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 1.0) {
				eye_thick(i, j);
			}
			if (j == 4.5 || j == 4.0 || j == 1.5 || j == 0.5) {
				eye(i, j);
			}
			if (j == 6.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == -1.0) {
			if (j == 5.0 || j == 0.0) {
				white(i, j);
			}
			if (j == 6.0 || j <= -0.5 && j >= -2.0 || j <= -3.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.5 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 5.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 4.5 || j == 4.0 || j <= 1.5 && j >= 0.5) {
				eye(i, j);
			}
			if (j == 6.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == -0.5) {
			if (j == 6.0 || j <= -0.5 && j >= -2.0 || j <= -3.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.5 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 5.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 5.0 || j <= 3.5 && j >= 2.0 || j == 0.0) {
				skin(i, j);
			}
			if (j == 4.5 || j == 4.0 || j <= 1.5 && j >= 0.5) {
				eye_thin(i, j);
			}
			if (j == 6.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == 0.0) {
			if (j == 6.0 || j <= 0.0 && j >= -1.5 || j <= -2.5 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.0 || j == -6.5 || j == -7.0) {
				hair_shadow(i, j);
			}
			if (j == 5.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 5.0 && j >= 0.5) {
				skin(i, j);
			}
			if (j == 6.5 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == 0.5) {
			if (j == 6.0 || j == 5.5 || j <= -0.0 && j >= -1.5 || j <= -2.5 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.0 || j == -6.5 || j == -7.0) {
				hair_shadow(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 4.5 && j >= 0.5) {
				skin(i, j);
			}
			if (j == 6.5 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == 1.0) {
			if (j <= 6.0 && j >= 5.0 || j <= 0.0 && j >= -1.5 || j <= -2.5 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.0 || j <= -6.5 && j >= -7.5) {
				hair_shadow(i, j);
			}
			if (j == 4.5 || j == 4.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.5 || j <= 1.5 && j >= 0.5) {
				skin(i, j);
			}
			if (j == 2.0) {
				mouth(i, j);
			}
			if (j == 6.5 || j == -8.0) {
				black(i, j);
			}
		}
		if (i == 1.5) {
			if (j <= 5.5 && j >= 4.0 || j <= 0.0 && j >= -1.0 || j <= -2.5 && j >= -6.5) {
				hair(i, j);
			}
			if (j == -1.5 || j == -2.0 || j <= -7.0 && j >= -8.0) {
				hair_shadow(i, j);
			}
			if (j <= 3.5 && j >= 1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				skin(i, j);
			}
			if (j == 6.0 || j == -8.5) {
				black(i, j);
			}
		}
		if (i == 2.0) {
			if (j <= 0.0 && j >= -1.0 || j <= 5.5 && j >= 2.5 || j <= -2.5 && j >= -7.0) {
				hair(i, j);
			}
			if (j == -1.5 || j == -2.0 || j <= -7.5 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j <= 1.5 && j >= 0.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 0.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 6.0 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 2.5) {
			if (j <= 5.0 && j >= 3.0 || j <= -2.5 && j >= -7.0) {
				hair(i, j);
			}
			if (j == 5.5 || j == -1.5 || j == -2.0 || j <= -7.5 && j >= -9.0) {
				hair_shadow(i, j);
			}
			if (j == 2.5 || j == -0.5 || j == -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= 0.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 6.0 || j == -9.5 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == 3.0) {
			if (j <= 5.0 && j >= 3.5 || j <= -2.5 && j >= -7.5) {
				hair(i, j);
			}
			if (j == 5.5 || j == -2.0 || j <= -8.0 && j >= -10.0) {
				hair_shadow(i, j);
			}
			if (j == 3.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 2.5 || j == -0.5) {
				skin(i, j);
			}
			if (j <= 2.0 && j >= 0.0) {
				bunny_suit(i, j);
			}
			if (j == 6.0 || j == -10.5) {
				black(i, j);
			}
		}
		if (i == 3.5) {
			if (j == 4.5 || j == 4.0 || j <= -3.0 && j >= -7.5) {
				hair(i, j);
			}
			if (j == 5.5 || j == 5.0 || j <= -8.0 && j >= -9.0) {
				hair_shadow(i, j);
			}
			if (j == 3.5 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 3.0 || j == 2.5 || j <= -0.5 && j >= -1.5) {
				skin(i, j);
			}
			if (j <= 2.0 && j >= 0.5) {
				bunny_suit(i, j);
			}
			if (j == 0.0) {
				shadow1(i, j);
			}
			if (j == 6.0 || j == -2.5 || j == -9.5 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == 4.0) {
			if (j == 4.5 || j <= -3.5 && j >= -7.5) {
				hair(i, j);
			}
			if (j <= 6.0 && j >= 5.0 || j == -8.0 || j == -8.5) {
				hair_shadow(i, j);
			}
			if (j == 4.0 || j == -0.5 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.5 || j <= -1.0 && j >= -2.0) {
				skin(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				bunny_suit(i, j);
			}
			if (j == 2.0 || j == 0.5 || j == 0.0) {
				shadow1(i, j);
			}
			if (j == 6.5 || j == -1.0 || j == -3.0 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 4.5) {
			if (j <= -4.5 && j >= -7.5 || j == -8.5) {
				hair(i, j);
			}
			if (j <= 6.0 && j >= 5.0) {
				hair_shadow(i, j);
			}
			if (j <= 2.0 && j >= 0.0) {
				bunny_suit(i, j);
			}
			if (j == -0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 4.0 && j >= 3.0 || j <= -1.5 && j >= -2.5) {
				skin(i, j);
			}
			if (j == 2.5 || j == -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 4.5 || j == -3.0) {
				shadow2(i, j);
			}
			if (j == 6.5 || j == -1.0 || j == -3.5 || j == -4.0 || j == -8.0 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 5.0) {
			if (j <= -4.5 && j >= -7.5) {
				hair(i, j);
			}
			if (j == 6.0 || j == 5.5) {
				hair_shadow(i, j);
			}
			if (j == 3.5 || j == -2.0) {
				skin(i, j);
			}
			if (j == 3.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= 0.0) {
				bunny_suit(i, j);
			}
			if (j == 2.5 || j == -0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 5.0 || j == 4.0 || j == -2.5 || j == -3.5) {
				shadow2(i, j);
			}
			if (j == 6.5 || j == -1.0 || j == -4.0 || j == -8.0 || j == -8.5) {
				black(i, j);
			}
			if (j == 4.5 || j == -3.0) {
				white(i, j);
			}
		}
		if (i == 5.5) {
			if (j <= -5.0 && j >= -6.5) {
				hair(i, j);
			}
			if (j == 6.5 || j == 3.0) {
				hair_shadow(i, j);
			}
			if (j == 6.0 || j == 5.5 || j == -4.0 || j == -4.5) {
				skin(i, j);
			}
			if (j <= 2.0 && j >= 0.5) {
				bunny_suit(i, j);
			}
			if (j == 2.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				shadow1(i, j);
			}
			if (j == 3.5 || j == 5.0 || j == -2.0 || j == -3.5) {
				shadow2(i, j);
			}
			if (j == 7.0 || j == -1.0 || j == -1.5 || j == -7.0 || j == -7.5) {
				black(i, j);
			}
			if (j == 4.5 || j== 4.0 || j == -2.5 || j == -3.0) {
				white(i, j);
			}
		}
		if (i == 6.0) {
			if (j == -5.5 || j <= -6.5 && j >= -7.5) {
				hair(i, j);
			}
			if (j == 6.5 || j == 3.5 || j == 3.0) {
				hair_shadow(i, j);
			}
			if (j <= 6.0 && j >= 5.0 || j <= -3.5 && j >= -4.5) {
				skin(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= 0.5) {
				bunny_suit(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				shadow1(i, j);
			}
			if (j == 4.5 || j == 4.0 || j == -2.5 || j == -3.0) {
				shadow2(i, j);
			}
			if (j == 7.0 || j == -1.0 || j == -2.0 || j == -5.0 || j == -6.0 || j == -8.0) {
				black(i, j);
			}
		}
		if (i == 6.5) {
			if (j == 3.5) {
				hair_shadow(i, j);
			}
			if (j == 6.0 || j == 5.5 || j == -4.0 || j == -4.5) {
				skin(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= 0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				shadow1(i, j);
			}
			if (j == 6.5 || j <= 5.0 && j >= 4.0 || j == 3.0 || j <= -1.0 && j >= -3.5 || j== -5.0 || j == -5.5 || j <= -6.5 && j >= -7.5) {
				black(i, j);
			}
		}
		if (i == 7.0) {
			if (j <= 2.0 && j >= 1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 2.5 || j == 0.5 || j == 0.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= -0.5 && j >= -1.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 6.0 || j == 5.5 || j == 3.5 || j == 3.0 || j == -2.0 || j == -4.0 || j == -4.5) {
				black(i, j);
			}
		}
		if (i == 7.5) {
			if (j == 3.0 || j == 1.0 || j == 0.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.5 && j >= 1.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 3.5 || j == -3.0) {
				black(i, j);
			}
			if (j <= 0.0 && j >= -2.0) {
				white(i, j);
			}
			if (j == -2.5) {
				toe(i, j);
			}
		}
		if (i == 8.0) {
			if (j <=3.0 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 3.5 || j == 1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 4.0 || j == 1.0 || j == 0.5 || j == -3.0) {
				black(i, j);
			}
			if (j <= 0.0 && j >= -2.0) {
				white(i, j);
			}
			if (j == -2.5) {
				toe(i, j);
			}
		}
		if (i == 8.5) {
			if (j == 3.0 || j == 2.5) {
				skin(i, j);
			}
			if (j == 3.5 || j == 2.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= -0.5 && j >= -2.0) {
				reg_shadow_thin(i, j);
			}
			if (j == 4.0 || j == 1.5 || j == 0.0 || j == -2.5) {
				black(i, j);
			}
		}
		if (i == 9.0) {
			if (j == 3.0 || j == 2.5) {
				skin(i, j);
			}
			if (j == 3.5 || j == 2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 4.0 || j == 1.5 || j <= -0.5 && j >= -2.0) {
				black(i, j);
			}
		}
		if (i == 9.5) {
			if (j == 3.0) {
				skin(i, j);
			}
			if (j == 3.5 || j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 4.0 || j == 2.0) {
				black(i, j);
			}
		}
		if (i == 10.0) {
			if (j == 3.5 || j == 2.0) {
				black(i, j);
			}
			if (j == 3.0 || j == 2.5) {
				toe(i, j);
			}
		}
		if (i == 10.5) {
			if (j == 3.0 || j == 2.5) {
				black(i, j);
			}
		}
	}
}

void jump1(GLdouble i) {
	GLdouble j;
	for (j = -13.0; j < 13.0; j += 0.5) {
		//上から下に置いていく
		if (i == -10.0) {
			if (j <= 4.5 && j >= 3.5 || j <= -3.0 && j >= -4.0) {
				black(i, j);
			}
		}
		if (i == -9.5) {
			if (j <= 4.5 && j >= 3.5 || j <= -3.0 && j >= -4.0) {
				ear_outside(i, j);
			}
			if (j == 5.5 || j == 5.0 || j == 3.0 || j == -2.5 || j == -4.5) {
				black(i, j);
			}
		}
		if (i == -9.0) {
			if (j <= 4.5 && j >= 3.5 || j <= -3.0 && j >= -4.0) {
				white(i, j);
			}
			if (j == 5.5 || j == 5.0 || j == 3.0 || j == -2.5 || j == -4.5) {
				ear_outside(i, j);
			}
			if (j == 6.5 ||j == 6.0 || j == 2.5 || j == -2.0  || j == -5.0) {
				black(i, j);
			}
		}
		if (i == -8.5) {
			if (j <= 5.0 && j >= 3.5 || j <= -2.5 && j >= -4.0) {
				white(i, j);
			}
			if (j <= 6.5 && j >= 5.5 || j == 3.0 || j == -2.0 || j == -4.5 || j == -5.0) {
				ear_outside(i, j);
			}
			if (j <= 8.5 && j >= 7.0 || j == 2.5 || j == -1.5 || j == -5.5) {
				black(i, j);
			}
		}
		if (i == -8.0) {
			if (j <= 6.5 && j >= 4.5 || j <= -3.0 && j >= -4.5) {
				white(i, j);
			}
			if (j <= 8.5 && j >= 7.0 || j <= 4.0 && j >= 3.0 || j <= -1.5 && j >= -2.5 || j == -5.0 || j == -5.5) {
				ear_outside(i, j);
			}
			if (j <= 10.5 && j >= 9.0 || j <= 2.5 && j >= -1.0 || j == -6.0) {
				black(i, j);
			}
		}
		if (i == -7.5) {
			if (j <= 7.5 && j >= 5.5 || j <= -3.5 && j >= -5.5) {
				white(i, j);
			}
			if (j <= 10.5 && j >= 8.0 || j == 5.0 || j == 4.5 || j == -2.5 || j == -3.0 || j == -6.0) {
				ear_outside(i, j);
			}
			if (j <= 4.0 && j >= -2.0) {
				hair_shadow(i, j);
			}
			if (j <= 12.0 && j >= 11.0 || j <= -6.5 && j >= -8.5) {
				black(i, j);
			}
		}
		if (i == -7.0) {
			if (j <= 10.0 && j >= 7.0 || j <= -3.5 && j >= -6.0) {
				white(i, j);
			}
			if (j <= 12.0 && j >= 10.5 || j == -3.0 || j <= -6.5 && j >= -8.5) {
				ear_outside(i, j);
			}
			if (j <= 4.0 && j >= -2.5) {
				hair(i, j);
			}
			if (j <= 6.5 && j >= 4.5) {
				hair_shadow(i, j);
			}
			if (j == 12.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == -6.5) {
			if (j <= 11.0 && j >= 8.5 || j <= -5.5 && j >= -7.5) {
				white(i, j);
			}
			if (j == 12.0 || j == 11.5 || j == 8.0 || j == 7.5 || j == -4.5 || j == -5.0 || j <= -8.0 && j >= -9.0) {
				ear_outside(i, j);
			}
			if (j <= 6.0 && j >= -4.0) {
				hair(i, j);
			}
			if (j == 7.0 || j == 6.5) {
				hair_shadow(i, j);
			}
			if (j == 12.5 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == -6.0) {
			if (j <= -6.5 && j >= -9.0) {
				white(i, j);
			}
			if (j <= 11.5 && j >= 8.0 || j <= -5.0 && j >= -6.0 || j == -9.5) {
				ear_outside(i, j);
			}
			if (j <= 6.5 && j >= -4.5) {
				hair(i, j);
			}
			if (j == 7.5 || j == 7.0) {
				hair_shadow(i, j);
			}
			if (j == 12.0 || j == 11.5 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == -5.5) {
			if (j <= -7.5 && j >= -9.5) {
				white(i, j);
			}
			if (j == 7.5 || j <= -6.0 && j >= -7.0 || j == -10.0) {
				ear_outside(i, j);
			}
			if (j <= 7.0 && j >= -5.5) {
				hair(i, j);
			}
			if (j == 8.0) {
				hair_shadow(i, j);
			}
			if (j <= 11.5 && j >= 8.5 || j == -10.5) {
				black(i, j);
			}
		}
		if (i == -5.0) {
			if (j <= -7.5 && j >= -9.5) {
				ear_outside(i, j);
			}
			if (j <= 7.5 && j >= -6.0) {
				hair(i, j);
			}
			if (j == 8.0 || j == -6.5 || j == -7.0 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == -4.5) {
			if (j <= 6.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j <= 7.5 && j >= 6.5) {
				hair_shadow(i, j);
			}
			if (j == 8.0 || j == -6.5 || j <= -7.5 && j >= -9.5) {
				black(i, j);
			}
		}
		if (i == -4.0) {
			if (j == 4.5 || j == 4.0 || j <= 3.0 && j >= 2.0 || j <= 0.5 && j >= -6.5) {
				hair(i, j);
			}
			if (j <= 6.0 && j >= 5.0) {
				hair_shadow(i, j);
			}
			if (j == 3.5 || j == 1.5 || j == 1.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= 7.5 && j >= 6.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == -3.5) {
			if (j == 6.0 || j <= -1.0 && j >= -7.0) {
				hair(i, j);
			}
			if (j == 5.5 || j == 5.0) {
				hair_shadow(i, j);
			}
			if (j <= 3.5 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 4.5 || j == 4.0 || j <= 1.5 && j >= -0.5) {
				eye_thick(i, j);
			}
			if (j == 6.5 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == -3.0) {
			if (j == 6.0 || j == 5.5 || j <= -0.5 && j >= -7.0) {
				hair(i, j);
			}
			if (j <= 3.5 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				eye_thick(i, j);
			}
			if (j == 4.5) {
				eye(i, j);
			}
			if (j == 6.5 || j == -7.5 || j == -8.0) {
				black(i, j);
			}
			if (j == 4.0 || j == 0.5 || j == 0.0) {
				white(i, j);
			}
		}
		if (i == -2.5) {
			if (j == 6.0 || j == 5.5 || j <= -0.5 && j >= -1.5 || j <= -2.5 && j >= -6.5 || j == -7.5 || j == -8.0) {
				hair(i, j);
			}
			if (j == -2.0) {
				hair_shadow(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 4.5 || j == 1.5 || j == 0.5) {
				eye(i, j);
			}
			if (j == 4.0 || j == 1.0) {
				eye_thick(i, j);
			}
			if (j == 6.5 || j == -7.0 || j == -8.5 || j == -9.0) {
				black(i, j);
			}
			if (j == 0.0) {
				white(i, j);
			}
		}
		if (i == -2.0) {
			if (j == 6.0 || j == 5.5 || j <= -0.5 && j >= -1.5 || j <= -2.5 && j >= -7.0 || j == -8.5 || j == -9.0) {
				hair(i, j);
			}
			if (j == -2.0 ) {
				hair_shadow(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 4.5 || j == 4.0 || j <= 1.5 && j >= 0.5) {
				eye(i, j);
			}
			if (j == 6.5 || j == -7.5 || j == -8.0 || j == -9.5 || j == -10.0) {
				black(i, j);
			}
			if (j == 0.0) {
				white(i, j);
			}
		}
		if (i == -1.5) {
			if (j == 6.0 || j == 5.5 || j <= -0.5 && j >= -2.0 || j <= -3.0 && j >= -8.0 || j == -9.5 || j == -10.0) {
				hair(i, j);
			}
			if (j == -2.5 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j <= 3.5 && j >= 2.0 || j == 0.0) {
				skin(i, j);
			}
			if (j == 4.5 || j == 4.0 || j <= 1.5 && j >= 0.5) {
				eye_thin(i, j);
			}
			if (j == 6.5 || j == -7.0 || j == -8.5 || j == -9.0 || j == -10.5) {
				black(i, j);
			}
			if (j == 5.0) {
				white(i, j);
			}
		}
		if (i == -1.0) {
			if (j == 6.0 || j <= -0.5 && j >= -2.0 || j <= -3.0 && j >= -7.0 || j <= -8.0 && j >= -9.0) {
				hair(i, j);
			}
			if (j == -2.5 || j == -7.5) {
				hair_shadow(i, j);
			}
			if (j == 5.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 5.0 && j >= 0.0) {
				skin(i, j);
			}
			if (j == 4.5 || j == 4.0 || j <= 1.5 && j >= 0.5) {
				eye(i, j);
			}
			if (j == 6.5 || j == -9.5 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == -0.5) {
			if (j == 6.0 || j == 0.0 || j <= -1.5 && j >= -2.5 || j <= -3.5 && j >= -7.0 || j <= -8.0 && j >= -10.0) {
				hair(i, j);
			}
			if (j == -2.5 || j == -0.5 || j == -1.0 || j == -3.0 || j == -7.5) {
				hair_shadow(i, j);
			}
			if (j == 5.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 5.0 && j >= 0.5) {
				skin(i, j);
			}
			if (j == 6.5 || j == -7.0 || j == -10.5 || j == -11.0) {
				black(i, j);
			}
		}
		if (i == 0.0) {
			if (j == 6.0 || j == 5.5 || j == -0.5 || j <= -2.0 && j >= -3.0 || j <= -5.0 && j >= -6.0 || j <= -7.5 && j >= -11.0) {
				hair(i, j);
			}
			if (j == 0.0 || j == -1.0 || j == -1.5 || j <= -3.5 && j >= -4.5 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 4.5 && j >= 3.0 || j <= 2.0 && j >= 0.5) {
				skin(i, j);
			}
			if (j == 2.5) {
				mouth(i, j);
			}
			if (j == 6.5 || j == -7.0 || j == -11.5) {
				black(i, j);
			}
		}
		if (i == 0.5) {
			if (j <= 6.0 && j >= 5.0 || j == -0.5 || j == -1.0 || j <= -2.5 && j >= -4.5 || j <= -5.5 && j >= -6.5 || j <= -8.5 && j >= -11.5) {
				hair(i, j);
			}
			if (j == 0.0 || j == -1.5 || j == -2.0 || j == -5.0 || j == -7.0) {
				hair_shadow(i, j);
			}
			if (j == 4.5 || j == 4.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 0.5) {
				skin(i, j);
			}
			if (j == 6.5 || j == -7.5 || j == -8.0 || j == -12.0) {
				black(i, j);
			}
		}
		if (i == 1.0) {
			if (j <= 6.0 && j >= 4.0 || j <= -0.5 && j >= -2.0 || j == -6.0 || j == -6.5 || j == -9.5) {
				hair(i, j);
			}
			if (j == 0.0 || j <= -2.5 && j >= -5.5 || j <= -7.0 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j <= 3.0 && j >= 1.5 || j == 0.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 3.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.0) {
				skin(i, j);
			}
			if (j == 6.5 || j == -8.0 || j == -9.0 || j <= -10.0 && j >= -11.5) {
				black(i, j);
			}
		}
		if (i == 1.5) {
			if (j == 6.0 || j <=4.5 && j >= 3.0 || j <= -1.0 && j >= -7.5) {
				hair(i, j);
			}
			if (j == 5.5 || j == 5.0 || j <= -8.0 && j >= -10.0) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == 0.5 || j == 0.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 2.5 || j == 1.0 || j == -0.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.5) {
				skin(i, j);
			}
			if (j == 6.5 || j == -10.5) {
				black(i, j);
			}
		}
		if (i == 2.0) {
			if (j <= -2.0 && j >= -7.5) {
				hair(i, j);
			}
			if (j == 6.0 || j == 5.5 || j == 4.0 || j == 3.5 || j == -8.0 || j == -8.5) {
				hair_shadow(i, j);
			}
			if (j == 3.0 || j == 2.0 || j == -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.5 || j <= 0.5 && j >= -0.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 6.5 || j == 5.0 || j == 4.5 || j == -1.5 || j <= -9.0 && j >= -10.0) {
				black(i, j);
			}
		}
		if (i == 2.5) {
			if (j <= -3.5 && j >= -5.5 || j == -7.5) {
				hair(i, j);
			}
			if (j == 6.0 || j == 5.5 || j <= -8.5 && j >= -9.5) {
				hair_shadow(i, j);
			}
			if (j == -0.5 || j == -1.0) {
				skin(i, j);
			}
			if (j == 3.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.5 || j == 0.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 2.0 || j == 1.0 || j == 0.5) {
				bunny_suit(i, j);
			}
			if (j == 1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 6.5 || j == 5.0 || j == 4.0 || j == 3.5 || j <= -2.0 && j >= -3.0 || j <= -6.0 && j >= -7.0 || j == -8.0 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == 3.0) {
			if (j <= -4.5 && j >= -6.5) {
				hair(i, j);
			}
			if (j == 6.0 || j == 5.5) {
				hair_shadow(i, j);
			}
			if (j == 2.5 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 0.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= -0.5 && j >= -1.5) {
				skin(i, j);
			}
			if (j <= 2.0 && j >= 0.5) {
				bunny_suit(i, j);
			}
			if (j == 6.5 || j == 5.0 || j == 3.0 || j == -2.5 || j == -3.5 || j == -4.0 || j == -7.0 || j == -7.5 || j <= -8.5 && j >= -9.5) {
				black(i, j);
			}
		}
		if (i == 3.5) {
			if (j <= -5.0 && j >= -6.5) {
				hair(i, j);
			}
			if (j == 6.0 || j == 5.5) {
				hair_shadow(i, j);
			}
			if (j == -0.5 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.0 || j == -1.5) {
				skin(i, j);
			}
			if (j == 1.0) {
				bunny_suit(i, j);
			}
			if (j == 2.0 || j == 1.5 || j == 0.5 || j == 0.0) {
				shadow1(i, j);
			}
			if (j == -2.0) {
				shadow2(i, j);
			}
			if (j == 6.5 || j == 5.0 || j == 2.5 || j == -3.0 || j == -4.0 || j == -4.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == 4.0) {
			if (j == -5.5 || j == -6.0) {
				hair(i, j);
			}
			if (j == 5.5) {
				hair_shadow(i, j);
			}
			if (j == -0.5 || j == -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.5 && j >= 0.5) {
				bunny_suit(i, j);
			}
			if (j == 2.0 || j== 0.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == -1.5 || j == -2.5) {
				shadow2(i, j);
			}
			if (j == 6.0 || j == 5.0 || j == 2.5 || j == -3.0 || j == -5.0 || j == -6.5) {
				black(i, j);
			}
			if (j == -2.0) {
				white(i, j);
			}
		}
		if (i == 4.5) {
			if (j <= 1.5 && j >= -0.5) {
				bunny_suit(i, j);
			}
			if (j == 2.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -3.0) {
				shadow2(i, j);
			}
			if (j == 5.5 || j == 2.5 || j == -3.5 || j == -5.5 || j == -6.0) {
				black(i, j);
			}
			if (j <= -1.5 && j >= -2.5) {
				white(i, j);
			}
		}
		if (i == 5.0) {
			if (j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.5 && j >= -0.5) {
				bunny_suit(i, j);
			}
			if (j == 2.0 || j== -1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == -2.5 || j == -3.0) {
				shadow2(i, j);
			}
			if (j == 3.0 || j == 2.5 || j == -3.5) {
				black(i, j);
			}
			if (j == -2.0) {
				white(i, j);
			}
		}
		if (i == 5.5) {
			if (j == -2.5 || j == -3.0) {
				skin(i, j);
			}
			if (j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= -0.5) {
				bunny_suit(i, j);
			}
			if (j == 1.5 || j == -1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 3.0 && j >= 2.0) {
				reg_shadow_thick(i, j);
			}
			if (j == -2.0) {
				shadow2(i, j);
			}
			if (j == 3.5 || j == -3.5) {
				black(i, j);
			}
		}
		if (i == 6.0) {
			if (j == -2.5) {
				skin(i, j);
			}
			if (j == -0.5 || j == -3.0) {
				skin_shadow_thin(i, j);
			}
			if (j == -1.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 0.5) {
				bunny_suit(i, j);
			}
			if (j == 1.5 || j == 1.0 || j == 0.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 2.5 || j == 2.0) {
				reg_shadow_thin(i, j);
			}
			if (j == 3.5) {
				reg_shadow_thick(i, j);
			}
			if (j == -2.0) {
				shadow2(i, j);
			}
			if (j == 4.0 || j == -3.5) {
				black(i, j);
			}
			if (j == 3.0) {
				white(i, j);
			}
		}
		if (i == 6.5) {
			if (j == 1.5 || j <= 0.0 && j >= -1.0 || j == -3.0) {
				skin_shadow_thin(i, j);
			}
			if (j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == -2.5 || j == 2.0 || j == 1.0) {
				reg_shadow_thin(i, j);
			}
			if (j == 3.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 4.0 || j == -2.0 || j == -3.5) {
				black(i, j);
			}
			if (j == 3.0 || j == 2.5) {
				white(i, j);
			}
		}
		if (i == 7.0) {
			if (j <= -0.5 && j >= -1.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 0.0 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 3.5 || j == 1.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 2.0) {
				reg_shadow_thin(i, j);
			}
			if (j == 4.0 || j == 1.0 || j == 0.5 || j == -2.5 || j == -3.0) {
				black(i, j);
			}
			if (j == 3.0 || j == 2.5) {
				white(i, j);
			}
		}
		if (i == 7.5) {
			if (j == -0.5 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= -1.0 && j >= -2.0) {
				skin(i, j);
			}
			if (j == 1.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 3.0 || j == 1.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 3.5 || j == 0.5 || j == 0.0 || j == -3.0) {
				black(i, j);
			}
			if (j == 2.5 || j == 2.0) {
				white(i, j);
			}
		}
		if (i == 8.0) {
			if (j <= -1.5 && j >= -2.5) {
				skin(i, j);
			}
			if (j == -1.0 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.5 || j == 1.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 3.0 || j == 0.5 || j == -0.5 || j == -3.5) {
				black(i, j);
			}
			if (j == 2.0 || j== 1.5) {
				white(i, j);
			}
		}
		if (i == 8.5) {
			if (j == -2.0 || j == -2.5) {
				skin(i, j);
			}
			if (j == -1.5 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 2.5 || j == 0.5 || j == -1.0 || j == -2.5 || j == -3.5) {
				black(i, j);
			}
			if (j == 1.5) {
				white(i, j);
			}
			if (j == 1.0) {
				toe(i, j);
			}
		}
		if (i == 9.0) {
			if (j == -2.0 || j == -2.5) {
				skin(i, j);
			}
			if (j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0 || j == 1.0 || j == -1.0 || j == -3.5) {
				black(i, j);
			}
			if (j == 1.5 || j == -3.0) {
				toe(i, j);
			}
		}
		if (i == 9.5) {
			if (j == 1.5 || j == -1.5 || j == -3.0) {
				black(i, j);
			}
			if (j == -2.0 || j == -2.5) {
				toe(i, j);
			}
		}
		if (i == 10.0) {
			if (j == -2.0 || j == -2.5) {
				black(i, j);
			}
		}
	}
}

void jump2(GLdouble i) {
	GLdouble j;
	for (j = -13.0; j < 13.0; j += 0.5) {
		//上から下に置いていく
		if (i == -9.5) {
			if (j <= -1.0 && j >= -2.5) {
				black(i, j);
			}
		}
		if (i == -9.0) {
			if (j <= -1.0 && j >= -2.5) {
				ear_outside(i, j);
			}
			if (j <= 6.0 && j >= 2.5 || j == -0.5 || j == -3.0) {
				black(i, j);
			}
		}
		if (i == -8.5) {
			if (j <= -1.0 && j >= -2.0) {
				white(i, j);
			}
			if (j <= 6.0 && j >= 2.5 || j == -0.5 || j == -2.5) {
				ear_outside(i, j);
			}
			if (j == 6.5 || j == 2.0 || j == 0.0 || j == -3.0 || j == -3.5) {
				black(i, j);
			}
		}
		if (i == -8.0) {
			if (j <= 6.0 && j >= 3.5 || j <= -1.5 && j >= -2.5) {
				white(i, j);
			}
			if (j == 6.5 || j <= 3.0 && j >= 2.0 || j <= 0.0 && j >= -1.0 || j == -3.0 || j == -3.5) {
				ear_outside(i, j);
			}
			if (j == 7.5 || j == 7.0 || j <= 1.5 && j >= 0.5 || j == -4.0 || j == -4.5) {
				black(i, j);
			}
		}
		if (i == -7.5) {
			if (j <= 6.5 && j >= 4.0 || j <= -2.0 && j >= -3.0) {
				white(i, j);
			}
			if (j == 7.5 || j == 7.0 || j == 3.5 || j <= -0.5 && j >= -1.5 || j <= -3.5 && j >= -4.5) {
				ear_outside(i, j);
			}
			if (j == 2.0) {
				hair(i, j);
			}
			if (j == 3.0 || j == 2.5 || j <= 1.5 && j >= 0.0) {
				hair_shadow(i, j);
			}
			if (j <= 9.5 && j >= 8.0 || j <= -5.0 && j >= -6.5) {
				black(i, j);
			}
		}
		if (i == -7.0) {
			if (j <= 7.5 && j >= 4.5 || j <= -2.0 && j >= -4.0) {
				white(i, j);
			}
			if (j <= 9.5 && j >= 7.5 || j == 4.0 || j == 3.5 || j == -1.0 || j == -1.5 || j <= -4.5 && j >= -6.0) {
				ear_outside(i, j);
			}
			if (j <= 3.0 && j >= -0.5) {
				hair(i, j);
			}
			if (j == 10.5 || j == 10.0 || j == -6.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == -6.5) {
			if (j <= 9.5 && j >= 6.0 || j <= -3.0 && j >= -6.0) {
				white(i, j);
			}
			if (j == 10.5 || j == 10.0 || j <= 5.5 && j >= 4.5 || j == -2.0 || j == -2.5 || j == -6.5 || j == -7.0) {
				ear_outside(i, j);
			}
			if (j <= 4.0 && j >= -1.5) {
				hair(i, j);
			}
			if (j <= 6.5 && j >= 4.5) {
				hair_shadow(i, j);
			}
			if (j == 11.0 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == -6.0) {
			if (j <= 10.5 && j >= 7.0 || j <= -3.5 && j >= -7.0) {
				white(i, j);
			}
			if (j == 11.0 || j <= 6.5 && j >= 5.5 || j == -2.5 || j == -3.0 || j == -7.5) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= -2.0) {
				hair(i, j);
			}
			if (j == 7.0 || j == 6.5) {
				hair_shadow(i, j);
			}
			if (j == 11.5 || j == -8.0) {
				black(i, j);
			}
		}
		if (i == -5.5) {
			if (j <= 10.5 && j >= 8.0 || j <= -4.5 && j >= -7.5) {
				white(i, j);
			}
			if (j == 11.0 || j <= 7.5 && j >= 6.0 || j == -3.5 || j == -4.0 || j == -8.0) {
				ear_outside(i, j);
			}
			if (j <= 5.5 && j >= -3.0) {
				hair(i, j);
			}
			if (j == 7.5 || j == 7.0) {
				hair_shadow(i, j);
			}
			if (j == 11.5 || j == -8.5) {
				black(i, j);
			}
		}
		if (i == -5.0) {
			if (j <= 9.0 && j >= 8.0 || j <= -5.5 && j >= -8.0) {
				white(i, j);
			}
			if (j <= 10.5 && j >= 9.5 || j <= 7.5 && j >= 6.5 || j == -4.5 || j == -5.0 || j == -8.5) {
				ear_outside(i, j);
			}
			if (j <= 6.0 && j >= -4.0) {
				hair(i, j);
			}
			if (j == 8.0) {
				hair_shadow(i, j);
			}
			if (j <= 11.0 && j >= 8.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == -4.5) {
			if (j <= 9.0 && j >= 8.0 || j <= -5.0 && j >= -7.0 || j == -8.0 || j == -8.5) {
				ear_outside(i, j);
			}
			if (j <= 6.0 && j >= -4.0) {
				hair(i, j);
			}
			if (j == 6.5) {
				hair_shadow(i, j);
			}
			if (j <= 10.5 && j >= 9.5 || j == 7.5 || j == 7.0 || j == -4.5 || j == -9.0) {
				black(i, j);
			}
			if (j == -7.5) {
				white(i, j);
			}
		}
		if (i == -4.0) {
			if (j <= -6.0 && j >= -8.0) {
				ear_outside(i, j);
			}
			if (j <= 6.0 && j >= -4.5) {
				hair(i, j);
			}
			if (j == 6.5) {
				hair_shadow(i, j);
			}
			if (j <= 9.0 && j >= 8.0 || j == 7.0 || j == -5.0 || j == -5.5 || j == -8.5) {
				black(i, j);
			}
		}
		if (i == -3.5) {
			if (j <= 5.5 && j >= 4.5 || j <= 3.5 && j >= -4.5) {
				hair(i, j);
			}
			if (j == 6.5 || j == 6.0) {
				hair_shadow(i, j);
			}
			if (j == 4.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 7.0 || j == -5.0 || j <= -6.0 && j >= -8.0) {
				black(i, j);
			}
		}
		if (i == -3.0) {
			if (j == 4.5 || j == 3.5 || j <= 1.5 && j >= 0.5 || j <= -0.5 && j >= -5.0) {
				hair(i, j);
			}
			if (j == 5.5 || j == 5.0 || j == 0.0) {
				hair_shadow(i, j);
			}
			if (j <= 3.0 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 4.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 6.5 || j == 6.0 || j == -5.5) {
				black(i, j);
			}
		}
		if (i == -2.5) {
			if (j == 5.0 || j <= 1.5 && j >= 0.5 || j <= -0.5 && j >= -5.5) {
				hair(i, j);
			}
			if (j == 5.5 || j == 0.0) {
				hair_shadow(i, j);
			}
			if (j <= 3.5 && j >= 2.0) {
				skin(i, j);
			}
			if (j == 4.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 4.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				eye_thick(i, j);
			}
			if (j == 4.5) {
				eye(i, j);
			}
			if (j == 6.0 || j == -6.0) {
				black(i, j);
			}
			if (j == 4.0 || j == 0.5 || j == 0.0) {
				white(i, j);
			}
		}
		if (i == -2.0) {
			if (j == 5.5 || j == 5.0 || j <= -1.0 && j >= -2.0 || j <= -3.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == -2.5) {
				hair_shadow(i, j);
			}
			if (j == 4.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.0 && j >= 1.5) {
				skin(i, j);
			}
			if (j == 4.0 || j == 3.5 || j <= 1.0 && j >= -0.5) {
				eye_thick(i, j);
			}
			if (j == 6.0 || j <= -6.5 && j >= -8.0) {
				black(i, j);
			}
		}
		if (i == -1.5) {
			if (j == 5.5 || j == 5.0 || j <= -1.0 && j >= -2.0 || j <= -3.0 && j >= -8.0) {
				hair(i, j);
			}
			if (j == -2.5) {
				hair_shadow(i, j);
			}
			if (j == 4.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 3.0 && j >= 1.5) {
				skin(i, j);
			}
			if (j == 4.0) {
				eye(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				eye_thick(i, j);
			}
			if (j == 6.0 || j == -8.5) {
				black(i, j);
			}
			if (j == 3.5 || j == 0.0 || j == -0.5) {
				white(i, j);
			}
		}
		if (i == -1.0) {
			if (j == 5.5 || j == 5.0 || j <= -1.0 && j >= -2.5 || j <= -4.0 && j >= -7.0 || j == -8.5) {
				hair(i, j);
			}
			if (j == -3.0 || j == -3.5) {
				hair_shadow(i, j);
			}
			if (j <= 3.0 && j >= 1.5) {
				skin(i, j);
			}
			if (j == 4.0 || j == 1.0 || j == 0.0) {
				eye(i, j);
			}
			if (j == 3.5 || j == 0.5){
				eye_thick(i, j);
			}
			if (j == 6.0 || j == -7.5 || j == -8.0 || j == -9.0) {
				black(i, j);
			}
			if (j == 4.5 || j == -0.5) {
				white(i, j);
			}
		}
		if (i == -0.5) {
			if (j == 5.5 || j <= -1.0 && j >= -3.5 || j <= -5.5 && j >= -7.5) {
				hair(i, j);
			}
			if (j == -2.5 || j <= -4.0 && j >= -5.0) {
				hair_shadow(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 4.5 || j <= 3.0 && j >= 1.5) {
				skin(i, j);
			}
			if (j == 4.0 || j == 3.5 || j <= 1.0 && j >= 0.0) {
				eye(i, j);
			}
			if (j == 6.0 || j == -8.0 || j == -8.5) {
				black(i, j);
			}
			if (j == -0.5) {
				white(i, j);
			}
		}
		if (i == 0.0) {
			if (j == 6.0 || j == 5.5 || j == -0.5 || j <= -2.0 && j >= -4.5 || j <= -5.5 && j >= -6.5 || j == -8.0) {
				hair(i, j);
			}
			if (j == -2.5 || j == -1.0 || j == -1.5 || j == -5.0) {
				hair_shadow(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 4.5 && j >= 0.0) {
				skin(i, j);
			}
			if (j == 6.5 || j == -7.0 || j == -7.5 || j == -8.5) {
				black(i, j);
			}
		}
		if (i == 0.5) {
			if (j == 6.0 || j == 5.5 || j ==-1.5 || j <= -5.0 && j >= -7.0 || j == -8.5) {
				hair(i, j);
			}
			if (j == -0.5 || j == -1.0 || j <= -2.0 && j >= -4.5) {
				hair_shadow(i, j);
			}
			if (j == 5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 4.5 && j >= 0.0 || j <= 2.0 && j >= 0.5) {
				skin(i, j);
			}
			if (j == 6.5 || j == -7.5 || j == -8.0 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 1.0) {
			if (j <= 6.5 && j >= 5.0 || j <= -1.5 && j >= -7.5) {
				hair(i, j);
			}
			if (j == -0.5 || j == -1.0) {
				hair_shadow(i, j);
			}
			if (j == 4.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 4.0 && j >= 2.5 || j <= 1.5 && j >= 0.0) {
				skin(i, j);
			}
			if (j == 2.0) {
				mouth(i, j);
			}
			if (j == 7.0 || j == -7.5 || j == -8.0 || j == -8.5) {
				black(i, j);
			}
		}
		if (i == 1.5) {
			if (j <= 7.0 && j >= 6.0 || j == 5.0 || j == 4.5 || j <= -1.5 && j >= -7.5) {
				hair(i, j);
			}
			if (j == 5.5 || j == -0.5 || j == -1.0) {
				hair_shadow(i, j);
			}
			if (j == 3.0 || j == 2.5 || j == 1.0 || j == 0.5) {
				skin(i, j);
			}
			if (j == 4.0 || j == 3.5 || j == 0.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0 || j == 1.5) {
				mouth(i, j);
			}
			if (j == 7.5 || j == -8.0) {
				black(i, j);
			}
		}
		if (i == 2.0) {
			if (j <= 7.5 && j >= 6.5 || j <= 5.0 && j >= 3.5 || j <= -1.5 && j >= -6.5) {
				hair(i, j);
			}
			if (j == 6.0 || j == 5.5 || j <= -7.0 && j >= -8.0) {
				hair_shadow(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				skin_shadow_thin(i, j);
			}
			if (j <= 3.0 && j >= 1.5 || j == 0.5 || j == -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0) {
				skin(i, j);
			}
			if (j == 8.0 || j == -8.5) {
				black(i, j);
			}
		}
		if (i == 2.5) {
			if (j <= 8.0 && j >= 7.0 || j == 5.0 || j == 4.5 || j <= -2.0 && j >= -5.5) {
				hair(i, j);
			}
			if (j == 6.0 || j == 5.5 || j <= -6.0 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j == 1.0 || j == -0.5) {
				skin(i, j);
			}
			if (j == 4.0 || j == 3.5 || j == -1.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 0.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 3.0 || j == 2.5) {
				bunny_suit(i, j);
			}
			if (j == 2.0 || j == 1.5 || j == 0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 8.5 || j == 6.5 || j == -1.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 3.0) {
			if (j == 8.5 || j == 8.0 || j == 5.0 || j == -4.0 || j == -4.5) {
				hair(i, j);
			}
			if (j <= 6.5 && j >= 5.5 || j <= -6.0 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j == 4.0 || j == 3.5 || j == -0.5 || j == -1.5) {
				skin(i, j);
			}
			if (j == 4.5 || j == 3.0 || j == 0.0 || j == -1.0 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0 || j == 1.5 || j == 0.5) {
				bunny_suit(i, j);
			}
			if (j == 2.5 || j == 1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 9.0 || j == 7.5 || j == 7.0 || j <= -2.5 && j >= -3.5 || j == -5.0 || j == -5.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 3.5) {
			if (j == -5.0) {
				hair(i, j);
			}
			if (j <= 6.5 && j >= 5.0 || j <= -7.5 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j == 4.0 || j == -0.5) {
				skin(i, j);
			}
			if (j == 4.5 || j == 3.0 || j == 0.0 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= 1.5 && j >= 0.5) {
				bunny_suit(i, j);
			}
			if (j == 2.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 2.0) {
				shadow1(i, j);
			}
			if (j == 3.5 || j== -1.0 || j == -1.5) {
				shadow2(i, j);
			}
			if (j == 8.5 || j == 8.0 || j == 7.0 || j == -3.0 || j == -4.0 || j == -4.5 || j <= -5.5 && j >= -7.0 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 4.0) {
			if (j == -5.5 || j == -6.0) {
				hair(i, j);
			}
			if (j <= 6.5 && j >= 5.5 || j == -8.5 || j == -9.0) {
				hair_shadow(i, j);
			}
			if (j == 0.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 3.0 || j == -0.5) {
				skin(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				bunny_suit(i, j);
			}
			if (j == 2.5 || j == 2.0 || j == 0.5 || j == -1.5) {
				shadow1(i, j);
			}
			if (j == 4.0) {
				shadow2(i, j);
			}
			if (j == 7.0 || j == 5.0 || j == 4.5 || j == -2.5 || j == -5.0 || j == -6.5 ||  j== -7.5 || j == -8.0 || j == -9.5) {
				black(i, j);
			}
			if (j == 3.5 || j == -1.0) {
				white(i, j);
			}
		}
		if (i == 4.5) {
			if (j == -9.0) {
				hair(i, j);
			}
			if (j <= 7.0 && j >= 6.0) {
				hair_shadow(i, j);
			}
			if (j == 3.5 || j == 3.0 || j == -1.0) {
				skin(i, j);
			}
			if (j == -0.5 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.5 && j >= 0.0) {
				bunny_suit(i, j);
			}
			if (j == 2.0 || j == 0.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 4.0 || j == -2.0) {
				shadow2(i, j);
			}
			if (j == 7.5 || j == 5.5 || j == 4.5 || j == -2.5 || j == -5.5 || j == -6.0 || j == -8.5 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == 5.0) {
			if (j <= 2.5 && j >= 0.0) {
				bunny_suit(i, j);
			}
			if (j == 7.0 || j == 6.5) {
				hair_shadow(i, j);
			}
			if (j == 3.5 || j == 3.0 || j <= -0.5 && j >= -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -2.0) {
				shadow2(i, j);
			}
			if (j == 7.5 || j == 6.0 || j == 4.0 || j == -2.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 5.5) {
			if (j == 7.5 || j == 7.0) {
				hair_shadow(i, j);
			}
			if (j <= 1.0 && j >= -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.5 && j >= 1.5) {
				bunny_suit(i, j);
			}
			if (j == 2.0 || j == -1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 3.0) {
				reg_shadow_thick(i, j);
			}
			if (j == -2.5 ) {
				shadow2(i, j);
			}
			if (j == 8.0 || j == 6.5 || j == 3.5 || j == -3.0) {
				black(i, j);
			}
			if (j == -1.5 || j == -2.0) {
				white(i, j);
			}
		}
		if (i == 6.0) {
			if (j == 2.5 || j <= 1.0 && j >= -0.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.5 || j == -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0) {
				bunny_suit(i, j);
			}
			if (j == 3.0) {
				reg_shadow_thick(i, j);
			}
			if (j == -2.5) {
				shadow2(i, j);
			}
			if (j == 7.5 || j == 7.0 || j == 3.5 || j == -3.0) {
				black(i, j);
			}
			if (j == -1.5 || j == -2.0) {
				white(i, j);
			}
		}
		if (i == 6.5) {
			if (j == 2.5 || j == 1.5 || j == 1.0 || j <= 0.0 && j >= -1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 2.0 || j == 0.5 || j== -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 0.5) {
				bunny_suit(i, j);
			}
			if (j == 1.5 || j == 1.0 || j == 0.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 3.0) {
				reg_shadow_thick(i, j);
			}
			if (j == -2.0) {
				shadow2(i, j);
			}
			if (j == 3.5 ||  j== -2.5) {
				black(i, j);
			}
		}
		if (i == 7.0) {
			if (j == 1.0 || j == 0.5) {
				skin(i, j);
			}
			if (j == 1.5 || j <= 0.0 && j >= -1.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 2.0 || j== -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 2.5) {
				reg_shadow_thin(i, j);
			}
			if (j == 3.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 3.5 || j == -2.0 || j == -2.5) {
				black(i, j);
			}
		}
		if (i == 7.5) {
			if (j <= 1.0 && j >= 0.0) {
				skin(i, j);
			}
			if (j == -1.0 || j == -1.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.5 || j == -0.5 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 3.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 2.0) {
				reg_shadow_thin(i, j);
			}
			if (j == 3.5 || j == -2.5) {
				black(i, j);
			}
			if (j == 2.5) {
				white(i, j);
			}
		}
		if (i == 8.0) {
			if (j <= 0.5 && j >= -0.5) {
				skin(i, j);
			}
			if (j == 1.0 || j == -1.0 || j== -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 3.0 || j == 1.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 3.5 || j == 0.5 || j == 0.0 || j == -2.0) {
				black(i, j);
			}
			if (j == 2.5 || j == 2.0) {
				white(i, j);
			}
		}
		if (i == 8.5) {
			if (j == 0.0 || j == -0.5) {
				skin(i, j);
			}
			if (j == 0.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 3.0 || j == 1.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 3.5 || j == -1.5) {
				black(i, j);
			}
			if (j <= 2.5 && j >= 1.5) {
				white(i, j);
			}
			if (j == -1.0) {
				toe(i, j);
			}
		}
		if (i == 9.0) {
			if (j == -0.5) {
				skin(i, j);
			}
			if (j == 0.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 3.0 || j == 1.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 3.5 || j == 1.0 || j == 0.5 || j == -1.5) {
				black(i, j);
			}
			if (j == 2.5 || j == 2.0) {
				white(i, j);
			}
			if (j == -1.0) {
				toe(i, j);
			}
		}
		if (i == 9.5) {
			if (j == 3.5 || j == 1.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 4.0 || j == 1.0 || j == 0.0 || j == -1.0) {
				black(i, j);
			}
			if (j <= 3.0 && j >= 2.0) {
				white(i, j);
			}
			if (j == -0.5) {
				toe(i, j);
			}
		}
		if (i == 10.0) {
			if (j == 3.5 || j == 2.0 || j == 1.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 4.0 || j == 1.0 || j == -0.5) {
				black(i, j);
			}
			if (j == 3.0 || j == 2.5) {
				white(i, j);
			}
		}
		if (i == 10.5) {
			if (j == 3.5 || j == 2.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 4.0 || j == 1.5) {
				black(i, j);
			}
			if (j == 3.0 || j == 2.5) {
				white(i, j);
			}
		}
		if (i == 11.0) {
			if (j == 3.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 4.0 || j == 2.0) {
				black(i, j);
			}
			if (j == 3.0) {
				white(i, j);
			}
			if (j == 2.5) {
				toe(i, j);
			}
		}
		if (i == 11.5) {
			if (j == 3.5 || j == 2.0) {
				black(i, j);
			}
			if (j == 3.0 || j == 2.5) {
				toe(i, j);
			}
		}
		if (i == 12.0) {
			if (j == 3.0 || j == 2.5) {
				black(i, j);
			}
		}
	}
}

void attack1(GLdouble i) {
	GLdouble j;
	for (j = -12.0; j < 12.0; j += 0.5) {
		//上から下に置いていく
		if (i == -10.5) {
			if (j == 3.0 || j == 2.5 || j <= -2.5 && j >= -3.5) {
				black(i, j);
			}
		}
		if (i == -10.0) {
			if (j == 3.0 || j == 2.5 || j <= -2.5 && j >= -3.5) {
				ear_outside(i, j);
			}
			if (j == 3.5 || j == 2.0 || j == -2.0 || j == -4.0) {
				black(i, j);
			}
		}
		if (i == -9.5) {
			if (j == 3.0 || j == 2.5 || j <= -2.5 && j >= -3.5) {
				white(i, j);
			}
			if (j == 3.5 || j == 2.0 || j == -2.0 || j == -4.0) {
				ear_outside(i, j);
			}
			if (j == 4.0 || j == 1.5 || j == -1.5 || j == -4.5 || j == -5.0) {
				black(i, j);
			}
		}
		if (i == -9.0) {
			if (j <= 3.5 && j >= 2.0 || j <= -2.0 && j >= -4.0) {
				white(i, j);
			}
			if (j == 4.0 || j == 1.5 || j == -1.5 || j == -4.5 || j == -5.0) {
				ear_outside(i, j);
			}
			if (j == 4.5 || j == 1.0 || j == -1.0 || j == -5.5) {
				black(i, j);
			}
		}
		if (i == -8.5) {
			if (j <= 3.5 && j >= 1.5 || j <= -1.5 && j >= -5.0) {
				white(i, j);
			}
			if (j == 4.5 || j == 4.0 || j == 1.0 || j == -1.0 || j == -5.5) {
				ear_outside(i, j);
			}
			if (j == 5.5 || j == 5.0 || j <= 0.5 && j >= -0.5 || j == -6.0) {
				black(i, j);
			}
		}
		if (i == -8.0) {
			if (j <= 4.0 && j >= 3.0 || j <= -1.5 && j >= -5.5) {
				white(i, j);
			}
			if (j <= 5.5 && j >= 4.5 || j <= 2.5 && j >= 1.5 || j == -0.5 || j == -1.0 || j == -6.0) {
				ear_outside(i, j);
			}
			if (j <= 1.0 && j >= 0.0) {
				hair_shadow(i, j);
			}
			if (j == 6.0 || j == -6.5) {
				black(i, j);
			}
		}
		if (i == -7.5) {
			if (j <= 5.5 && j >= 3.0 || j <= -2.0 && j >= -6.0) {
				white(i, j);
			}
			if (j == 6.0 || j == 2.5 || j == 2.0 || j == -1.5 || j == -6.5) {
				ear_outside(i, j);
			}
			if (j <= 1.0 && j >= -1.0) {
				hair(i, j);
			}
			if (j == 1.5) {
				hair_shadow(i, j);
			}
			if (j == 6.5 || j <= -7.0 && j >= -8.0) {
				black(i, j);
			}
		}
		if (i == -7.0) {
			if (j <= 6.0 && j >= 3.5 || j <= -2.5 && j >= -6.5) {
				white(i, j);
			}
			if (j == 6.5 || j == -2.0 || j <= -7.0 && j >= -8.0) {
				ear_outside(i, j);
			}
			if (j <= 2.5 && j >= -1.5) {
				hair(i, j);
			}
			if (j == 3.0) {
				hair_shadow(i, j);
			}
			if (j == 7.5 || j == 7.0 || j == -8.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == -6.5) {
			if (j <= 6.5 && j >= 4.5 || j <= -3.5 && j >= -7.5) {
				white(i, j);
			}
			if (j == 7.5 || j == 7.0 || j == 4.0 || j == -2.5 || j == -3.0 || j <= -8.0 && j >= -9.0) {
				ear_outside(i, j);
			}
			if (j <= 3.0 && j >= -2.0) {
				hair(i, j);
			}
			if (j == 3.5) {
				hair_shadow(i, j);
			}
			if (j == 8.0 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == -6.0) {
			if (j <= 7.0 && j >= 4.5 || j <= -4.0 && j >= -9.0) {
				white(i, j);
			}
			if (j == 8.0 || j == 7.5 || j == 4.0 || j == -3.5 || j == -9.5) {
				ear_outside(i, j);
			}
			if (j <= 3.0 && j >= -2.0) {
				hair(i, j);
			}
			if (j == 3.5 || j == -2.5 || j == -3.0) {
				hair_shadow(i, j);
			}
			if (j == 8.5 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == -5.5) {
			if (j <= 8.0 && j >= 5.0 || j <= -5.0 && j >= -9.0) {
				white(i, j);
			}
			if (j == 8.5 || j == 4.5 || j == -4.0 || j == -4.5 || j == -9.5) {
				ear_outside(i, j);
			}
			if (j <= 3.5 && j >= -3.0) {
				hair(i, j);
			}
			if (j == 4.0 || j == -3.5) {
				hair_shadow(i, j);
			}
			if (j == 9.0 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == -5.0) {
			if (j <= 8.5 && j >= 5.0 || j <= -6.0 && j >= -8.0) {
				white(i, j);
			}
			if (j == 9.0 || j == 4.5 || j == -5.0 || j == -5.5 || j == -8.5 || j == -9.0) {
				ear_outside(i, j);
			}
			if (j <= 4.0 && j >= -4.0) {
				hair(i, j);
			}
			if (j == -4.5) {
				hair_shadow(i, j);
			}
			if (j == 10.0 || j == 9.5 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == -4.5) {
			if (j <= 9.0 && j >= 5.5) {
				white(i, j);
			}
			if (j == 10.0 || j == 9.5 || j == 5.0 || j == 4.5 || j <= -5.5 && j >= -8.5) {
				ear_outside(i, j);
			}
			if (j <= 3.0 && j >= 0.0 || j <= -1.0 && j >= -4.0) {
				hair(i, j);
			}
			if (j == 4.0 || j == 3.5 || j == -4.5) {
				hair_shadow(i, j);
			}
			if (j == -0.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 10.5 || j == -5.0 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == -4.0) {
			if (j <= 9.0 && j >= 6.0) {
				white(i, j);
			}
			if (j == 9.5 || j <= 5.5 && j >= 4.5) {
				ear_outside(i, j);
			}
			if (j == 3.5 || j == 2.5 || j == 2.0 || j <= 1.0 && j >= 0.0 || j <= -1.5 && j >= -4.5) {
				hair(i, j);
			}
			if (j == 4.0 || j == 3.0) {
				hair_shadow(i, j);
			}
			if (j == -0.5) {
				skin(i, j);
			}
			if (j == 1.5 || j == -1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 10.0 || j <= -5.0 && j >= -8.5) {
				black(i, j);
			}
		}
		if (i == -3.5) {
			if (j <= 9.0 && j >= 6.0) {
				ear_outside(i, j);
			}
			if (j == 4.0 || j == 3.5 || j == 1.0 || j == 0.0 || j <= -1.5 && j >= -4.5) {
				hair(i, j);
			}
			if (j == 3.0 || j == -5.0) {
				hair_shadow(i, j);
			}
			if (j == 0.5 || j == -0.5 || j == -1.0) {
				skin(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0 || j == 1.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 9.5 || j <= 5.5 && j >= 4.5 || j == -5.5) {
				black(i, j);
			}
		}
		if (i == -3.0) {
			if (j == 3.5 || j == 3.0 || j <= -2.0 && j >= -4.5) {
				hair(i, j);
			}
			if (j == -5.0 || j == -5.5) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= 1.5 && j >= -0.5) {
				skin(i, j);
			}
			if (j == -1.0 || j == -1.5) {
				eye_thick(i, j);
			}
			if (j <= 9.0 && j >= 6.0 || j == 4.0 || j == -6.0) {
				black(i, j);
			}
		}
		if (i == -2.5) {
			if (j <= 4.0 && j >= 3.0 || j <= -1.5 && j >= -5.5) {
				hair(i, j);
			}
			if (j == -4.5 || j == -6.0) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= 1.5 && j >= 0.0 || j == -1.0) {
				skin(i, j);
			}
			if (j == -0.5) {
				eye_thick(i, j);
			}
			if (j == 4.5 || j == -6.5) {
				black(i, j);
			}
		}
		if (i == -2.0) {
			if (j <= 4.0 && j >= 3.0 || j <= -1.5 && j >= -5.5) {
				hair(i, j);
			}
			if (j == -6.0 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= 1.5 && j >= 0.5 || j == -0.5 || j == -1.0) {
				skin(i, j);
			}
			if (j == 0.0) {
				eye_thick(i, j);
			}
			if (j == 4.5 ||  j== -7.0) {
				black(i, j);
			}
		}
		if (i == -1.5) {
			if (j <= 4.0 && j >= 3.0 || j <= -1.5 && j >= -6.5) {
				hair(i, j);
			}
			if (j == -7.0) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= 0.0) {
				skin(i, j);
			}
			if (j == -0.5 || j == -1.0) {
				eye_thick(i, j);
			}
			if (j ==4.5 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == -1.0) {
			if (j == 4.0 || j == 3.5 || j <= -1.5 && j >= -3.5 || j <= -5.0 && j >= -7.0) {
				hair(i, j);
			}
			if (j == -4.0 || j == -4.5 || j == -7.5) {
				hair_shadow(i, j);
			}
			if (j == 3.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.5 && j >= -1.0) {
				skin(i, j);
			}
			if (j == 4.5 || j == -8.0) {
				black(i, j);
			}
		}
		if (i == -0.5) {
			if (j <= 4.5 && j >= 3.5 || j <= -1.0 && j >= -2.0 || j == -3.0 || j == -3.5 || j <= -4.5 && j >= -7.5) {
				hair(i, j);
			}
			if (j == -2.5 || j == -4.0 || j == -8.0) {
				hair_shadow(i, j);
			}
			if (j == 3.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.5 && j >= -0.5) {
				skin(i, j);
			}
			if (j == 5.0 || j == -8.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 0.0) {
			if (j == 4.5 || j == 4.0 || j == 3.0 || j == -1.0 || j == -1.5 || j == -2.5 || j == -3.0 || j <= -5.0 && j >= -8.5) {
				hair(i, j);
			}
			if (j == 3.5 || j == -2.0 || j <= -3.5 && j >= -4.5 || j == -9.0) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= 1.0 || j == 0.0 || j == -0.5) {
				skin(i, j);
			}
			if (j == 0.5) {
				mouth(i, j);
			}
			if (j == 5.0 || j == -7.0 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == 0.5) {
			if (j == 4.5 || j == 4.0 || j == 3.0 || j == 2.5 || j <= -2.0 && j >= -3.0 || j <= -4.5 && j >= -6.5 || j <= -8.0 && j >= -9.0) {
				hair(i, j);
			}
			if (j == 3.5 || j <= -0.5 && j >= -1.5 || j == -3.5 || j == -4.0 || j == -7.0) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == 1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0) {
				skin(i, j);
			}
			if (j == 0.5 || j == 0.0) {
				mouth(i, j);
			}
			if (j == 5.0 || j == -7.5 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == 1.0) {
			if (j == 4.5 || j == 4.0 || j == 3.0 || j == 2.0 || j == 1.5 || j <= -1.0 && j >= -2.5 || j <= -4.0 && j >= -7.0) {
				hair(i, j);
			}
			if (j == 3.5 || j == 2.5 || j == -3.0 || j == -3.5 || j == -7.5) {
				hair_shadow(i, j);
			}
			if (j <= 1.0 && j >= -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 0.5 && j >= -1.0) {
				skin(i, j);
			}
			if (j == -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 5.0 || j <= -8.0 && j >= -9.0) {
				black(i, j);
			}
		}
		if (i == 1.5) {
			if (j <= 4.5 && j >= 3.5 || j == 2.5 || j <= -4.0 && j >= -8.0) {
				hair(i, j);
			}
			if (j == 5.0 || j == 3.0 || j == 2.0 || j == -2.0 || j == -3.0 || j == -3.5) {
				hair_shadow(i, j);
			}
			if (j <= 1.5 && j >= -0.5 || j == -1.5) {
				skin_shadow_thin(i, j);
			}
			if (j == -1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 5.5 || j == -2.5 || j == -8.5) {
				black(i, j);
			}
		}
		if (i == 2.0) {
			if (j <= 4.5 && j >= 3.0 || j <= -4.0 && j >= -7.0 || j == -8.5) {
				hair(i, j);
			}
			if (j == 5.0 || j == -3.0 || j == -3.5 || j == -7.5 || j == -8.0) {
				hair_shadow(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				skin(i, j);
			}
			if (j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.5 || j == 2.0 || j == 0.0 || j <= -1.0 && j >= -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 0.5 || j == -0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 5.5 || j == -2.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 2.5) {
			if (j <= 4.5 && j >= 3.5 || j <= -4.0 && j >= -7.0) {
				hair(i, j);
			}
			if (j == 5.0  || j == -3.0 || j <= -7.5 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j == 3.0 || j == -2.5) {
				skin_shadow_thin(i, j);
			}
			if (j <= 2.5 && j >= 1.0 || j <= -1.0 && j >= -2.0) {
				skin(i, j);
			}
			if (j == 0.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 0.5 || j == -0.5) {
				bunny_suit(i, j);
			}
			if (j == 5.5 || j == -3.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 3.0) {
			if (j <= 5.0 && j >= 4.0 || j <= -4.0 && j >= -7.0) {
				hair(i, j);
			}
			if (j ==5.5 || j <= -7.5 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j == 3.5 || j == 1.5 || j == -3.0) {
				skin_shadow_thin(i, j);
			}
			if (j == -4.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= -1.5 && j >= -2.5) {
				skin(i, j);
			}
			if (j <= 1.0 && j >= -1.0) {
				bunny_suit(i, j);
			}
			if (j == 3.0 || j == 2.5) {
				shadow2(i, j);
			}
			if (j == 6.0 || j == -3.5 || j == -9.0) {
				black(i, j);
			}
			if (j == 2.0) {
				white(i, j);
			}
		}
		if (i == 3.5) {
			if (j == 4.5 || j == 4.0 || j == -4.0 || j <= -5.0 && j >= -7.0) {
				hair(i, j);
			}
			if (j == 5.5 || j == 5.0 || j <= -7.5 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j == -2.0) {
				skin(i, j);
			}
			if (j == 2.0 || j == -3.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 1.0 && j >= -1.5) {
				bunny_suit(i, j);
			}
			if (j == 3.5 || j == -3.0) {
				shadow2(i, j);
			}
			if (j == 6.0 || j == -4.5 || j == -9.0) {
				black(i, j);
			}
			if (j ==3.0 || j == 2.5 || j == -2.5) {
				white(i, j);
			}
		}
		if (i == 4.0) {
			if (j == 4.5 || j == 4.0 || j == -6.0 || j == -7.0) {
				hair(i, j);
			}
			if (j == 5.5 || j == 5.0 || j <= -7.5 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j == 3.5 || j == 3.0) {
				skin(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 1.0 && j >= -1.5) {
				bunny_suit(i, j);
			}
			if (j == 1.5) {
				reg_shadow_thick(i, j);
			}
			if (j == -3.5) {
				shadow2(i, j);
			}
			if (j == 6.0 || j == 2.0 || j == -4.0 || j == -5.0 || j == -5.5 || j == -6.5 || j == -9.0) {
				black(i, j);
			}
			if (j <= -2.0 && j >= -3.0) {
				white(i, j);
			}
		}
		if (i == 4.5) {
			if (j == 4.5 || j == 4.0 || j == -6.0 || j == -8.0) {
				hair(i, j);
			}
			if (j == 5.5 || j == 5.0 || j == -8.5) {
				hair_shadow(i, j);
			}
			if (j <= 3.5 && j >= 2.5) {
				skin_shadow_thin(i, j);
			}
			if (j == -3.5 || j == -4.0) {
				skin(i, j);
			}
			if (j <= 1.0 && j >= -2.0) {
				bunny_suit(i, j);
			}
			if (j == 1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 2.5) {
				shadow2(i, j);
			}
			if (j == 6.0 || j == 2.0 || j == -4.5 || j == -5.5 || j <= -6.5 && j >= -7.5 || j == -9.0) {
				black(i, j);
			}
			if (j == -2.5 || j == -3.0) {
				white(i, j);
			}
		}
		if (i == 5.0) {
			if (j == 4.0 || j == -6.5) {
				hair(i, j);
			}
			if (j == 5.5) {
				hair_shadow(i, j);
			}
			if (j == -3.0 || j == -3.5) {
				skin(i, j);
			}
			if (j == -4.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.5 || j == 1.0 || j == -2.0) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 0.5 && j >= -1.5) {
				bunny_suit(i, j);
			}
			if (j == 1.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 6.0 || j == 5.0 || j == 4.5 || j <= 3.5 && j >= 2.0 || j == -4.5 || j == -6.0 || j == -7.0 ||j == -8.0 || j == -8.5) {
				black(i, j);
			}
			if (j == -2.5) {
				white(i, j);
			}
		}
		if (i == 5.5) {
			if (j == 1.5 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				bunny_suit(i, j);
			}
			if (j == 1.0 || j == 0.5 || j == -1.0 || j == -1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 5.5 || j == 4.0 || j == 2.0 || j <= -2.5 && j >= -4.0 || j == -6.5) {
				black(i, j);
			}
		}
		if (i == 6.0) {
			if (j == 1.0 || j == 0.5 || j == -1.5 || j == -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.5 || j == -1.0 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 2.0 || j == -3.0) {
				black(i, j);
			}
		}
		if (i == 6.5) {
			if (j <= 1.5 && j >= 0.0 || j == -1.0 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= -1.5 && j >= -2.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -3.5) {
				black(i, j);
			}
		}
		if (i == 7.0) {
			if (j == -1.5 || j == -3.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= -2.0 && j >= -3.0) {
				skin(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				white(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -1.0 || j == -4.0) {
				black(i, j);
			}
		}
		if (i == 7.5) {
			if (j == -2.0 || j == -3.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -2.5 || j == -3.0) {
				skin(i, j);
			}
			if (j == 1.5 ||j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				white(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -1.5 || j == -4.0) {
				black(i, j);
			}
		}
		if (i == 8.0) {
			if (j == -2.5 || j == -4.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -3.0 || j == -3.5) {
				skin(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				white(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -2.0 || j == -4.5) {
				black(i, j);
			}
		}
		if (i == 8.5) {
			if (j == -3.0 || j == -4.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -3.5 || j == -4.0) {
				skin(i, j);
			}
			if (j == 2.0 || j == 0.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				white(i, j);
			}
			if (j == 2.5 || j == 0.0 || j == -2.5 || j == -5.0) {
				black(i, j);
			}
		}
		if (i == 9.0) {
			if (j == -3.0 || j == -5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= -3.5 && j >= -4.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 0.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				white(i, j);
			}
			if (j == 2.5 || j == 0.0 || j == -2.5 || j == -5.5) {
				black(i, j);
			}
		}
		if (i == 9.5) {
			if (j == -4.0 || j == -4.5) {
				skin(i, j);
			}
			if (j == -3.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.0|| j == -5.0) {
				toe(i, j);
			}
			if (j == 2.5 || j == 0.5 || j == -3.0 || j == -5.5) {
				black(i, j);
			}
			if (j == 1.5) {
				white(i, j);
			}
		}
		if (i == 10.0) {
			if (j == 2.0 || j == 1.5 || j == -4.0 || j == -4.5) {
				toe(i, j);
			}
			if (j == 2.5 || j == 1.0 || j == -3.5 || j == -5.0) {
				black(i, j);
			}
		}
		if (i == 10.5) {
			if (j == 2.0 || j == 1.5 || j == -4.0 || j == -4.5) {
				black(i, j);
			}
		}
		
	}
}

void attack2(GLdouble i) {
	GLdouble j;
	for (j = -12.0; j < 14.5; j += 0.5) {
		//上から下に置いていく
		if (i == -11.0) {
			if (j == 7.5) {
				black(i, j);
			}
		}
		if (i == -10.5) {
			if (j == 7.5) {
				hammer_scarlet(i, j);
			}
			if (j == 8.0 || j == 7.0 || j == 3.0 || j == 2.5 || j <= -2.5 && j >= -3.5) {
				black(i, j);
			}
		}
		if (i == -10.0) {
			if (j == 8.0 || j == 7.0) {
				hammer_scarlet(i, j);
			}
			if (j == 7.5) {
				hammer_dark_red(i, j);
			}
			if (j == 8.5 || j == 6.5 || j == 3.5 || j == 2.0 || j == -2.0 || j == -4.0) {
				black(i, j);
			}
			if (j == 3.0 || j == 2.5 || j <= -2.5 && j >= -3.5) {
				ear_outside(i, j);
			}
		}
		if (i == -9.5) {
			if (j == 6.5) {
				hammer_scarlet(i, j);
			}
			if (j <= 8.0 && j >= 7.0) {
				hammer_dark_red(i, j);
			}
			if (j == 8.5 || j == 6.0 || j == 4.0 || j == 1.5 || j == -1.5 || j == -4.5 || j == -5.0) {
				black(i, j);
			}
			if (j == 3.0 || j == 2.5 || j <= -2.5 && j >= -3.5) {
				white(i, j);
			}
			if (j == 3.5 || j == 2.0 || j == -2.0 || j == -4.0) {
				ear_outside(i, j);
			}
		}
		if (i == -9.0) {
			if (j == 6.0) {
				hammer_scarlet(i, j);
			}
			if (j <= 8.0 && j >= 6.5) {
				hammer_dark_red(i, j);
			}
			if (j == 8.5 || j == 5.5 || j == 4.5 || j == 1.0 || j == -1.0 || j == -5.5) {
				black(i, j);
			}
			if (j <= 3.5 && j >= 2.0 || j <= -2.0 && j >= -4.0) {
				white(i, j);
			}
			if (j == 4.0 || j == 1.5 || j == -1.5 || j == -4.5 || j == -5.0) {
				ear_outside(i, j);
			}
		}
		if (i == -8.5) {
			if (j == 5.5) {
				hammer_scarlet(i, j);
			}
			if (j <= 7.5 && j >= 6.0) {
				hammer_dark_red(i, j);
			}
			if (j == 8.0) {
				hammer_yellow(i, j);
			}
			if (j == 9.0 || j == 5.0 || j <= 0.5 && j >= -0.5 || j == -6.0) {
				black(i, j);
			}
			if (j == 8.5 || j <= 3.5 && j >= 1.5 || j <= -1.5 && j >= -5.0) {
				white(i, j);
			}
			if (j == 4.5 || j == 4.0 || j == 1.0 || j == -1.0 || j == -5.5) {
				ear_outside(i, j);
			}
		}
		if (i == -8.0) {
			if (j == 5.0) {
				hammer_scarlet(i, j);
			}
			if (j == 8.5 || j == 8.0 || j <= 7.0 && j >= 5.5) {
				hammer_dark_red(i, j);
			}
			if (j == 7.5) {
				hammer_yellow(i, j);
			}
			if (j == 9.0 || j <= 4.0 && j >= 3.0 || j <= -1.5 && j >= -5.5) {
				white(i, j);
			}
			if (j == 9.5 || j == 4.5 || j == -6.5) {
				black(i, j);
			}
			if (j <= 5.5 && j >= 4.5 || j <= 2.5 && j >= 1.5 || j == -0.5 || j == -1.0 || j == -6.0) {
				ear_outside(i, j);
			}
			if (j <= 1.0 && j >= 0.0) {
				hair_shadow(i, j);
			}
		}
		if (i == -7.5) {
			if (j == 8.0 || j == 5.5 || j == 5.0) {
				hammer_scarlet(i, j);
			}
			if (j == 7.5 || j == 6.5 || j == 6.0) {
				hammer_dark_red(i, j);
			}
			if (j == 9.0 || j == 8.5 || j == 7.0) {
				hammer_yellow(i, j);
			}
			if (j == 10.0 || j == 4.5 || j <= -7.0 && j >= -8.0) {
				black(i, j);
			}
			if (j == 9.5 || j <= 5.5 && j >= 3.0 || j <= -2.0 && j >= -6.0) {
				white(i, j);
			}
			if (j == 2.5 || j == 2.0 || j == -1.5 || j == -6.5) {
				ear_outside(i, j);
			}
			if (j <= 1.0 && j >= -1.0) {
				hair(i, j);
			}
			if (j == 1.5) {
				hair_shadow(i, j);
			}
		}
		if (i == -7.0) {
			if (j == 7.5) {
				hammer_scarlet(i, j);
			}
			if (j == 8.0 || j == 7.0) {
				hammer_dark_red(i, j);
			}
			if (j <= 9.5 && j >= 8.5) {
				hammer_yellow(i, j);
			}
			if (j == 10.5 || j == 6.5 || j == 5.5 || j == 5.0 || j == -8.5 || j == -9.0) {
				black(i, j);
			}
			if (j == 10.0 || j == 6.0 || j <= 4.5 && j >= 3.5 || j <= -2.5 && j >= -6.5) {
				white(i, j);
			}
			if (j == -2.0 || j <= -7.0 && j >= -8.0) {
				ear_outside(i, j);
			}
			if (j <= 2.5 && j >= -1.5) {
				hair(i, j);
			}
			if (j == 3.0) {
				hair_shadow(i, j);
			}
		}
		if (i == -6.5) {
			if (j == 8.0) {
				hammer_scarlet(i, j);
			}
			if (j == 8.5 || j == 7.5) {
				hammer_dark_red(i, j);
			}
			if (j <= 10.0 && j >= 9.0) {
				hammer_yellow(i, j);
			}
			if (j == 6.5) {
				hammer_gray(i, j);
			}
			if (j == 11.0 || j == 7.0 || j == 6.0 || j == -9.5) {
				black(i, j);
			}
			if (j == 10.5 || j <= 5.5 && j >= 4.5 || j <= -3.5 && j >= -7.5) {
				white(i, j);
			}
			if (j == 4.0 || j == -2.5 || j == -3.0 || j <= -8.0 && j >= -9.0) {
				ear_outside(i, j);
			}
			if (j <= 3.0 && j >= -2.0) {
				hair(i, j);
			}
			if (j == 3.5) {
				hair_shadow(i, j);
			}
		}
		if (i == -6.0) {
			if (j == 8.5) {
				hammer_scarlet(i, j);
			}
			if (j == 9.0 || j == 8.0) {
				hammer_dark_red(i, j);
			}
			if (j <= 10.5 && j >= 9.5) {
				hammer_yellow(i, j);
			}
			if (j == 7.0) {
				hammer_gray(i, j);
			}
			if (j <= 12.5 && j >= 11.5 || j == 7.5 || j == 6.5 || j == -10.0) {
				black(i, j);
			}
			if (j == 11.0 || j <= 6.0 && j >= 4.5 || j <= -4.0 && j >= -9.0) {
				white(i, j);
			}
			if (j == 4.0 || j == -3.5 || j == -9.5) {
				ear_outside(i, j);
			}
			if (j <= 3.0 && j >= -2.0) {
				hair(i, j);
			}
			if (j == 3.5 || j == -2.5 || j == -3.0) {
				hair_shadow(i, j);
			}
		}
		if (i == -5.5) {
			if (j <= 12.5 && j >= 11.5 || j <= 10.5 && j >= 8.5) {
				hammer_dark_red(i, j);
			}
			if (j == 11.0) {
				hammer_yellow(i, j);
			}
			if (j == 7.5) {
				hammer_gray(i, j);
			}
			if (j == 13.0 || j == 8.0 || j == 7.0 || j == -10.0) {
				black(i, j);
			}
			if (j <= 6.5 && j >= 5.0 || j <= -5.0 && j >= -9.0) {
				white(i, j);
			}
			if (j == 4.5 || j == -4.0 || j == -4.5 || j == -9.5) {
				ear_outside(i, j);
			}
			if (j <= 3.5 && j >= -3.0) {
				hair(i, j);
			}
			if (j == 4.0 || j == -3.5) {
				hair_shadow(i, j);
			}
		}
		if (i == -5.0) {
			if (j == 13.0) {
				hammer_scarlet(i, j);
			}
			if (j <= 12.5 && j >= 11.0 || j <= 10.0 && j >= 9.0) {
				hammer_dark_red(i, j);
			}
			if (j == 10.5){
				hammer_yellow(i, j);
			}
			if (j == 8.0) {
				hammer_gray(i, j);
			}
			if (j == 13.5 || j == 8.5 || j == 7.0  || j == -9.5) {
				black(i, j);
			}
			if (j == 7.5 || j <= 6.5 && j >= 5.0 || j <= -6.0 && j >= -8.0) {
				white(i, j);
			}
			if (j == 4.5 || j == -5.0 || j == -5.5 || j == -8.5 || j == -9.0) {
				ear_outside(i, j);
			}
			if (j <= 4.0 && j >= -4.0) {
				hair(i, j);
			}
			if (j == -4.5) {
				hair_shadow(i, j);
			}
		}
		if (i == -4.5) {
			if (j == 12.5) {
				hammer_scarlet(i, j);
			}
			if (j <= 12.0 && j >= 10.5 || j == 9.5 || j == 7.0) {
				hammer_dark_red(i, j);
			}
			if (j == 10.0) {
				hammer_yellow(i, j);
			}
			if (j == 8.5) {
				hammer_gray(i, j);
			}
			if (j == 13.0 || j == 9.0 || j == 8.0 || j == 7.5 || j == 6.5 || j == -5.0 || j == -9.0) {
				black(i, j);
			}
			if (j == 6.0 || j == 5.5) {
				white(i, j);
			}
			if (j == 5.0 || j == 4.5 || j <= -5.5 && j >= -8.5) {
				ear_outside(i, j);
			}
			if (j <= 3.0 && j >= 0.0 || j <= -1.0 && j >= -4.0) {
				hair(i, j);
			}
			if (j == 4.0 || j == 3.5 || j == -4.5) {
				hair_shadow(i, j);
			}
			if (j == -0.5) {
				skin_shadow_thin(i, j);
			}
		}
		if (i == -4.0) {
			if (j == 12.0 || j == 6.5) {
				hammer_scarlet(i, j);
			}
			if (j <= 11.5 && j >= 10.0) {
				hammer_dark_red(i, j);
			}
			if (j == 9.0) {
				hammer_gray(i, j);
			}
			if (j == 12.5 || j == 9.5 || j == 8.5 || j == 7.0 || j == 6.0 || j <= -5.0 && j >= -8.5) {
				black(i, j);
			}
			if (j <= 5.5 && j >= 4.5) {
				ear_outside(i, j);
			}
			if (j == 3.5 || j == 2.5 || j == 2.0 || j <= 1.0 && j >= 0.0 || j <= -1.5 && j >= -4.5) {
				hair(i, j);
			}
			if (j == 4.0 || j == 3.0) {
				hair_shadow(i, j);
			}
			if (j == -0.5) {
				skin(i, j);
			}
			if (j == 1.5 || j == -1.0) {
				skin_shadow_thin(i, j);
			}
		}
		if (i == -3.5) {
			if (j == 11.5) {
				hammer_scarlet(i, j);
			}
			if (j <= 11.0 && j >= 10.0) {
				hammer_dark_red(i, j);
			}
			if (j == 6.0) {
				hammer_yellow(i, j);
			}
			if (j == 12.0 || j == 9.0 || j == 6.5 || j <= 5.5 && j >= 4.5 || j == -5.5) {
				black(i, j);
			}
			if (j == 9.5) {
				white(i, j);
			}
			if (j == 4.0 || j == 3.5 || j == 1.0 || j == 0.0 || j <= -1.5 && j >= -4.5) {
				hair(i, j);
			}
			if (j == 3.0 || j == -5.0) {
				hair_shadow(i, j);
			}
			if (j == 0.5 || j == -0.5 || j == -1.0) {
				skin(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0 || j == 1.5) {
				skin_shadow_thin(i, j);
			}
		}
		if (i == -3.0) {
			if (j == 11.0 || j == 10.0) {
				hammer_scarlet(i, j);
			}
			if (j == 10.5) {
				hammer_dark_red(i, j);
			}
			if (j == 5.5) {
				hammer_yellow(i, j);
			}
			if (j ==11.5 || j == 9.5 || j == 6.0 || j == 5.0 || j == 4.0 || j == -6.0) {
				black(i, j);
			}
			if (j == 3.5 || j == 3.0 || j <= -2.0 && j >= -4.5) {
				hair(i, j);
			}
			if (j == -5.0 || j == -5.5) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= 1.5 && j >= -0.5) {
				skin(i, j);
			}
			if (j == -1.0 || j == -1.5) {
				eye_thick(i, j);
			}
		}
		if (i == -2.5) {
			if (j == 10.5 || j == 10.0) {
				hammer_scarlet(i, j);
			}
			if (j == 5.0) {
				hammer_yellow(i, j);
			}
			if (j == 11.0 || j == 9.5 || j == 5.5 || j == 4.5 || j == -6.5) {
				black(i, j);
			}
			if (j <= 4.0 && j >= 3.0 || j <= -1.5 && j >= -5.5) {
				hair(i, j);
			}
			if (j == -4.5 || j == -6.0) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= 1.5 && j >= 0.0 || j == -1.0) {
				skin(i, j);
			}
			if (j == -0.5) {
				eye_thick(i, j);
			}
		}
		if (i == -2.0) {
			if (j == 4.5) {
				hammer_yellow(i, j);
			}
			if (j == 10.5 || j == 10.0 || j == 5.0 || j == 4.0 || j == -7.0) {
				black(i, j);
			}
			if (j <= 4.0 && j >= 3.0 || j <= -1.5 && j >= -5.5) {
				hair(i, j);
			}
			if (j == -6.0 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= 1.5 && j >= 0.5 || j == -0.5 || j == -1.0) {
				skin(i, j);
			}
			if (j == 0.0) {
				eye_thick(i, j);
			}
		}
		if (i == -1.5) {
			if (j == 4.0) {
				hammer_scarlet(i, j);
			}
			if (j == 4.5 || j == 3.5 || j == -7.5) {
				black(i, j);
			}
			if (j <= 4.0 && j >= 3.0 || j <= -1.5 && j >= -6.5) {
				hair(i, j);
			}
			if (j == -7.0) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= 0.0) {
				skin(i, j);
			}
			if (j == -0.5 || j == -1.0) {
				eye_thick(i, j);
			}
		}
		if (i == -1.0) {
			if (j <= -1.5 && j >= -3.5 || j <= -5.0 && j >= -7.0) {
				hair(i, j);
			}
			if (j == -4.0 || j == -4.5 || j == -7.5) {
				hair_shadow(i, j);
			}
			if (j == 3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 4.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 3.5 || j <= 2.5 && j >= -1.0) {
				skin(i, j);
			}
			if (j == 5.0 || j == 4.0 || j == -8.0) {
				black(i, j);
			}
		}
		if (i == -0.5) {
			if (j <= -1.0 && j >= -2.0 || j == -3.0 || j == -3.5 || j <= -4.5 && j >= -7.5) {
				hair(i, j);
			}
			if (j == -2.5 || j == -4.0 || j == -8.0) {
				hair_shadow(i, j);
			}
			if (j == 3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 4.0 || j == 3.5 || j <= 2.5 && j >= -0.5) {
				skin(i, j);
			}
			if (j == 4.5 || j == -8.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 0.0) {
			if (j == -1.0 || j == -1.5 || j == -2.5 || j == -3.0 || j <= -5.0 && j >= -8.5) {
				hair(i, j);
			}
			if (j == -2.0 || j <= -3.5 && j >= -4.5 || j == -9.0) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 3.5) {
				skin_shadow_thin(i, j);
			}
			if (j <= 2.0 && j >= 1.0 || j == 0.0 || j == -0.5) {
				skin(i, j);
			}
			if (j == 0.5) {
				mouth(i, j);
			}
			if (j == 4.0 || j == -7.0 || j == -9.5) {
				black(i, j);
			}
			if (j == 3.0) {
				white(i, j);
			}
		}
		if (i == 0.5) {
			if (j <= -2.0 && j >= -3.0 || j <= -4.5 && j >= -6.5 || j <= -8.0 && j >= -9.0) {
				hair(i, j);
			}
			if (j <= -0.5 && j >= -1.5 || j == -3.5 || j == -4.0 || j == -7.0) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == 1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0) {
				skin(i, j);
			}
			if (j == 0.5 || j == 0.0) {
				mouth(i, j);
			}
			if (j == 3.0) {
				shadow2(i, j);
			}
			if (j == 3.5|| j == -7.5 || j == -9.5) {
				black(i, j);
			}
			if (j == 2.5) {
				white(i, j);
			}
		}
		if (i == 1.0) {
			if (j == 3.0 || j <= -1.0 && j >= -2.5 || j <= -4.0 && j >= -7.0) {
				hair(i, j);
			}
			if (j == -3.0 || j == -3.5 || j == -7.5) {
				hair_shadow(i, j);
			}
			if (j <= 1.0 && j >= -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0 || j == 1.5 || j <= 0.5 && j >= -1.0) {
				skin(i, j);
			}
			if (j == -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 2.5) {
				shadow2(i, j);
			}
			if (j == 3.5 || j <= -8.0 && j >= -9.0) {
				black(i, j);
			}
		}
		if (i == 1.5) {
			if (j == 2.5 || j <= -4.0 && j >= -8.0) {
				hair(i, j);
			}
			if (j == -2.0 || j == -3.0 || j == -3.5) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j <= 0.5 && j >= -0.5 || j == -1.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				skin(i, j);
			}
			if (j == -1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 3.0 || j == -2.5 || j == -8.5) {
				black(i, j);
			}
		}
		if (i == 2.0) {
			if (j == 2.5 || j <= -4.0 && j >= -7.0 || j == -8.5) {
				hair(i, j);
			}
			if (j == -3.0 || j == -3.5 || j == -7.5 || j == -8.0) {
				hair_shadow(i, j);
			}
			if (j == 1.0) {
				skin(i, j);
			}
			if (j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0 || j == 1.5 || j == 0.0 || j <= -1.0 && j >= -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 0.5 || j == -0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 3.0 || j == -2.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 2.5) {
			if (j == 2.0 || j <= -4.0 && j >= -7.0) {
				hair(i, j);
			}
			if (j == -3.0 || j <= -7.5 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j == 1.0 || j == -2.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.5 || j <= -1.0 && j >= -2.0) {
				skin(i, j);
			}
			if (j == 0.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 0.5 || j == -0.5) {
				bunny_suit(i, j);
			}
			if (j == 2.5 || j == -3.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 3.0) {
			if (j == 2.0 || j <= -4.0 && j >= -7.0) {
				hair(i, j);
			}
			if (j <= -7.5 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j == 1.5 || j == -3.0) {
				skin_shadow_thin(i, j);
			}
			if (j == -4.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= -1.5 && j >= -2.5) {
				skin(i, j);
			}
			if (j <= 1.0 && j >= -1.0) {
				bunny_suit(i, j);
			}
			if (j == 2.5 || j == -3.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 3.5) {
			if (j == -4.0 || j <= -5.0 && j >= -7.0) {
				hair(i, j);
			}
			if (j <= -7.5 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j == -2.0) {
				skin(i, j);
			}
			if (j == -3.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 1.0 && j >= -1.5) {
				bunny_suit(i, j);
			}
			if (j == -3.0) {
				shadow2(i, j);
			}
			if (j == 2.0 || j == -4.5 || j == -9.0) {
				black(i, j);
			}
			if (j == -2.5) {
				white(i, j);
			}
		}
		if (i == 4.0) {
			if (j == -6.0 || j == -7.0) {
				hair(i, j);
			}
			if (j <= -7.5 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j == 1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 1.0 && j >= -1.5) {
				bunny_suit(i, j);
			}
			if (j == 1.5) {
				reg_shadow_thick(i, j);
			}
			if (j == -3.5) {
				shadow2(i, j);
			}
			if (j == 2.0 || j == -4.0 || j == -5.0 || j == -5.5 || j == -6.5 || j == -9.0) {
				black(i, j);
			}
			if (j <= -2.0 && j >= -3.0) {
				white(i, j);
			}
		}
		if (i == 4.5) {
			if (j == -6.0 || j == -8.0) {
				hair(i, j);
			}
			if (j == -8.5) {
				hair_shadow(i, j);
			}
			if (j == -3.5 || j == -4.0) {
				skin(i, j);
			}
			if (j <= 1.0 && j >= -2.0) {
				bunny_suit(i, j);
			}
			if (j == 1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 2.0 || j == -4.5 || j == -5.5 || j <= -6.5 && j >= -7.5 || j == -9.0) {
				black(i, j);
			}
			if (j == -2.5 || j == -3.0) {
				white(i, j);
			}
		}
		if (i == 5.0) {
			if (j == -6.5) {
				hair(i, j);
			}
			if (j == -3.0 || j == -3.5) {
				skin(i, j);
			}
			if (j == -4.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.5 || j == 1.0 || j == -2.0) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 0.5 && j >= -1.5) {
				bunny_suit(i, j);
			}
			if (j == 1.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 2.0 || j == -4.5 || j == -6.0 || j == -7.0 || j == -8.0 || j == -8.5) {
				black(i, j);
			}
			if (j == -2.5) {
				white(i, j);
			}
		}
		if (i == 5.5) {
			if (j == 1.5 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				bunny_suit(i, j);
			}
			if (j == 1.0 || j == 0.5 || j == -1.0 || j == -1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 2.0 || j <= -2.5 && j >= -4.0 || j == -6.5) {
				black(i, j);
			}
		}
		if (i == 6.0) {
			if (j == 1.0 || j == 0.5 || j == -1.5 || j == -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.5 || j == -1.0 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 2.0 || j == -3.0) {
				black(i, j);
			}
		}
		if (i == 6.5) {
			if (j <= 1.5 && j >= 0.0 || j == -1.0 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= -1.5 && j >= -2.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -3.5) {
				black(i, j);
			}
		}
		if (i == 7.0) {
			if (j == -1.5 || j == -3.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= -2.0 && j >= -3.0) {
				skin(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				white(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -1.0 || j == -4.0) {
				black(i, j);
			}
		}
		if (i == 7.5) {
			if (j == -2.0 || j == -3.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -2.5 || j == -3.0) {
				skin(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				white(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -1.5 || j == -4.0) {
				black(i, j);
			}
		}
		if (i == 8.0) {
			if (j == -2.5 || j == -4.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -3.0 || j == -3.5) {
				skin(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				white(i, j);
			}
			if (j == 2.0 || j == -0.5 || j == -2.0 || j == -4.5) {
				black(i, j);
			}
		}
		if (i == 8.5) {
			if (j == -3.0 || j == -4.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -3.5 || j == -4.0) {
				skin(i, j);
			}
			if (j == 2.0 || j == 0.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				white(i, j);
			}
			if (j == 2.5 || j == 0.0 || j == -2.5 || j == -5.0) {
				black(i, j);
			}
		}
		if (i == 9.0) {
			if (j == -3.0 || j == -5.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= -3.5 && j >= -4.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 0.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.5 || j == 1.0) {
				white(i, j);
			}
			if (j == 2.5 || j == 0.0 || j == -2.5 || j == -5.5) {
				black(i, j);
			}
		}
		if (i == 9.5) {
			if (j == -4.0 || j == -4.5) {
				skin(i, j);
			}
			if (j == -3.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.0 || j == -5.0) {
				toe(i, j);
			}
			if (j == 2.5 || j == 0.5 || j == -3.0 || j == -5.5) {
				black(i, j);
			}
			if (j == 1.5) {
				white(i, j);
			}
		}
		if (i == 10.0) {
			if (j == 2.0 || j == 1.5 || j == -4.0 || j == -4.5) {
				toe(i, j);
			}
			if (j == 2.5 || j == 1.0 || j == -3.5 || j == -5.0) {
				black(i, j);
			}
		}
		if (i == 10.5) {
			if (j == 2.0 || j == 1.5 || j == -4.0 || j == -4.5) {
				black(i, j);
			}
		}
	}
}

void attack3(GLdouble i) {
	GLdouble j;
	for (j = -12.0; j < 12.0; j += 0.5) {
		//上から下に置いていく
		if (i == -10.5) {
			if (j == 3.0 || j == 2.5 || j <= -2.5 && j >= -3.5) {
				black(i, j);
			}
		}
		if (i == -10.0) {
			if (j == 3.0 || j == 2.5 || j <= -2.5 && j >= -3.5) {
				ear_outside(i, j);
			}
			if (j == 3.5 || j == 2.0 || j == -2.0 || j == -4.0) {
				black(i, j);
			}
		}
		if (i == -9.5) {
			if (j == 3.0 || j == 2.5 || j <= -2.5 && j >= -3.5) {
				white(i, j);
			}
			if (j == 3.5 || j == 2.0 || j == -2.0 || j == -4.0) {
				ear_outside(i, j);
			}
			if (j == 4.0 || j == 1.5 || j == -1.5 || j == -4.5 || j == -5.0) {
				black(i, j);
			}
		}
		if (i == -9.0) {
			if (j <= 3.5 && j >= 2.0 || j <= -2.0 && j >= -4.0) {
				white(i, j);
			}
			if (j == 4.0 || j == 1.5 || j == -1.5 || j == -4.5 || j == -5.0) {
				ear_outside(i, j);
			}
			if (j == 4.5 || j == 1.0 || j == -1.0 || j == -5.5) {
				black(i, j);
			}
		}
		if (i == -8.5) {
			if (j <= 3.5 && j >= 1.5 || j <= -1.5 && j >= -5.0) {
				white(i, j);
			}
			if (j == 4.5 || j == 4.0 || j == 1.0 || j == -1.0 || j == -5.5) {
				ear_outside(i, j);
			}
			if (j == 5.5 || j == 5.0 || j <= 0.5 && j >= -0.5 || j == -6.0) {
				black(i, j);
			}
		}
		if (i == -8.0) {
			if (j <= 4.0 && j >= 3.0 || j <= -1.5 && j >= -5.5) {
				white(i, j);
			}
			if (j <= 5.5 && j >= 4.5 || j <= 2.5 && j >= 1.5 || j == -0.5 || j == -1.0 || j == -6.0) {
				ear_outside(i, j);
			}
			if (j <= 1.0 && j >= 0.0) {
				hair_shadow(i, j);
			}
			if (j == 6.0 || j == -6.5) {
				black(i, j);
			}
		}
		if (i == -7.5) {
			if (j <= 5.5 && j >= 3.0 || j <= -2.0 && j >= -6.0) {
				white(i, j);
			}
			if (j == 6.0 || j == 2.5 || j == 2.0 || j == -1.5 || j == -6.5) {
				ear_outside(i, j);
			}
			if (j <= 1.0 && j >= -1.0) {
				hair(i, j);
			}
			if (j == 1.5) {
				hair_shadow(i, j);
			}
			if (j == 6.5 || j <= -7.0 && j >= -8.0) {
				black(i, j);
			}
		}
		if (i == -7.0) {
			if (j <= 6.0 && j >= 3.5 || j <= -2.5 && j >= -6.5) {
				white(i, j);
			}
			if (j == 6.5 || j == -2.0 || j <= -7.0 && j >= -8.0) {
				ear_outside(i, j);
			}
			if (j <= 2.5 && j >= -1.5) {
				hair(i, j);
			}
			if (j == 3.0) {
				hair_shadow(i, j);
			}
			if (j == 7.5 || j == 7.0 || j == -8.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == -6.5) {
			if (j <= 6.5 && j >= 4.5 || j <= -3.5 && j >= -7.5) {
				white(i, j);
			}
			if (j == 7.5 || j == 7.0 || j == 4.0 || j == -2.5 || j == -3.0 || j <= -8.0 && j >= -9.0) {
				ear_outside(i, j);
			}
			if (j <= 3.0 && j >= -2.0) {
				hair(i, j);
			}
			if (j == 3.5) {
				hair_shadow(i, j);
			}
			if (j == 8.0 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == -6.0) {
			if (j <= 7.0 && j >= 4.5 || j <= -4.0 && j >= -9.0) {
				white(i, j);
			}
			if (j == 8.0 || j == 7.5 || j == 4.0 || j == -3.5 || j == -9.5) {
				ear_outside(i, j);
			}
			if (j <= 3.0 && j >= -2.0) {
				hair(i, j);
			}
			if (j == 3.5 || j == -2.5 || j == -3.0) {
				hair_shadow(i, j);
			}
			if (j == 8.5 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == -5.5) {
			if (j <= 8.0 && j >= 5.0 || j <= -5.0 && j >= -9.0) {
				white(i, j);
			}
			if (j == 8.5 || j == 4.5 || j == -4.0 || j == -4.5 || j == -9.5) {
				ear_outside(i, j);
			}
			if (j <= 3.5 && j >= -3.0) {
				hair(i, j);
			}
			if (j == 4.0 || j == -3.5) {
				hair_shadow(i, j);
			}
			if (j == 9.0 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == -5.0) {
			if (j <= 8.5 && j >= 5.0 || j <= -6.0 && j >= -8.0) {
				white(i, j);
			}
			if (j == 9.0 || j == 4.5 || j == -5.0 || j == -5.5 || j == -8.5 || j == -9.0) {
				ear_outside(i, j);
			}
			if (j <= 4.0 && j >= -4.0) {
				hair(i, j);
			}
			if (j == -4.5) {
				hair_shadow(i, j);
			}
			if (j == 10.0 || j == 9.5 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == -4.5) {
			if (j <= 9.0 && j >= 5.5) {
				white(i, j);
			}
			if (j == 10.0 || j == 9.5 || j == 5.0 || j == 4.5 || j <= -5.5 && j >= -8.5) {
				ear_outside(i, j);
			}
			if (j <= 3.0 && j >= 0.0 || j <= -1.0 && j >= -4.0) {
				hair(i, j);
			}
			if (j == 4.0 || j == 3.5 || j == -4.5) {
				hair_shadow(i, j);
			}
			if (j == -0.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 10.5 || j == -5.0 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == -4.0) {
			if (j <= 9.0 && j >= 6.0) {
				white(i, j);
			}
			if (j == 9.5 || j <= 5.5 && j >= 4.5) {
				ear_outside(i, j);
			}
			if (j == 3.5 || j == 2.5 || j == 2.0 || j <= 1.0 && j >= 0.0 || j <= -1.5 && j >= -4.5) {
				hair(i, j);
			}
			if (j == 4.0 || j == 3.0) {
				hair_shadow(i, j);
			}
			if (j == -0.5) {
				skin(i, j);
			}
			if (j == 1.5 || j == -1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 10.0 || j <= -5.0 && j >= -8.5) {
				black(i, j);
			}
		}
		if (i == -3.5) {
			if (j <= 9.0 && j >= 6.0) {
				ear_outside(i, j);
			}
			if (j == 4.0 || j == 3.5 || j == 1.0 || j == 0.0 || j <= -1.5 && j >= -4.5) {
				hair(i, j);
			}
			if (j == 3.0 || j == -5.0) {
				hair_shadow(i, j);
			}
			if (j == 0.5 || j == -0.5 || j == -1.0) {
				skin(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0 || j == 1.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 9.5 || j <= 5.5 && j >= 4.5 || j == -5.5) {
				black(i, j);
			}
		}
		if (i == -3.0) {
			if (j <= 4.0 && j >= 3.0 || j <= -2.0 && j >= -4.5) {
				hair(i, j);
			}
			if (j == -5.0 || j == -5.5) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= 1.5 && j >= 0.5) {
				skin(i, j);
			}
			if (j <= 0.0 && j >= -1.5) {
				eye_thick(i, j);
			}
			if (j <= 9.0 && j >= 6.0 || j == 4.5 || j == -6.0) {
				black(i, j);
			}
		}
		if (i == -2.5) {
			if (j <= 4.0 && j >= 3.0 || j <= -1.5 && j >= -5.5) {
				hair(i, j);
			}
			if (j == -4.5 || j == -6.0) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= 1.5 && j >= 0.5) {
				skin(i, j);
			}
			if (j == 0.0 || j == -0.5) {
				eye_thick(i, j);
			}
			if (j == 4.5 || j == -6.5) {
				black(i, j);
			}
			if (j == -1.0) {
				white(i, j);
			}
		}
		if (i == -2.0) {
			if (j <= 4.0 && j >= 3.0 || j <= -1.5 && j >= -5.5) {
				hair(i, j);
			}
			if (j == -6.0 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= 1.5 && j >= 0.5) {
				skin(i, j);
			}
			if (j == 0.0 || j == -1.0) {
				eye(i, j);
			}
			if (j == -0.5) {
				eye_thick(i, j);
			}
			if (j == 4.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == -1.5) {
			if (j == 3.5 || j == 3.0 || j <= -1.5 && j >= -6.5) {
				hair(i, j);
			}
			if (j == -7.0) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= 0.5) {
				skin(i, j);
			}
			if (j <= 0.0 && j >= -1.0) {
				eye(i, j);
			}
			if (j == 4.0 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == -1.0) {
			if (j == 3.5 || j <= -1.5 && j >= -3.5 || j <= -5.0 && j >= -7.0) {
				hair(i, j);
			}
			if (j == -4.0 || j == -4.5 || j == -7.5) {
				hair_shadow(i, j);
			}
			if (j == 3.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.5 && j >= 0.5) {
				skin(i, j);
			}
			if (j <= 0.0 && j >= -1.0) {
				eye_thin(i, j);
			}
			if (j == 4.0 || j == -8.0) {
				black(i, j);
			}
		}
		if (i == -0.5) {
			if (j == 3.5 || j == -1.5 || j == -2.0 || j == -3.0 || j == -3.5 || j <= -4.5 && j >= -7.5) {
				hair(i, j);
			}
			if (j == -2.5 || j == -4.0 || j == -8.0) {
				hair_shadow(i, j);
			}
			if (j == 3.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.5 && j >= -1.0) {
				skin(i, j);
			}
			if (j == 4.0 || j == -8.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 0.0) {
			if (j == 3.5 || j == 3.0 || j == -1.5 || j == -2.5 || j == -3.0 || j <= -5.0 && j >= -8.5) {
				hair(i, j);
			}
			if (j == -2.0 || j <= -3.5 && j >= -4.5 || j == -9.0) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= 1.0 || j <= 0.0 && j >= -1.0) {
				skin(i, j);
			}
			if (j == 0.5) {
				mouth(i, j);
			}
			if (j <= 5.0 && j >= 4.0 || j == -7.0 || j == -9.5) {
				black(i, j);
			}
		}
		if (i == 0.5) {
			if (j <= -2.0 && j >= -3.0 || j <= -4.5 && j >= -6.5 || j <= -8.0 && j >= -9.0) {
				hair(i, j);
			}
			if (j == -1.0 || j == -1.5 || j == -3.5 || j == -4.0 || j == -7.0) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == 1.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 5.0 && j >= 4.0 || j == 1.0 || j == -0.5) {
				skin(i, j);
			}
			if (j == 0.5 || j == 0.0) {
				mouth(i, j);
			}
			if (j == 5.5 || j == -7.5 || j == -9.5) {
				black(i, j);
			}
			if (j <= 3.5 && j >= 2.5) {
				white(i, j);
			}
		}
		if (i == 1.0) {
			if (j <= -1.0 && j >= -2.5 || j <= -4.0 && j >= -7.0) {
				hair(i, j);
			}
			if (j == -3.0 || j == -3.5 || j == -7.5) {
				hair_shadow(i, j);
			}
			if (j == 5.0 || j == 4.5) {
				skin_shadow_thin(i, j);
			}
			if (j <= 1.0 && j >= -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.5 && j >= 1.5 || j <= 0.5 && j >= -1.0) {
				skin(i, j);
			}
			if (j == -2.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 5.5 || j <= -8.0 && j >= -9.0) {
				black(i, j);
			}
			if (j <= 4.0 && j >= 3.0) {
				white(i, j);
			}
		}
		if (i == 1.5) {
			if (j <= -4.0 && j >= -8.0) {
				hair(i, j);
			}
			if (j == -2.0 || j == -3.0 || j == -3.5) {
				hair_shadow(i, j);
			}
			if (j == 1.0) {
				skin(i, j);
			}
			if (j <= 2.5 && j >= 1.5 || j <= 0.5 && j >= -1.5) {
				skin_shadow_thin(i, j);
			}
			if (j <= 4.0 && j >= 3.0) {
				shadow2(i, j);
			}
			if (j == 5.0 || j == 4.5 || j == -2.5 || j == -8.5) {
				black(i, j);
			}
		}
		if (i == 2.0) {
			if (j <= 4.5 && j >= 2.0 || j <= -4.0 && j >= -7.0 || j == -8.5) {
				hair(i, j);
			}
			if (j == -3.0 || j == -3.5 || j == -7.5 || j == -8.0) {
				hair_shadow(i, j);
			}
			if (j <= 0.0 && j >= -1.5) {
				skin(i, j);
			}
			if (j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.5 || j == 0.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 5.0 || j == -2.0 || j == -2.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 2.5) {
			if (j <= 4.5 && j >= 2.0 || j <= -4.0 && j >= -7.0) {
				hair(i, j);
			}
			if (j <= -7.5 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j == -1.5) {
				skin_shadow_thin(i, j);
			}
			if (j <= 0.5 && j >= -1.0) {
				skin(i, j);
			}
			if (j == 1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 1.0 || j == -0.5) {
				bunny_suit(i, j);
			}
			if (j == 5.0 || j <= -2.0 && j >= -3.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == 3.0) {
			if (j <= 4.5 && j >= 2.0 || j <= -4.0 && j >= -7.0) {
				hair(i, j);
			}
			if (j <= -7.5 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j == -0.5 || j == -1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == -4.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= 0.0) {
				skin(i, j);
			}
			if (j == -1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 5.0 || j == -2.0 || j == -3.5 || j == -9.0) {
				black(i, j);
			}
			if (j == 1.5) {
				white(i, j);
			}
		}
		if (i == 3.5) {
			if (j == 5.0 || j == 4.5 || j == 3.5 || j == 2.5 || j == -4.0 || j <= -5.0 && j >= -7.0) {
				hair(i, j);
			}
			if (j <= -7.5 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j == 1.0) {
				skin(i, j);
			}
			if (j == 0.5) {
				skin_shadow_thin(i, j);
			}
			if (j == -1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 0.0 && j >= -1.0) {
				bunny_suit(i, j);
			}
			if (j == 5.5 || j == 4.0 || j == 3.0 || j == -2.0 || j == -3.5 || j == -4.5 || j == -9.0) {
				black(i, j);
			}
			if (j == 2.0 || j == 1.5) {
				white(i, j);
			}
		}
		if (i == 4.0) {
			if (j == 5.0 || j == 4.5 || j == -6.0 || j == -7.0) {
				hair(i, j);
			}
			if (j <= -7.5 && j >= -8.5) {
				hair_shadow(i, j);
			}
			if (j == 3.0 || j == 2.5) {
				skin(i, j);
			}
			if (j == 1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == -1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 0.5 && j >= -1.0) {
				bunny_suit(i, j);
			}
			if (j == 1.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 1.5) {
				shadow2(i, j);
			}
			if (j == 5.5 || j == 4.0 || j == 3.5 || j == -2.0 || j == -2.5 || j == -4.0 || j == -5.0 || j == -5.5 || j == -6.5 || j == -9.0) {
				black(i, j);
			}
			if (j == 2.0) {
				white(i, j);
			}
		}
		if (i == 4.5) {
			if (j == 5.5 || j == -6.0 || j == -8.0) {
				hair(i, j);
			}
			if (j == -8.5) {
				hair_shadow(i, j);
			}
			if (j == 3.0 || j == 2.5) {
				skin(i, j);
			}
			if (j <= 1.0 && j >= -1.0) {
				bunny_suit(i, j);
			}
			if (j == -1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 2.0 || j == 1.5 || j == -2.5) {
				shadow2(i, j);
			}
			if (j == 6.0 || j == 5.0 || j == 4.5 || j == 3.5 || j == -3.0 || j == -5.5 || j <= -6.5 && j >= -7.5 || j == -9.0) {
				black(i, j);
			}
			if (j == -2.0) {
				white(i, j);
			}
		}
		if (i == 5.0) {
			if (j == -6.5) {
				hair(i, j);
			}
			if (j == 2.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == -1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 1.5 && j >= -1.0) {
				bunny_suit(i, j);
			}
			if (j == 1.5) {
				reg_shadow_thick(i, j);
			}
			if (j == -2.5) {
				shadow2(i, j);
			}
			if (j == 5.5 || j == 3.0 || j == -3.0 || j == -6.0 || j == -7.0 || j == -8.0 || j == -8.5) {
				black(i, j);
			}
			if (j == -2.0) {
				white(i, j);
			}
		}
		if (i == 5.5) {
			if (j == 2.5 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				bunny_suit(i, j);
			}
			if (j == 2.0 || j == 1.5 || j <= 0.0 && j >= -1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == -2.0) {
				shadow2(i, j);
			}
			if (j == 3.0 || j == -2.5 || j == -6.5) {
				black(i, j);
			}
		}
		if (i == 6.0) {
			if (j == 2.0 || j == 1.5 || j == -0.5 || j == -1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 2.5 || j == 0.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.0 || j == 0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 3.0 || j == -2.0) {
				black(i, j);
			}
		}
		if (i == 6.5) {
			if (j == 3.0 || j == 1.5 || j == 0.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.5 || j == 2.0 || j == -0.5 || j == -1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 3.5 || j == 1.0 || j == 0.5 || j == -2.0) {
				black(i, j);
			}
		}
		if (i == 7.0) {
			if (j == 0.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -0.5 || j == -1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 3.5 || j == 2.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 3.0 || j == 2.5) {
				white(i, j);
			}
			if (j == 4.0 || j == 1.5 || j == 0.5 || j == -2.0) {
				black(i, j);
			}
		}
		if (i == 7.5) {
			if (j == 0.0 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= -0.5 && j >= -1.5) {
				skin(i, j);
			}
			if (j == 4.0 || j == 2.0) {
				reg_shadow_thick(i, j);
			}
			if (j <= 3.5 && j >= 2.5) {
				white(i, j);
			}
			if (j == 4.5 || j == 1.5 || j == 0.5 || j == -2.5) {
				black(i, j);
			}
		}
		if (i == 8.0) {
			if (j == -0.5 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.0 || j == -1.5) {
				skin(i, j);
			}
			if (j == 4.5 || j == 2.5) {
				reg_shadow_thick(i, j);
			}
			if (j <= 4.0 && j >= 3.0) {
				white(i, j);
			}
			if (j == 5.0 || j == 2.0 || j == 0.0 || j == -2.5) {
				black(i, j);
			}
		}
		if (i == 8.5) {
			if (j == -0.5 || j == -2.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.0 || j == -1.5) {
				skin(i, j);
			}
			if (j == 5.0 || j == 3.5 || j == 3.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 4.5 || j == 4.0) {
				white(i, j);
			}
			if (j == 5.5 || j == 2.5 || j == 0.0 || j == -2.5) {
				black(i, j);
			}
		}
		if (i == 9.0) {
			if (j == -1.0 || j == -2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -1.5 || j == -2.0) {
				skin(i, j);
			}
			if (j == 5.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 4.5) {
				white(i, j);
			}
			if (j == 4.0) {
				toe(i, j);
			}
			if (j == 5.5 || j == 3.5 || j == 3.0 || j == -0.5 || j == -3.0) {
				black(i, j);
			}
		}
		if (i == 9.5) {
			if (j <= -1.5 && j >= -2.5) {
				skin(i, j);
			}
			if (j == -1.0 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 5.0 || j == 4.5) {
				toe(i, j);
			}
			if (j == 5.5 || j == 4.0 || j == -0.5 || j == -3.5) {
				black(i, j);
			}
		}
		if (i == 10.0) {
			if (j == -2.0 || j == -2.5) {
				skin(i, j);
			}
			if (j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -3.0) {
				toe(i, j);
			}
			if (j == 5.0 || j == 4.5 || j == -1.0 || j == -3.5) {
				black(i, j);
			}
		}
		if (i == 10.5) {
			if (j == -2.0 || j == -2.5) {
				toe(i, j);
			}
			if (j == -1.5 || j == -3.0) {
				black(i, j);
			}
		}
		if (i == 11.0) {
			if (j == -2.0 || j == -2.5) {
				black(i, j);
			}
		}
	}
}

void attack4(GLdouble i) {
	GLdouble j;
	for (j = -15.0; j < 15.0; j += 0.5) {
		//上から下に置いていく
		if (i == -10.5) {
			if (j == 3.5 || j == 3.0 || j == -4.0 || j == -4.5) {
				black(i, j);
			}
		}
		if (i == -10.0) {
			if (j == 3.5 || j == 3.0 || j == -4.0 || j == -4.5) {
				ear_outside(i, j);
			}
			if (j == 4.0 || j == 2.5 || j == -3.5 || j == -5.0 || j == -5.5) {
				black(i, j);
			}
		}
		if (i == -9.5) {
			if (j == 3.5 || j == 3.0 || j == -4.0 || j == -4.5) {
				white(i, j);
			}
			if (j == 4.0 || j == 2.5 || j == -3.5 || j == -5.0 || j == -5.5) {
				ear_outside(i, j);
			}
			if (j == 5.0 || j == 4.5 || j == 2.0 || j == -3.0 || j == -6.0) {
				black(i, j);
			}
		}
		if (i == -9.0) {
			if (j <= 4.0 && j >= 2.5 || j <= -3.5 && j >= -5.5) {
				white(i, j);
			}
			if (j == 5.0 || j == 4.5 || j == 2.0 || j == -3.0 || j == -6.0) {
				ear_outside(i, j);
			}
			if (j == 5.5 || j == 1.5 || j == -2.5 || j == -6.5 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == -8.5) {
			if (j <= 5.0 && j >= 2.0 || j <= -3.0 && j >= -6.0) {
				white(i, j);
			}
			if (j == 5.5 || j == 1.5 || j == -2.5 || j == -6.5 || j == -7.0) {
				ear_outside(i, j);
			}
			if (j == 6.0 || j <= 1.5 && j >= -2.5 || j == -7.5) {
				black(i, j);
			}
		}
		if (i == -8.0) {
			if (j <= 5.5 && j >= 2.0 || j <= -3.0 && j >= -7.0) {
				white(i, j);
			}
			if (j == 6.0 || j == 1.5 || j == -2.5 || j == -7.5) {
				ear_outside(i, j);
			}
			if (j <= 1.5 && j >= -2.5) {
				hair_shadow(i, j);
			}
			if (j == 7.0 || j == 6.5 || j == -8.0) {
				black(i, j);
			}
		}
		if (i == -7.5) {
			if (j <= 6.0 && j >= 3.0 || j <= -4.5 && j >= -7.5) {
				white(i, j);
			}
			if (j == 7.0 || j == 6.5 || j == 2.0 || j == 2.5 || j <= -3.0 && j >= -4.0 || j == -8.0) {
				ear_outside(i, j);
			}
			if (j == 1.5) {
				hair_shadow(i, j);
			}
			if (j <= 1.0 && j >= -2.5) {
				hair(i, j);
			}
			if (j == 7.5 || j == -8.5 || j == -9.0) {
				black(i, j);
			}
		}
		if (i == -7.0) {
			if (j <= 7.0 && j >= 4.0 || j <= -5.5 && j >= -8.0) {
				white(i, j);
			}
			if (j == 7.5 || j == 3.5 || j == 3.0 || j <= -4.0 && j >= -5.0 || j == -8.5 || j == -9.0) {
				ear_outside(i, j);
			}
			if (j <= 2.5 && j >= -3.5) {
				hair(i, j);
			}
			if (j <= 9.0 && j >= 8.0 || j == -9.5 || j == -10.0) {
				black(i, j);
			}
		}
		if (i == -6.5) {
			if (j <= 7.5 && j >= 4.5 || j <= -6.0 && j >= -9.0) {
				white(i, j);
			}
			if (j <= 9.0 && j >= 8.0 || j == 4.0 || j == 3.5 || j == -5.0 || j == -5.5 || j == -9.5 || j == -10.0) {
				ear_outside(i, j);
			}
			if (j <= 3.0 && j >= -4.5) {
				hair(i, j);
			}
			if (j == 9.5 || j == -10.5 || j == -11.0) {
				black(i, j);
			}
		}
		if (i == -6.0) {
			if (j <= 9.0 && j >= 5.0 || j <= -7.5 && j >= -10.0) {
				white(i, j);
			}
			if (j == 9.5 || j == 4.5 || j <= -6.0 && j >= -7.0 || j == -9.0 || j == -10.5 || j == -11.0) {
				ear_outside(i, j);
			}
			if (j <= 4.0 && j >= -5.0) {
				hair(i, j);
			}
			if (j == 10.0 || j == -5.5 || j == -11.5) {
				black(i, j);
			}
		}
		if (i == -5.5) {
			if (j <= 9.5 && j >= 5.5 || j <= -8.0 && j >= -11.0) {
				white(i, j);
			}
			if (j == 10.0 || j == 5.0 || j == -7.5 || j == -8.0 || j == -11.5) {
				ear_outside(i, j);
			}
			if (j <= 4.5 && j >= -5.5) {
				hair(i, j);
			}
			if (j == 10.5 || j <= -6.0 && j >= -7.0 || j == -12.0) {
				black(i, j);
			}
		}
		if (i == -5.0) {
			if (j == 0.0) {
				skin_shadow_thin(i, j);
			}
			if (j <= 10.0 && j >= 6.0 || j <= -8.5 && j >= -11.5) {
				white(i, j);
			}
			if (j == 10.5 || j == 5.5 || j == -8.0 || j == -12.0) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= -5.5) {
				hair(i, j);
			}
			if (j == 3.5) {
				hair_shadow(i, j);
			}
			if (j == 11.0 || j == -6.0 || j == -7.5 || j == -12.5) {
				black(i, j);
			}
		}
		if (i == -4.5) {
			if (j <= 10.5 && j >= 7.0 || j <= -9.0 && j >= -12.0) {
				white(i, j);
			}
			if (j == 11.0 || j == 6.5 || j == 6.0 || j == -8.5 || j == -10.0 || j == -12.5) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= 3.5 || j == 2.5 || j == 1.0 || j == 0.5 || j == -1.0 || j == -1.5 || j <= -2.5 && j >= -5.5) {
				hair(i, j);
			}
			if (j == 3.0 || j == -2.0) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == 1.5 || j == -0.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 0.0) {
				skin(i, j);
			}
			if (j == 11.5 || j == 5.5 || j == -6.0 || j == -8.0 || j == -13.0) {
				black(i, j);
			}
		}
		if (i == -4.0) {
			if (j <= 10.0 && j >= 7.5 || j <= -10.0 && j >= -11.5) {
				white(i, j);
			}
			if (j == 11.0 || j == 10.5 || j == 7.0 || j == -9.0 || j == -9.5 || j == -12.0) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= 3.5 || j == 0.5 || j == -1.5 || j <= -2.5 && j >= -5.5) {
				hair(i, j);
			}
			if (j == 3.0 || j == -2.0) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == -1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 1.5 || j == 1.0 || j == 0.0 || j == -0.5) {
				skin(i, j);
			}
			if (j == 11.5 || j <= 6.5 && j >= 5.5 || j == -6.0 || j == -8.5 || j == -12.5) {
				black(i, j);
			}
		}
		if (i == -3.5) {
			if (j <= 9.0 && j >= 8.0) {
				white(i, j);
			}
			if (j == 10.0 || j == 9.5 || j == 7.5 || j <= -10.0 &&j >= -11.5) {
				ear_outside(i, j);
			}
			if (j <= 4.5 && j >= 3.0 || j <= -3.5 && j >= -5.5) {
				hair(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 0.5 && j >= -0.5) {
				skin(i, j);
			}
			if (j <= 2.0 && j >= 1.0 || j <= -1.0 && j >= -3.0) {
				eyelash(i, j);
			}
			if (j == 11.0 || j == 10.5 || j == 7.0 || j == 5.0 || j == -6.0 || j == -9.0 || j == -9.5 || j == -12.0) {
				black(i, j);
			}
		}
		if (i == -3.0) {
			if (j == 2.0 || j == 1.0 || j == -2.5 || j == -2.5) {
				white(i, j);
			}
			if (j <= 9.0 && j >= 8.0) {
				ear_outside(i, j);
			}
			if (j <= 5.0 && j >= 3.0 || j <= -3.0 && j >= -6.0) {
				hair(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 0.5 && j >= -1.0) {
				skin(i, j);
			}
			if (j == -1.5 || j == -2.0) {
				eye_thick(i, j);
			}
			if (j == 1.5) {
				eye(i, j);
			}
			if (j == 9.5 || j == 7.5 || j == 5.5 || j == -6.5 || j <= -10.0 && j >= -11.5) {
				black(i, j);
			}
		}
		if (i == -2.5) {
			if (j == 2.0) {
				white(i, j);
			}
			if (j <= 5.5 && j >= 3.0 || j <= -3.5 && j >= -6.5) {
				hair(i, j);
			}
			if (j == 2.5 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 0.5 && j >= -1.0) {
				skin(i, j);
			}
			if (j == 1.0 || j == -2.0) {
				eye_thick(i, j);
			}
			if (j == 1.5 || j == -1.5 || j == -2.5) {
				eye(i, j);
			}
			if (j <= 9.0 && j >= 8.0 || j == 6.0 || j == -7.0) {
				black(i, j);
			}
		}
		if (i == -2.0) {
			if (j == 2.0) {
				white(i, j);
			}
			if (j <= 5.5 && j >= 3.5 || j <= -3.5 && j >= -7.0) {
				hair(i, j);
			}
			if (j == 3.0 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.5 || j <= 0.5 && j >= -1.0) {
				skin(i, j);
			}
			if (j == 1.5 || j == 1.0 || j <= -1.5 && j >= -2.5) {
				eye(i, j);
			}
			if (j == 6.0 || j == 5.0 || j == -7.5 || j <= -10.0 && j >= -14.0) {
				black(i, j);
			}
		}
		if (i == -1.5) {
			if (j <= -10.0 && j >= -14.0) {
				hammer_scarlet(i, j);
			}
			if (j == 2.0) {
				white(i, j);
			}
			if (j <= 6.0 && j >= 3.5 || j <= -3.5 && j >= -5.0 || j <= -5.5 && j >= -7.5) {
				hair(i, j);
			}
			if (j == 3.0 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 2.5 || j <= 0.5 && j >= -1.0) {
				skin(i, j);
			}
			if (j == 1.5 || j == 1.0 || j <= -1.5 && j >= -2.5) {
				eye_thin(i, j);
			}
			if (j == 6.5 || j == -8.0 || j == -9.5 || j == -14.5) {
				black(i, j);
			}
		}
		if (i == -1.0) {
			if (j == -10.0 || j == -11.5) {
				hammer_scarlet(i, j);
			}
			if (j == -10.5 || j == -11.0 || j <= -12.0 && j >= -14.0) {
				hammer_dark_red(i, j);
			}
			if (j == 6.5 || j == 6.0 || j <= 5.0 && j >= 3.5 || j <= -3.0 && j >= -5.5 || j <= -6.5 && j >= -8.0) {
				hair(i, j);
			}
			if (j == -6.0) {
				hair_shadow(i, j);
			}
			if (j == 3.0 || j == -3.0) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.5 && j >= -2.5) {
				skin(i, j);
			}
			if (j == 2.0 || j == 1.5 || j <= -1.0 && j >= -2.0) {
				eye_thin(i, j);
			}
			if (j == 7.0 || j == 5.5 || j == -8.5 || j == -9.5 || j == -14.5) {
				black(i, j);
			}
		}
		if (i == -0.5) {
			if (j <= -10.5 && j >= -13.5) {
				hammer_dark_red(i, j);
			}
			if (j <= 5.0 && j >= 3.0 || j <= -3.0 && j >= -6.0 || j <= -7.0 && j >= -8.0) {
				hair(i, j);
			}
			if (j == -2.5 || j == -6.5) {
				hair_shadow(i, j);
			}
			if (j == 2.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= 0.0 || j <= -1.0 && j >= -2.0) {
				skin(i, j);
			}
			if (j == -0.5) {
				mouth(i, j);
			}
			if (j <= 6.5 && j >= 5.5 || j == -8.5 || j == -10.0 || j == -14.0) {
				black(i, j);
			}
		}
		if (i == 0.0) {
			if (j == -10.5 || j == -11.0 || j == -13.0) {
				hammer_dark_red(i, j);
			}
			if (j <= -11.5 && j >= -12.5) {
				hammer_yellow(i, j);
			}
			if (j <= 5.5 && j >= 2.5 || j <= -3.0 && j >= -6.5) {
				hair(i, j);
			}
			if (j == -2.5 || j == -7.0) {
				hair_shadow(i, j);
			}
			if (j == 2.0 || j == 1.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 1.0 && j >= 0.0 || j == -1.5 || j == -2.0) {
				skin(i, j);
			}
			if (j == -0.5 || j == -1.0) {
				mouth(i, j);
			}
			if (j == 6.0 || j == -7.5 || j == -8.0 || j == -10.0 || j == -13.5) {
				black(i, j);
			}
		}
		if (i == 0.5) {
			if (j == -11.5) {
				hammer_scarlet(i, j);
			}
			if (j == -11.0) {
				hammer_dark_red(i, j);
			}
			if (j == -12.0 || j == -12.5) {
				hammer_yellow(i, j);
			}
			if (j <= 5.5 && j >= 1.5 || j <= -1.5 && j >= -3.5 || j <= -5.0 && j >= -6.5) {
				hair(i, j);
			}
			if (j == -4.5 || j == -7.5) {
				hair_shadow(i, j);
			}
			if (j <= 1.0 && j >= -1.0) {
				skin_shadow_thick(i, j);
			}
			if (j == 6.0 || j == -4.0 || j == -7.0 || j == -8.0 || j == -9.5 || j == -10.5 || j == -13.5) {
				black(i, j);
			}
			if (j == -10.0 || j == -13.0) {
				white(i, j);
			}
		}
		if (i == 1.0) {
			if (j == -11.0) {
				hammer_dark_red(i, j);
			}
			if (j <= -11.5 && j >= -12.5) {
				hammer_yellow(i, j);
			}
			if (j <= 6.0 && j >= 5.0 || j == 3.5 || j == 3.0 || j <= -5.5 && j >= -6.5) {
				hair(i, j);
			}
			if (j == 2.5 || j == 2.0 || j == -5.0) {
				hair_shadow(i, j);
			}
			if (j == 1.5 || j == -1.5) {
				skin_shadow_thin(i, j);
			}
			if (j <= -0.5 && j >= -2.5) {
				skin(i, j);
			}
			if (j <= 1.0 && j >= 0.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 6.5 || j == 4.5 || j == 4.0 || j == -4.5 || j == -7.0 || j == -7.5 || j == -9.5 || j == -10.5 || j == -13.5) {
				black(i, j);
			}
			if (j <= -3.0 && j >= -4.0 || j == -10.0 || j == -13.0) {
				white(i, j);
			}
		}
		if (i == 1.5) {
			if (j == -11.0) {
				hammer_dark_red(i, j);
			}
			if (j <= -11.5 && j >= -12.5) {
				hammer_yellow(i, j);
			}
			if (j <= 6.5 && j >= 5.5 || j == 3.5 || j == 3.0) {
				hair(i, j);
			}
			if (j == 2.5) {
				hair_shadow(i, j);
			}
			if (j <= 1.0 && j >= 0.0 || j == -2.0 || j == -3.5 || j == -4.0) {
				skin(i, j);
			}
			if (j == 1.5 || j <= -0.5 && j >= -1.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 0.5 || j == -1.0) {
				bunny_suit_shadow(i, j);
			}
			if (j == 7.0 || j == 5.0 || j == 4.0 || j == 2.0 || j <= -4.5 && j >= -9.5 || j == -10.5 || j == -13.5) {
				black(i, j);
			}
			if (j == -2.5 || j == -3.0 || j == -10.0 || j == -13.0) {
				white(i, j);
			}
		}
		if (i == 2.0) {
			if (j == -5.5) {
				hammer_scarlet(i, j);
			}
			if (j <= -8.0 && j >= -9.5 || j == -11.0) {
				hammer_dark_red(i, j);
			}
			if (j <= -6.0 && j >= -7.5 || j <= -11.5 && j >= -12.5) {
				hammer_yellow(i, j);
			}
			if (j == 6.0 || j == 3.5) {
				hair(i, j);
			}
			if (j <= 0.5 && j >= -1.0 || j == -4.0 || j == -4.5) {
				skin(i, j);
			}
			if (j == 1.5 || j == 1.0 || j == -3.0 || j == -3.5) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 1.0) {
				bunny_suit(i, j);
			}
			if (j == 6.5 || j == 5.5 || j == 4.0 || j <= 3.0 && j >= 2.0 || j == -5.0 || j == -10.5 || j == -13.5) {
				black(i, j);
			}
			if (j <= -1.5 && j >= -2.5 || j == -10.0 || j == -13.0) {
				white(i, j);
			}
		}
		if (i == 2.5) {
			if (j == -11.0) {
				hammer_dark_red(i, j);
			}
			if (j <= -11.5 && j >= -12.5) {
				hammer_yellow(i, j);
			}
			if (j <= 0.5 && j >= -1.0 || j == -4.0) {
				skin_shadow_thin(i, j);
			}
			if (j == -3.0 || j == -3.5) {
				skin(i, j);
			}
			if (j == 1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 1.0) {
				bunny_suit(i, j);
			}
			if (j == -1.5) {
				shadow2(i, j);
			}
			if (j == 6.0 || j == 3.5 || j == 2.0 || j <= -4.5 && j >= -9.5 || j == -10.5 || j == -13.5) {
				black(i, j);
			}
			if (j == -2.0 || j == -2.5 || j == -10.0 || j == -13.0) {
				white(i, j);
			}
		}
		if (i == 3.0) {
			if (j == -11.0) {
				hammer_dark_red(i, j);
			}
			if (j <= -11.5 && j >= -12.5) {
				hammer_yellow(i, j);
			}
			if (j == -3.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j <=1.0 && j >= -1.5) {
				bunny_suit(i, j);
			}
			if (j == 0.5 || j == -1.0 || j == -1.5) {
				shadow1(i, j);
			}
			if (j == -2.0 || j == -2.5) {
				shadow2(i, j);
			}
			if (j == 2.0 || j == -3.5 || j == -9.5 || j == -10.5 || j == -13.5) {
				black(i, j);
			}
			if (j == -10.0 || j == -13.0) {
				white(i, j);
			}
		}
		if (i == 3.5) {
			if (j == -11.5) {
				hammer_scarlet(i, j);
			}
			if (j == -11.0) {
				hammer_dark_red(i, j);
			}
			if (j == -12.0 || j == -12.5) {
				hammer_yellow(i, j);
			}
			if (j == 2.0 || j == -1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 1.5 && j >= -1.0) {
				bunny_suit(i, j);
			}
			if (j == 2.5 || j == -2.0 || j == -9.5 || j == -10.5 || j == -13.5) {
				black(i, j);
			}
			if (j == 1.5 || j == -10.0 || j == -13.0) {
				white(i, j);
			}
		}
		if (i == 4.0) {
			if (j == -10.5 || j == -11.0 || j == -13.0) {
				hammer_dark_red(i, j);
			}
			if (j <= -11.5 && j >= -12.5) {
				hammer_yellow(i, j);
			}
			if (j == 2.0 || j == -1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j <= 1.5 && j >= -1.0) {
				bunny_suit(i, j);
			}
			if (j == 2.5 || j == -2.0 || j == -10.0 || j == -13.5) {
				black(i, j);
			}
		}
		if (i == 4.5) {
			if (j <= -10.5 && j >= -13.5) {
				hammer_dark_red(i, j);
			}
			if (j <= 1.5 && j >= -1.0) {
				bunny_suit(i, j);
			}
			if (j == 2.0 || j == -1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 2.5 || j == -2.0 || j == -10.0 || j == -14.0) {
				black(i, j);
			}
		}
		if (i == 5.0) {
			if (j == -11.0) {
				hammer_scarlet(i, j);
			}
			if (j == -10.0 || j == -10.5 || j <= -11.5 && j >= -14.0) {
				hammer_dark_red(i, j);
			}
			if (j <= 2.0 && j >= 1.0 || j <= -0.5 && j >= -1.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 0.5 || j == 0.0) {
				bunny_suit(i, j);
			}
			if (j == 2.5 || j == -2.0 || j == -9.5 || j == -14.5) {
				black(i, j);
			}
		}
		if (i == 5.5) {
			if (j <= -10.0 && j >= -14.0) {
				hammer_scarlet(i, j);
			}
			if (j == 2.5 || j == 0.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j <= 2.0 && j >= 1.0 || j == -0.5 || j == -1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 0.5) {
				bunny_suit_shadow(i, j);
			}
			if (j == 1.5 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 3.0 || j == -2.0 || j == -9.5 || j == -14.5) {
				black(i, j);
			}
		}
		if (i == 6.0) {
			if (j == 2.0 || j == 1.5 || j == -0.5 || j == -1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 1.0 || j == 0.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == 3.0 || j == 0.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 3.5 || j == 0.5 || j == -2.0 || j <= -10.0 && j >= -14.0) {
				black(i, j);
			}
			if (j == 2.5) {
				white(i, j);
			}
		}
		if (i == 6.5) {
			if (j == 0.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -0.5 || j == -1.0) {
				skin_shadow_thin(i, j);
			}
			if (j == 3.5 || j == 1.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 4.0 || j == 1.0 || j == 0.5 || j == -2.0) {
				black(i, j);
			}
			if (j <= 3.0 && j >= 2.0) {
				white(i, j);
			}
		}
		if (i == 7.0) {
			if (j == 0.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -0.5 || j == -1.0) {
				skin(i, j);
			}
			if (j == 4.0 || j == 2.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 4.5 || j == 1.5 || j == 0.5 || j == -2.0) {
				black(i, j);
			}
			if (j <= 3.5 && j >= 2.5) {
				white(i, j);
			}
		}
		if (i == 7.5) {
			if (j == -0.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -0.5 || j == -1.0) {
				skin(i, j);
			}
			if (j == 4.0 || j == 2.5) {
				reg_shadow_thick(i, j);
			}
			if (j == 4.5 || j == 2.0 || j == 0.5 || j == -2.0) {
				black(i, j);
			}
			if (j == 3.5 || j == 3.0) {
				white(i, j);
			}
		}
		if (i == 8.0) {
			if (j == 0.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -0.5 || j == -1.0) {
				skin(i, j);
			}
			if (j == 4.0) {
				reg_shadow_thick(i, j);
			}
			if (j == 3.0) {
				toe(i, j);
			}
			if (j == 4.5 || j == 2.5 || j == 0.5 || j == -2.0) {
				black(i, j);
			}
			if (j == 3.5) {
				white(i, j);
			}
		}
		if (i == 8.5) {
			if (j == 0.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -0.5 || j == -1.0) {
				skin(i, j);
			}
			if (j == 4.0 || j == 3.5) {
				toe(i, j);
			}
			if (j == 4.5 || j == 3.0 || j == 0.5 || j == -2.0) {
				black(i, j);
			}
		}
		if (i == 9.0) {
			if (j == 0.0 || j == -1.5) {
				skin_shadow_thick(i, j);
			}
			if (j == -0.5 || j == -1.0) {
				skin(i, j);
			}
			if (j == 4.0 || j == 3.5 || j == 0.5 || j == -2.0) {
				black(i, j);
			}
		}
		if (i == 9.5) {
			if (j == 0.0) {
				skin_shadow_thick(i, j);
			}
			if (j == -0.5 || j == -1.0) {
				skin(i, j);
			}
			if (j == -1.5) {
				toe(i, j);
			}
			if (j == 0.5 || j == -2.0) {
				black(i, j);
			}
		}
		if (i == 10.0) {
			if (j == -0.5 || j == -1.0) {
				toe(i, j);
			}
			if (j == 0.0 || j == -1.5) {
				black(i, j);
			}
		}
		if (i == 10.5) {
			if (j == -0.5 || j == -1.0) {
				black(i, j);
			}
		}
	}
}
