#include "GL/freeglut.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>
#define PI   3.14159265358979323846
#define ld   long double

struct point{
	ld x,y;
	point(){
	}
	point(ld a, ld b) {
		x = a; y = b;
	}
	friend const point operator + (const point& a, const point& b) {
		return point(a.x + b.x, a.y + b.y);
	}
	friend const point operator - (const point& a, const point& b) {
		return point(a.x - b.x, a.y - b.y);
	}
	friend const point operator * (const point& a, const ld r) {
		return point(a.x * r, a.y * r);
	}
	friend const point operator / (const point& a, const ld r) {
		return point(a.x / r, a.y / r);
	}
	void rot(ld alf) // по часовой
	{
		ld x0 = x; ld y0 = y;
		x = x0 * cos(alf) + y0 * sin(alf);
		y = x0 * -sin(alf) + y0 * cos(alf);
	}
	void normalise()
	{
		ld r = sqrt(x*x + y*y);
		x /= r; y /= r;
	}
};
struct frac{
	frac(){
	}
	frac(point pp0, ld dlin0){
		pp = pp0;
		dlin = dlin0;
	}
	point pp;
	ld dlin;
};

// переменные.....
int tick = 100, w=1920, h=1024, iter = 0, mod = 3, r = 0, kol = 7; int ddd = mod;
std::vector < frac > a;
ld DLIN = 450, dlin = DLIN,
wsp = (double)w / 3, hsp = (double)h, // на сколько спавн выше и правее экрана
spdw = 35;                           // кол-во итерац за кот долетит снег
ld w0 = (w - dlin) / 2, h0 = h / 2 - dlin * sqrt(3.0) / 6;
point vec(dlin,0), p(w0, h0), wind( -(double)wsp / spdw, -(double)hsp / spdw ), pnext, rndw;   

// квадрат
int sqr(int a){
	return a*a;
}
// длина
inline ld dl(ld dlina, int it){
	while(it--){
		dlina /= 3;
	}
	return dlina;
}
// цвет
inline short color(){
	return (rand() % 127 + 127) / 2;
} 
// ф-ция построения фрактала
void frac_draw(int it){
	
	if(it){
		frac_draw(it - 1);
		vec.rot( PI / 3 );
		frac_draw(it - 1);
		vec.rot( - 2 * PI / 3 );
		frac_draw(it - 1);
		vec.rot( PI / 3 );
		frac_draw(it - 1);
	}
	else{
		p = p + vec;
		glColor3b(color(), color(), color());
		glVertex2d(p.x, p.y);
	}

}
// ф-ция рисования кадра
void draw(){
	glClear(GL_COLOR_BUFFER_BIT);  
	glLoadIdentity();
	glColor3d(0,0,0); // нач цвет

	for(int i=0; i<a.size(); i++){

	glBegin(GL_LINE_LOOP);  
	int m = 3;                                          // кол во основных итер
	p = a[i].pp;                                        // первая точка
	if(rand() % 3 == 0)                                 // вероятностный сдвиг итерац фрак
		iter += rand() % 3 - 1;
	dlin = dl( a[i].dlin , (iter / 5) % mod + 2);           // начал длина
	vec = point(dlin, 0);                               // начал вектор
	while(m--){
	frac_draw((iter / 5) % mod + 2);
	vec.rot(- 2* PI / 3); // основной поворот
	//vec.normalise();
	//vec = vec * dlin;
	}
	glEnd();

	}

	glFlush();
}
// добавление новой снежинки фрактала
inline frac newsnow(){
	return frac( point( rand() % w + wsp , rand() % h + hsp ), rand() % 450 + 100 );
}

void Timer(int value)
{   
	r = (r + 1) % (mod * kol); // цикл счета
	iter = r % (5*mod);             // цикл итераций фрактала

	if(r / mod >= a.size() ){  // создание новой снежинки до предела kol
		a.push_back( newsnow() );
	}
	for(int i=0; i<a.size(); i++){
		rndw = point(  (rand() % 41 - 20), (rand() % 41 - 20) );
		a[i].pp = a[i].pp + wind;
		a[i].pp = a[i].pp + rndw;
    if (a[i].pp.x < -wsp || a[i].pp.y < -hsp) { // если вылет за границы пересоздаем*
      a[i] = newsnow();
    }
	}


	glutPostRedisplay();
	glutTimerFunc( tick, Timer, 0 );
}

int main(int argc, char** argv){
	srand( time(0) );
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(w,h);
	glutCreateWindow("Snow Koch");

	glClearColor(0,0,0,1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,w,0,h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutDisplayFunc( draw );
	glutTimerFunc( tick, Timer, 0 );
	glutMainLoop();
	return 0;
}
