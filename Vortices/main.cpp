#include<GL/freeglut.h>
#include<math.h>
#define pi 3.14159265359
#define e 2.71828182846
#define vectorScale 0.08
const int n = 9;

double midpointX[n];	//midpoints between vortices
double midpointY[n]; 

double normXarr[n];	//pre-normalized vectors
double normYarr[n];

double tmp[n]; //for getting normalized vectors

double vortXarr[]{ 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., };		//vortices coordinates
double vortYarr[]{ -0.4, -0.3, -0.2, -0.1, 0., 0.1, 0.2, 0.3, 0.4, 0.5 };

double denominator(double x[], double y[], int i) {
	return sqrt(pow((x[i + 1] - x[i]), 2) + pow((y[i + 1] - y[i]), 2));
}

void midpoints(double x[], double y[]) 
{
	for (int i = 0; i < n; i++)
	{
		midpointX[i] = (x[i+1] + x[i]) / 2.;
		midpointY[i] = (y[i + 1] + y[i]) / 2.;
	}
}

void normCoords(double x[], double y[])
{
	for (int i = 0; i < n; i++){	//tau_k=((x_k+1-x_k),(y_k+1-y_k))/(sqrt((x_k+1-x_k)^2+(y_k+1-y_k)^2)
		normXarr[i] = (x[i + 1] - x[i]) / denominator(x,y,i);
		normYarr[i] = (y[i + 1] - y[i]) / denominator(x, y, i);
	}
	for (int i = 0; i < n; i++)	 //	tau(x,y); n(x,y)=tau(-y,x)
	{
		tmp[i] = normXarr[i];
		normXarr[i] = -normYarr[i];
		normYarr[i] = tmp[i];
	}
}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-2,2, -2, 2);
	glMatrixMode(GL_MODELVIEW);
}

void Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3d(0.0, 0.0, 0.0);
	glLineWidth(1);
	//*****************************
	glBegin(GL_LINES);
	glVertex2f(0., -10.0f); //y
	glVertex2f(0., 10.0f);
	glVertex2f(-10.0f, 0.);//x
	glVertex2f(10.0f, 0.);
	glEnd();
	//*****************************
	glColor3d(1.0, 0.0, 0.0);
	glPointSize(3.f);
	glBegin(GL_POINTS);
	for (int i = 0; i <=n;i++) {
		glVertex2d(vortXarr[i], vortYarr[i]);
	}
	glEnd();

	glColor3d(0.0, 1.0, 0.0);
	glPointSize(3.f);
	glBegin(GL_LINES);
	midpoints(vortXarr,vortYarr);
	normCoords(vortXarr,vortYarr);
	for (int i = 0; i < n;i++) {
		glVertex2d(midpointX[i], midpointY[i]);
		glVertex2d(midpointX[i] + vectorScale*normXarr[i], midpointY[i] + normYarr[i]);
	}
	glEnd();

	glFlush();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(1000, 700);
	glutInitWindowPosition(100, 100);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("Vortices");

	glutReshapeFunc(Reshape);
	glutDisplayFunc(Draw);
	glClearColor(1., 1., 1., 1.);

	glutMainLoop();
	return 0;
}