#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#include <GL/glut.h>
#include <math.h>

void put_cube(GLdouble i);

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
void black(GLdouble i, GLdouble j);
void white(GLdouble i, GLdouble j);


int isCOLORON = 1;
//0:GL_COLORオフ ，1:GL_COLORオン
int SHADINGMODE = 0;
//0:スムーズシェーディング，1:フラットシェーディング

/////////////////世界の中心に平面を書くための関数/////////////////
// 　X-Z平面上に幅w,高さdの平面を書く関数です． //
//　 色を決めるための機能はありませんので，関数を呼ぶ前に色を //
// 指定しなければならない． //
////////////////////////////////////////////////////////////////

void Obj_Plane(GLdouble w, GLdouble d)
{
	GLdouble norm[3];
	glPushMatrix();//全体の世界に影響を与えないために行列をよけておく
	glBegin(GL_POLYGON);//多角形を描画するための頂点リストを並べますよ
	norm[0] = 1; norm[1] = 0; norm[2] = 0;//面の向きベクトル（法線）の決定
	glNormal3dv(norm);//法線ベクトルを指定
	glVertex3d(w / 2.0, 0, d / 2.0);//右回りに頂点を決めていく：1点目
	glVertex3d(w / 2.0, 0, -d / 2.0);//右回りに頂点を決めていく：2点目
	glVertex3d(-w / 2.0, 0, -d / 2.0);//右回りに頂点を決めていく：3点目
	glVertex3d(-w / 2.0, 0, d / 2.0);//右回りに頂点を決めていく：4点目
	glEnd();//ここで頂点リストは終わりですよ
	glPopMatrix();//行列を元に戻す
}

///////////世界の中心にチェッカーボード模様を書くための関数/////////
// 　X-Z平面上に幅w,高さdの平面を書く関数 Obj_Planeを使って //
//　 チェッカーボードを作成する関数　　　　　　　　　　　　　　　// 
////////////////////////////////////////////////////////////////

void obj_checker(void) {

	GLfloat color[][4] = {
		{ 1, 1, 1, 1.0 },
		{ 0, 0, 0, 1.0 },
	};

	int bWB = 0;
	double i, j;
	for (i = -10; i < 10.5; i += 0.5) {
		for (j = -10; j < 10; j += 0.5) {
			glPushMatrix();
			if (bWB) {
				if (isCOLORON) {
					glColor3fv(color[0]);
				}
				else {
					glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[0]);
				}
			}
			else {
				if (isCOLORON) {
					glColor3fv(color[1]);
				}
				else {
					glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color[1]);
				}
			}
			bWB = !bWB;
			glTranslated(i, 0, j);
			Obj_Plane(0.5, 0.5);
			glPopMatrix();
		}
		bWB = (int)(fabs(bWB - 1));
	}
}

void subaxis(GLfloat d, GLfloat s) {
	int i;
	double pi = 3.14, t;
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= 6; i++) {
		t = i * 2 * pi / 6;
		glNormal3f((GLfloat)cos(t), 0.0, (GLfloat)sin(t));
		glVertex3f((GLfloat)(d * cos(t)), -s, (GLfloat)(d * sin(t)));
		glVertex3f((GLfloat)(d * cos(t)), s, (GLfloat)(d * sin(t)));
	}
	glEnd();
	glTranslatef(0.0, s, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCone(2.0*d, 4.0*d, 5, 5);
}

void obj_axis(GLfloat d, GLfloat s) {

	glEnable(GL_COLOR);
	glPushMatrix();
	glColor3d(1, 0, 0);
	subaxis(d, s);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glColor3d(0, 1, 0);
	subaxis(d, s);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90.0, 0.0, 0.0, 1.0);
	glColor3d(0, 0, 1);
	subaxis(d, s);
	glPopMatrix();
	glDisable(GL_COLOR);
}

void init(void) {
	if (isCOLORON) {
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	}
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	if (SHADINGMODE)glShadeModel(GL_FLAT);//フラットシェーディング
	else glShadeModel(GL_SMOOTH);//スムーズシェーディング
								 //
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
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
	gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/*こいつでカメラをいじるカメラ位置のx,y,z　カメラの向く座標のx,y,z　画面の上方向を指すベクトル*/
	gluLookAt(0.1, 30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}

void create_cube()
{
	GLdouble i;

	for (i = -10.0; i < 10.5; i += 0.5) {
		put_cube(i);
	}
}

void display()
{
	static GLfloat diffuse[3] = { 0.2f, 0.8f, 0.2f };
	static GLfloat ambient[3] = { 0.2f, 0.8f, 0.2f };
	static GLfloat specular[3] = { 1.0f, 1.0f, 1.0f };
	static GLfloat shininess[1] = { 32.0f };

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	create_cube();

	obj_checker();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
	case 'Q':
	case '\033': /* '\033' は ESC の ASCII コード */
		exit(0);
	default:
		break;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Lighting Sample");
	init();
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
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
	glColor3d(0.55, 0.40, 1.0);//ちょこっとの影(胸)
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

//cubeを置く場所と色を決定して置く
//くそ長い
void put_cube(GLdouble i) {
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
			if (j == 1.5 || j == -2.5) {
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
			if (j == - 1.0 || j == -2.5) {
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
			if (j == 4.0 || j == 2.5 || j == 2.0 || j == -3.0 || j == -3.5 || j <= -4.5 && j >= -6.0) {
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
		if (i == 10.0) {
			if (j == 1.5 || j == 1.0 || j == -2.0 || j == -2.5) {
				black(i, j);
			}
		}
	}
}