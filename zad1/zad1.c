#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#define PI 3.141596
GLfloat angle, zoom, ye;

GLfloat rlight, alfa;

int light, move;


GLUquadricObj *obj1;


GLfloat pos[4] = {25.0, 35.0, -2.0, 1.0};
GLfloat red[4] = {0.9, 0.0, 0.0, 1.0};
GLfloat green[4] = {0.1, 0.9, 0.2, 1.0};
GLfloat blue[4] = {0.3, 0.3, 1.0, 1.0};
GLfloat white[4] = {1.0f, 1.0f, 1.0f, 1.0f};

GLfloat ambientLight[4] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat diffuseLight[4] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat specularLight[4] = {1.0f, 1.0f, 1.0f, 1.0f};


/*normalizacja wektora*/
void ReduceToUnit(float vector[3])
{
	float length;

	length = (float)sqrt((vector[0]*vector[0]) +
			(vector[1]*vector[1]) +
			(vector[2]*vector[2]));
	if(length == 0.0f)
		length = 1.0f;

	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
}

/*obliczanie wektora normalnego*/
void calcNormal(float v[3][3], float out[3])
{
	float v1[3],v2[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;

	// Calculate two vectors from the three points
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];

	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];

	// Take the cross product of the two vectors to get
	// the normal vector which will be stored in out
	out[x] = v1[y]*v2[z] - v1[z]*v2[y];
	out[y] = v1[z]*v2[x] - v1[x]*v2[z];
	out[z] = v1[x]*v2[y] - v1[y]*v2[x];

	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);
}




void init(char* fileName1, char* fileName2, char* fileName3)
{
	glClearColor(0, 0, 0, 1.0);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);      //wielokąty wypełnione/ połączpne wierzchołki/...
	glEnable(GL_DEPTH_TEST);

	alfa = 0.0f;
	rlight = 50.0f;
	pos[0] = rlight*cos(alfa*PI/180.0);
	pos[2] = rlight*sin(alfa*PI/180.0);
	angle = 0.0f;
	zoom = 55.0f;
	ye = 7.0;

	light = 1;

	obj1 = gluNewQuadric();



	gluQuadricDrawStyle(obj1, GLU_FILL);


	gluQuadricNormals(obj1, GLU_SMOOTH);
	gluQuadricOrientation(obj1, GLU_OUTSIDE);
	gluQuadricTexture(obj1, GLU_TRUE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glDisable(GL_TEXTURE_2D);

	glLightfv(GL_LIGHT0, GL_AMBIENT, red);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, red);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void reshape(int w, int h)      //do rzutowania, "stały element gry", patrz: main
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat) w/ (GLfloat) h, 1.0, 400.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
};

/*display w jakiejs czesci też wszędzie wyglada tak samo*/
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     //czyści bufory
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//if(light)
	//	glEnable(GL_LIGHTING);
	//else
		glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	/*polozenia obserwatora - troche inaczej niz zwykle*/
	glRotatef(180.0f*atan(ye/zoom)/PI, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -ye, -zoom);
	glRotatef(angle, 0.0, 1.0, 0.0);


	glLightfv(GL_LIGHT0, GL_POSITION, pos); //umieszcza w ukłądzie odniesienia światło

	glPushMatrix();
	/*w trakcie rysowania zachodzi konieczność poruszania elemetów sceny względem siebie. pushmatrix zapamiętuje aktualnny stan przekształceń, tzn zapamiętuję w zasadzie położenie układu odniesienia. */

	glPushMatrix();
	glRotatef(0.0f, 1.0, 0.0f, 0.0f);
	/*...*/
	glutWireTorus(2.0,10.0,20,100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10.0f*cos(alfa*PI/180.0), 10.0f*sin(alfa*PI/180.0), 0.0f);
	glRotatef(-90.0f, 1.0, 0.0f, 0.0f);
	glRotatef(-alfa, 0.0, 1.0f, 0.0f);
	glutSolidTorus(0.5,3.0,5,10);
	glPopMatrix();

	glPopMatrix();


	glFinish();     //zmusz potok renderowania do opróżnienia się przed rysowaniem (?)
	glutSwapBuffers();      //zmiana buforów (patrz: double buffer w main)
}


void skeys(int skey, int x, int y ) {
	switch (skey) {
		case GLUT_KEY_LEFT: angle -=2.0f;
				    if (angle < 0.0f) angle = 360.0f;
				    break;
		case GLUT_KEY_RIGHT: angle +=2.0f;
				     if (angle > 360.0f) angle = 0.0f;
				     break;
		case GLUT_KEY_UP: zoom -=2.0f;
				  if(zoom < 2.0f) zoom = 2.0f;
				  break;
		case GLUT_KEY_DOWN: zoom += 2.0f;
				    break;
		case GLUT_KEY_PAGE_UP: ye+=2.0f;
				       break;
		case GLUT_KEY_PAGE_DOWN: ye-=2.0f;
					 break;
	}
	glutPostRedisplay();
}


void keybs(unsigned char key, int x, int y) {

	if(key == 'l')
		light = !light;
	if(key == 'm')
		move = !move;
	glutPostRedisplay();

}


void idle()
{
	alfa+=0.01f;
	if(alfa > 360.0) alfa = 0.0f;
	pos[0] = rlight*cos(alfa*PI/180.0);
	pos[2] = rlight*sin(alfa*PI/180.0);
	glutPostRedisplay();    //wymuszenie przerysowania ekranu
};

/*przedrostek funkcji oznacza skąd wziełą się dakna funkcja - gltu, glu, gl ; gl - natywne
  glu miałą rozszerzyć możliwości openGL,
  glut miał zapewnić przenośność kodu pomiędzy platformami. Zadaniem gluta jest ukrycie tego, że systemu okienkowe w różnych systemach różnią się. openGL nie zawieta funkcji platform specific?.
  */



/*w każdyn OpenGLowym programie(przynajmniej w starym openGL) main będzie wyglądał podobnie*/
int main(int argc, char** argv) {
	glutInit(&argc, argv); //inicjalizuje opengl i gluta. z argumentów w zasadzie się nie korzysta. musi być w tej postaci
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); //jaki chcemy mieć tryb renderowania. GLUT_RGB  - bufor kolorów w formacie RGB(),  GLUT_DOUBLE (chcemy stosować podwójny bufor kolorów - do animacji. piszemy po tym niewidocznym, a potem zamieniamy) GLUT_DEPTH - włłącza algorytm usuwania niewidocznych krawędzi
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);      //obowiązkowa, tworzy wszystkie potrzebne struktury

	init("grass.bmp", "ziemia.bmp", "bricks.bmp");
	//kolejne funkcje to obsługa zdarzeń. mechanizm calllbacków, funkcje wywoływane kiedy zajdzie odpowiednie zdarzenie. arg - nazwa funkcji (wskaźnik na funkcję)   
	glutDisplayFunc(display);       //kiedy zajdzie konieczność przerysowania ekranu
	glutReshapeFunc(reshape);       //zmienił się rozmiar okna (tu musimu od nowa ustawić od nowa parametry rzutowania)
	glutSpecialFunc(skeys);         //naciśniecie kalwisza niealfanumerycznego
	glutKeyboardFunc(keybs);        //naciśniecie alfanumerycznego
	glutIdleFunc(idle);             //program nic nie robi. wywołuje się, kiedy program kończy jakąś operację

	glutMainLoop();
	return 0;
}
