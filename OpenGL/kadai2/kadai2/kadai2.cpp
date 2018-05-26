#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

void DrawString(const char *str, void *font, float x, float y, float z);
void obj_terrain();
void obj_rectangular(const GLdouble*);
void obj_building1();
void obj_building2();
void obj_bridge();
void obj_curbs();
void obj_road();
void obj_center();
void obj_cube();
void obj_pyramid();
void obj_house();
void obj_tree();
void set_curb_long();
void set_curb_short();
void set_curb_very_short();
void set_object();
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void my_mouse_func(int button, int state, int x, int y);
void my_mouse_motion(int x, int y);

int isCOLORON = 1;
//0:GL_COLORオフ ，1:GL_COLORオン
int SHADINGMODE = 0;
//0:スムーズシェーディング，1:フラットシェーディング

//------------- 各種データ構造 ------------------------//
//マウスの状態
struct _MOUSE
{
	int Xstart, Ystart;
	bool flag;
	double weight;
};
_MOUSE MouseStatus = { 0,0,false,0.5 };

//回転関係
struct _ObjectRotate
{
	double xAngle, yAngle;
};
_ObjectRotate ObjRot = { 0,0 };

void GLUT_INIT() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 800);
}

void GLUT_CALL_FUNC() {
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(my_mouse_func);
	glutMotionFunc(my_mouse_motion);
}

void obj_plane(GLdouble w1, GLdouble w2, GLdouble h, GLdouble d1, GLdouble d2){

	GLdouble norm[3];
	glPushMatrix();//全体の世界に影響を与えないために行列をよけておく
	glBegin(GL_POLYGON);//多角形を描画するための頂点リストを並べますよ
	norm[0] = 1; norm[1] = 0; norm[2] = 0;//面の向きベクトル（法線）の決定
	glNormal3dv(norm);//法線ベクトルを指定
	glVertex3d(w1 / 2.0, h, d1 / 2.0);//右回りに頂点を決めていく：1点目
	glVertex3d(w1 / 2.0, h, d2 / 2.0);//右回りに頂点を決めていく：2点目
	glVertex3d(w2 / 2.0, h, d2 / 2.0);//右回りに頂点を決めていく：3点目
	glVertex3d(w2 / 2.0, h, d1 / 2.0);//右回りに頂点を決めていく：4点目
	glEnd();//ここで頂点リストは終わりですよ
	glPopMatrix();//行列を元に戻す
}

void obj_terrain(void) {

	glColor3d(0.4, 0.39, 0.38);
	obj_plane(40.0, 0.0, 0.0, 40.0, -40.0);
	glColor3d(0.0, 0.39, 0.0);
	obj_plane(0.0, -40.0, 0.0, 40.0, -40.0);
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

void display()
{
	void *font = GLUT_BITMAP_HELVETICA_18;
	char str[256];
	static GLfloat diffuse[3] = { 0.2f, 0.8f, 0.2f };
	static GLfloat ambient[3] = { 0.2f, 0.8f, 0.2f };
	static GLfloat specular[3] = { 1.0f, 1.0f, 1.0f };
	static GLfloat shininess[1] = { 32.0f };

	glClearColor(0.5f, 0.8f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(5.0, 20.0, 40.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glRotated(ObjRot.xAngle, 1, 0, 0);
	glRotated(ObjRot.yAngle, 0, 1, 0);

	set_object();
	sprintf_s(str, "%f", ObjRot.xAngle);

	glColor3d(0.0, 1.0, 1.0);
	DrawString(str, font, -9.0, 2.0, 14.0);
	glutSwapBuffers();
	glFlush();
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

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
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

void my_mouse_func(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON &&state == GLUT_DOWN) {
		MouseStatus.Xstart = x;
		MouseStatus.Ystart = y;
		MouseStatus.flag = true;
	}
	else {
		MouseStatus.flag = false;
	}

}

void my_mouse_motion(int x, int y)
{
	if (MouseStatus.flag == false)return;

	static int xdir, ydir;

	xdir = x - MouseStatus.Xstart;
	ydir = y - MouseStatus.Ystart;

	ObjRot.xAngle += (double)ydir * MouseStatus.weight;
	ObjRot.yAngle += (double)xdir * MouseStatus.weight;

	MouseStatus.Xstart = x;
	MouseStatus.Ystart = y;

	glutPostRedisplay();
}

void DrawString(const char *str, void *font, float x, float y, float z)
{
	glRasterPos3f(x, y, z);
	while (*str) {
		glutBitmapCharacter(font, *str);
		++str;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	GLUT_INIT();
	glutCreateWindow("都会と田舎");
	init();
	GLUT_CALL_FUNC();
	glutMainLoop();

	return 0;
}

void set_object() {
	obj_terrain();
	obj_building1();
	obj_building2();
	obj_bridge();
	obj_center();
	obj_road();
	obj_tree();
	obj_house();
	obj_curbs();
}

void obj_building1() {

	GLdouble vertex[][3] = {
		{ 10.0, 0.0, 0.0 },
		{ 13.0, 0.0, 0.0 },
		{ 13.0, 10.0, 0.0 },
		{ 10.0, 10.0, 0.0 },
		{ 10.0, 0.0, 2.0 },
		{ 13.0, 0.0, 2.0 },
		{ 13.0, 10.0, 2.0 },
		{ 10.0, 10.0, 2.0 }
	};

	int face[][4] = {
		{ 0, 1, 2, 3 },
		{ 1, 5, 6, 2 },
		{ 5, 4, 7, 6 },
		{ 4, 0, 3, 7 },
		{ 4, 5, 1, 0 },
		{ 3, 2, 6, 7 }
	};

	int i;
	int j;

	glColor3d(0.46, 0.45, 0.44);
	glBegin(GL_QUADS);

	for (j = 0; j < 6; ++j) {
		for (i = 0; i < 4; ++i) {
			obj_rectangular(vertex[face[j][i]]);
		}
	}
	glEnd();
}

void obj_building2() {

	GLdouble vertex[][3] = {
		{ 15.0, 0.0, 0.0 },
		{ 18.0, 0.0, 0.0 },
		{ 18.0, 10.0, 0.0 },
		{ 15.0, 10.0, 0.0 },
		{ 15.0, 0.0, 2.0 },
		{ 18.0, 0.0, 2.0 },
		{ 18.0, 10.0, 2.0 },
		{ 15.0, 10.0, 2.0 }
	};

	int face[][4] = {
		{ 0, 1, 2, 3 },
		{ 1, 5, 6, 2 },
		{ 5, 4, 7, 6 },
		{ 4, 0, 3, 7 },
		{ 4, 5, 1, 0 },
		{ 3, 2, 6, 7 }
	};

	int i;
	int j;

	glColor3d(0.46, 0.45, 0.44);
	glBegin(GL_QUADS);

	for (j = 0; j < 6; ++j) {
		for (i = 0; i < 4; ++i) {
			obj_rectangular(vertex[face[j][i]]);
		}
	}
	glEnd();
}

void obj_bridge() {
	GLdouble vertex[][3] = {
		{ 13.0, 6.0, 0.0 },
		{ 15.0, 6.0, 0.0 },
		{ 15.0, 7.0, 0.0 },
		{ 13.0, 7.0, 0.0 },
		{ 13.0, 6.0, 2.0 },
		{ 15.0, 6.0, 2.0 },
		{ 15.0, 7.0, 2.0 },
		{ 13.0, 7.0, 2.0 }
	};

	int face[][4] = {
		{ 0, 1, 2, 3 },
		{ 1, 5, 6, 2 },
		{ 5, 4, 7, 6 },
		{ 4, 0, 3, 7 },
		{ 4, 5, 1, 0 },
		{ 3, 2, 6, 7 }
	};

	int i;
	int j;

	glColor3d(0.26, 0.25, 0.24);
	glBegin(GL_QUADS);

	for (j = 0; j < 6; ++j) {
		for (i = 0; i < 4; ++i) {
			obj_rectangular(vertex[face[j][i]]);
		}
	}
	glEnd();
}

void obj_curbs() {
	set_curb_long();
	glTranslatef(-4.0, 0.0, 0.0);
	set_curb_long();
	glTranslatef(-12.0, 0.0, 0.0);
	set_curb_long();
	glTranslatef(-4.0, 0.0, 0.0);
	set_curb_long();
	glTranslatef(20.0, 0.0, 0.0);
	set_curb_short();
	glTranslatef(-4.0, 0.0, 0.0);
	set_curb_short();
	glTranslatef(-12.0, 0.0, 0.0);
	set_curb_short();
	glTranslatef(-4.0, 0.0, 0.0);
	set_curb_short();
	glTranslatef(16.0, 0.0, 0.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	set_curb_very_short();
	glTranslatef(0.0, 0.0, -16.0);
	set_curb_short();
	glTranslatef(0.0, 0.0, -16.0);
	set_curb_short();
	glTranslatef(-4.0, 0.0, 32.0);
	set_curb_very_short();
	glTranslatef(0.0, 0.0, -16.0);
	set_curb_short();
	glTranslatef(0.0, 0.0, -16.0);
	set_curb_short();
	glTranslatef(8.0, 0.0, 32.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
}

void set_curb_long() {
	GLdouble vertex[][3] = {
		{ 8.0, 0.0, -20.0 },
		{ 8.0, 0.2, -20.0 },
		{ 7.9, 0.2, -20.0 },
		{ 7.9, 0.0, -20.0 },
		{ 8.0, 0.0, 4.0 },
		{ 8.0, 0.2, 4.0 },
		{ 7.9, 0.2, 4.0 },
		{ 7.9, 0.0, 4.0 }
	};

	int face[][4] = {
		{ 0, 1, 2, 3 },
		{ 1, 5, 6, 2 },
		{ 5, 4, 7, 6 },
		{ 4, 0, 3, 7 },
		{ 4, 5, 1, 0 },
		{ 3, 2, 6, 7 }
	};

	int i;
	int j;

	glColor3d(0.26, 0.25, 0.24);
	glBegin(GL_QUADS);

	for (j = 0; j < 6; ++j) {
		for (i = 0; i < 4; ++i) {
			obj_rectangular(vertex[face[j][i]]);
		}
	}
	glEnd();
}

void set_curb_short() {
	GLdouble vertex[][3] = {
		{ 8.0, 0.0, 8.0 },
		{ 8.0, 0.2, 8.0 },
		{ 7.9, 0.2, 8.0 },
		{ 7.9, 0.0, 8.0 },
		{ 8.0, 0.0, 20.0 },
		{ 8.0, 0.2, 20.0 },
		{ 7.9, 0.2, 20.0 },
		{ 7.9, 0.0, 20.0 }
	};

	int face[][4] = {
		{ 0, 1, 2, 3 },
		{ 1, 5, 6, 2 },
		{ 5, 4, 7, 6 },
		{ 4, 0, 3, 7 },
		{ 4, 5, 1, 0 },
		{ 3, 2, 6, 7 }
	};

	int i;
	int j;

	glColor3d(0.26, 0.25, 0.24);
	glBegin(GL_QUADS);

	for (j = 0; j < 6; ++j) {
		for (i = 0; i < 4; ++i) {
			obj_rectangular(vertex[face[j][i]]);
		}
	}
	glEnd();
}

void set_curb_very_short() {
	GLdouble vertex[][3] = {
		{ 8.0, 0.0, 8.0 },
		{ 8.0, 0.2, 8.0 },
		{ 7.9, 0.2, 8.0 },
		{ 7.9, 0.0, 8.0 },
		{ 8.0, 0.0, 16.0 },
		{ 8.0, 0.2, 16.0 },
		{ 7.9, 0.2, 16.0 },
		{ 7.9, 0.0, 16.0 }
	};

	int face[][4] = {
		{ 0, 1, 2, 3 },
		{ 1, 5, 6, 2 },
		{ 5, 4, 7, 6 },
		{ 4, 0, 3, 7 },
		{ 4, 5, 1, 0 },
		{ 3, 2, 6, 7 }
	};

	int i;
	int j;

	glColor3d(0.26, 0.25, 0.24);
	glBegin(GL_QUADS);

	for (j = 0; j < 6; ++j) {
		for (i = 0; i < 4; ++i) {
			obj_rectangular(vertex[face[j][i]]);
		}
	}
	glEnd();
}

void obj_road() {

	glColor3d(0.3, 0.3, 0.3);
	obj_plane(16.0, 8.0, 0.05, 40.0, -40.0);
	obj_plane(-24.0, -16.0, 0.05, 40.0, -40.0);
	obj_plane(40.0, -40.0, 0.05, 8.0, 16.0);
}

void obj_center() {

	int i;
	glColor3d(1.0, 1.0, 1.0);
	for (i = 0; i < 10; i++) {
		obj_plane(12.2, 11.8, 0.06, 40.0 - 8.0 * i - 1.0, 40.0 - 8.0 * i - 6.0);
		obj_plane(-20.2, -19.8, 0.06, 40.0 - 8.0 * i - 1.0, 40.0 - 8.0 * i - 6.0);
		obj_plane(40.0 - 8.0 * i - 1.0, 40.0 - 8.0 * i - 6.0, 0.06, 12.2,11.8);
	}
	
}

void obj_cube(GLdouble nx, GLdouble ny, GLdouble nz) {

	GLdouble vertex[][3] = {
		{ -0.5, 0.0, -0.5 },
		{ 0.5, 0.0, -0.5 },
		{ 0.5, 1.0, -0.5 },
		{ -0.5, 1.0, -0.5 },
		{ -0.5, 0.0, 0.5 },
		{ 0.5, 0.0, 0.5 },
		{ 0.5, 1.0, 0.5 },
		{ -0.5, 1.0, 0.5 }
	};
	int face[][4] = {
		{ 0, 1, 2, 3 },
		{ 1, 5, 6, 2 },
		{ 5, 4, 7, 6 },
		{ 4, 0, 3, 7 },
		{ 4, 5, 1, 0 },
		{ 3, 2, 6, 7 }
	};
	GLdouble normal[][3] = {
		{ 0.0, 0.0, -1.0 },
		{ 1.0, 0.0, 0.0 },
		{ 0.0, 0.0, 1.0 },
		{ -1.0, 0.0, 0.0 },
		{ 0.0, -1.0, 0.0 },
		{ 0.0, 1.0, 0.0 }
	};

	int i, j;

	glBegin(GL_QUADS);
	for (j = 0; j < 6; j++) {
		glNormal3dv(normal[j]);
		for (i = 0; i < 4; i++) {
			glVertex3d(vertex[face[j][i]][0] * nx,
				vertex[face[j][i]][1] * ny,
				vertex[face[j][i]][2] * nz);
		}
	}
	glEnd();
}

void obj_pyramid(GLdouble nx, GLdouble ny, GLdouble nz) {
	GLdouble vertex[][3] = {
		{ 0.0, 1.0, 0.0 },
		{ -0.5, 0.0, -0.5 },
		{ 0.5, 0.0, -0.5 },
		{ 0.5, 0.0, 0.5 },
		{ -0.5, 0.0, 0.5 },
	};
	int face[][4] = {
		{ 0, 1, 2, 0 },
		{ 0, 2, 3, 0 },
		{ 0, 3, 4, 0 },
		{ 0, 4, 1, 0 },
		{ 1, 4, 3, 2 },
	};

	GLdouble normal[][3] = {
		{ 0.0, 1.0, -1.0 },
		{ 1.0, 1.0, 0.0 },
		{ 0.0, 1.0, 1.0 },
		{ -1.0, 1.0, 0.0 },
	};

	int i, j;

	glBegin(GL_TRIANGLES);
	for (j = 0; j < 4; j++) {
		glNormal3dv(normal[j]);
		for (i = 0; i < 3; i++) {
			glVertex3d(vertex[face[j][i]][0] * nx,
				vertex[face[j][i]][1] * ny,
				vertex[face[j][i]][2] * nz);
		}
	}
	glEnd();
	glBegin(GL_QUADS);
	glNormal3d(0.0, -1.0, 0.0);
	for (i = 0; i < 4; i++) {
		glVertex3d(vertex[face[j][i]][0] * nx,
			vertex[face[j][i]][1] * ny,
			vertex[face[j][i]][2] * nz);
	}
	glEnd();
}

void obj_tree() {
	
	int i;
	int j;
	for (i = -65; i < 0; i += 10) {
		for (j = -190; j < 30; j += 10) {
			glPushMatrix();
			glTranslated(i * 0.1, 0.5, j * 0.1);
			glColor3d(0.0, 0.4, 0.1);
			obj_pyramid(0.4, 1.5, 0.4);
			glPopMatrix();
			glTranslated(i * 0.1, 0.0, j * 0.1);
			glColor3d(0.4, 0.2, 0.0);
			obj_cube(0.1, 0.5, 0.1);
			glTranslated(-i * 0.1, 0.0, -j * 0.1);
		}
	}
	
}

void obj_house() {

	int i;
	for (i = -20; i < 10; i += 10) {
		glPushMatrix();
		glTranslated(-15.0, 0.5, i * 0.6);
		glColor3d(0.0, 0.4, 0.1);
		obj_pyramid(2.5, 0.5, 2.5);
		glPopMatrix();
		glTranslated(-15.0, 0.0, i * 0.6);
		glColor3d(0.4, 0.2, 0.0);
		obj_cube(2.0, 0.5, 2.0);
		glTranslated(15.0, 0.0, -i * 0.6);
	}
	
}

void obj_rectangular(const GLdouble* vertex) {
	glVertex3dv(vertex);
}

//x座標右が正
//z座標手前が正