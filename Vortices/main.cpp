#include<GL/freeglut.h>
#include<math.h>
#include<iostream>
#define pi 3.14159265359
#define e 2.71828182846
#define vectorScale 0.08
#define FIRST_INTENSITY 0.0
#define COEFS 1.0
#define EPSILON 0.0000001


//vortices coordinates
double vortXarr[]{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
double vortYarr[]{ -0.4, -0.3, -0.2, -0.1, 0., 0.1, 0.2, 0.3, 0.4, 0.5 };

const int n = (sizeof(vortXarr)/sizeof(vortXarr[0]))-1;
const int m = 100;
//midpoints between vortices
double midpointX[n];
double midpointY[n]; 

//pre-normalized and then normalized vectors
double normXarr[n];	
double normYarr[n];
double tmp[n]; 


//V_j_x = (y_0j - y)/(2Pi*(R_j)^2); analog for V_j_y:
double V_j_Xarr[n + 1];
double V_j_Yarr[n + 1];

//V on infinity:
double VinfX[m];
double VinfY[m];

void mass()
{
	for (int i = 0; i < m; i++)
	{
		VinfX[i] = 1.0;
		VinfY[i] = 0.0;
	}
}

//V=v_inf+sum(intensity*v_j)
double V_X[m], V_Y[m];
double sumIntVjX = 0;
double sumIntVjY = 0;


//Intensities and b for SLAR
double G_j_Xarr[n][n+1];
double G_j_Yarr[n][n+1];
double bArrX[n];
double bArrY[n];
double intensities[n+1];

double denominator(double x[], double y[], int i) 
{
	return sqrt(pow((x[i + 1] - x[i]), 2) + pow((y[i + 1] - y[i]), 2));
}

void midpoints(double x[], double y[]) 
{ 
	for (int i = 0; i < n; i++) //from 0-8
	{
		midpointX[i] = (x[i + 1] + x[i]) / 2.;
		midpointY[i] = (y[i + 1] + y[i]) / 2.;
	}
}

//tau_k=((x_k+1-x_k),(y_k+1-y_k))/(sqrt((x_k+1-x_k)^2+(y_k+1-y_k)^2)
void normCoords(double x[], double y[])
{
	for (int i = 0; i < n; i++)
	{	
		normXarr[i] = (x[i + 1] - x[i]) / denominator(x, y, i);
		normYarr[i] = (y[i + 1] - y[i]) / denominator(x, y, i);
	}

	//	tau(x,y); n(x,y)=tau(-y,x)
	for (int i = 0; i < n; i++)	 
	{
		tmp[i] = normXarr[i];
		normXarr[i] = -normYarr[i];
		normYarr[i] = tmp[i];
	}
}

double V_j_X(double x, double y, int i)
{
	return	V_j_Xarr[i]=(vortYarr[i] - y) / (sqrt(pow((x - vortXarr[i]), 2) + pow((y - vortYarr[i]), 2)));
}

double V_j_Y(double x, double y, int i)
{
	return	V_j_Yarr[i] = (vortXarr[i] - x) / (sqrt(pow((x - vortXarr[i]), 2) + pow((y - vortYarr[i]), 2)));
}


double sumIntensityVjX(double x, double y)
{
	for (int i = 0; i < +n;i++)
	{
		sumIntVjX = sumIntVjX + intensities[i] * V_j_X(x,y,i);

	}
	return sumIntVjX;
}

double sumIntensityVjY(double x, double y)
{
	for (int i = 0; i < +n;i++)
	{
		sumIntVjY = sumIntVjY + intensities[i] * V_j_Y(x, y, i);

	}
	return sumIntVjY;
}

void V_x(double x, double y) 
{
	for (int i = 0; i < m; i++)
	{
		V_X[i] = VinfX[i] + sumIntensityVjX(x,y);
	}
}

void V_y(double x, double y)
{
	for (int i = 0; i < m; i++)
	{
		V_Y[i] = VinfY[i] + sumIntensityVjY(x, y);
	}
}




void count(double x, double y) {
	
	for (int i = 0; i < n-1;i++) {
		for (int j = 0; j <= n;j++) {
			bArrX[i] = -(VinfX[i] * normXarr[i]);
			bArrY[i] = -(VinfY[i] * normYarr[i]);
			G_j_Xarr[i][j] = V_j_X(x, y, j)*normXarr[i];
			G_j_Yarr[i][j] = V_j_Y(x, y, j)*normYarr[i];
		}
	}
	
	bArrX[n] = bArrY[n] = FIRST_INTENSITY;

	for (int j = 0;j <= n;j++)
	{
		G_j_Xarr[n - 1][j] = G_j_Yarr[n - 1][j] = COEFS;
	}

	double x1, x2, x3, x4, x5, x6, x7,x8, x9, x10, x1_old, x2_old, x3_old, x4_old, x5_old, x6_old, x7_old, x8_old, x9_old, x10_old;
	double y1, y2, y3, y4, y5, y6, y7, y8, y9, y10, y1_old, y2_old, y3_old, y4_old, y5_old, y6_old, y7_old, y8_old, y9_old, y10_old;
	x1 = 0.1;
	x2 = 0.1;
	x3 = 0;
	x4 = 0;
	x5 = 0.3;
	x6 = 0;
	x7 = 0.26;
	x8 = 0.1;
	x9 = 0.07;
	x10 = 0.06;

	y1 = 0.13;
	y2 = 0.1;
	y3 = 0;
	y4 = 0;
	y5 = 0.6;
	y6 = 0;
	y7 = 0.06;
	y8 = 0.1;
	y9 = 0.07;
	y10 = 0.06;

	while (true)
	{
		x1_old = x1;
		x2_old = x2;
		x3_old = x3;
		x4_old = x4;
		x5_old = x5;
		x6_old = x6;
		x7_old = x7;
		x8_old = x8;
		x9_old = x9;
		x10_old = x10;

		y1_old = y1;
		y2_old = y2;
		y3_old = y3;
		y4_old = y4;
		y5_old = y5;
		y6_old = y6;
		y7_old = y7;
		y8_old = y8;
		y9_old = y9;
		y10_old = y10;

		x1 = bArrX[0] - G_j_Xarr[0][1] * x2 - G_j_Xarr[0][2] * x3 - G_j_Xarr[0][3] * x4 - G_j_Xarr[0][4] * x5 - G_j_Xarr[0][5] * x6 - G_j_Xarr[0][6] * x7 - G_j_Xarr[0][7] * x8 - G_j_Xarr[0][8] * x9 - G_j_Xarr[0][9] * x10;
		x2 = bArrX[1] - G_j_Xarr[1][0] * x1 - G_j_Xarr[1][2] * x3 - G_j_Xarr[1][3] * x4 - G_j_Xarr[1][4] * x5 - G_j_Xarr[1][5] * x6 - G_j_Xarr[1][6] * x7 - G_j_Xarr[1][7] * x8 - G_j_Xarr[1][8] * x9 - G_j_Xarr[1][9] * x10;
		x3 = bArrX[2] - G_j_Xarr[2][1] * x2 - G_j_Xarr[2][0] * x1 - G_j_Xarr[2][3] * x4 - G_j_Xarr[2][4] * x5 - G_j_Xarr[2][5] * x6 - G_j_Xarr[2][6] * x7 - G_j_Xarr[2][7] * x8 - G_j_Xarr[2][8] * x9 - G_j_Xarr[2][9] * x10;
		x4 = bArrX[3] - G_j_Xarr[3][1] * x2 - G_j_Xarr[3][2] * x3 - G_j_Xarr[3][0] * x1 - G_j_Xarr[3][4] * x5 - G_j_Xarr[3][5] * x6 - G_j_Xarr[3][6] * x7 - G_j_Xarr[3][7] * x8 - G_j_Xarr[3][8] * x9 - G_j_Xarr[3][9] * x10;
		x5 = bArrX[4] - G_j_Xarr[4][1] * x2 - G_j_Xarr[4][2] * x3 - G_j_Xarr[4][3] * x4 - G_j_Xarr[4][0] * x1 - G_j_Xarr[4][5] * x6 - G_j_Xarr[4][6] * x7 - G_j_Xarr[4][7] * x8 - G_j_Xarr[4][8] * x9 - G_j_Xarr[4][9] * x10;
		x6 = bArrX[5] - G_j_Xarr[5][1] * x2 - G_j_Xarr[5][2] * x3 - G_j_Xarr[5][3] * x4 - G_j_Xarr[5][4] * x5 - G_j_Xarr[5][0] * x1 - G_j_Xarr[5][6] * x7 - G_j_Xarr[5][7] * x8 - G_j_Xarr[5][8] * x9 - G_j_Xarr[5][9] * x10;
		x7 = bArrX[6] - G_j_Xarr[6][1] * x2 - G_j_Xarr[6][2] * x3 - G_j_Xarr[6][3] * x4 - G_j_Xarr[6][4] * x5 - G_j_Xarr[6][5] * x6 - G_j_Xarr[6][0] * x1 - G_j_Xarr[6][7] * x8 - G_j_Xarr[6][8] * x9 - G_j_Xarr[6][9] * x10;
		x8 = bArrX[7] - G_j_Xarr[7][1] * x2 - G_j_Xarr[7][2] * x3 - G_j_Xarr[7][3] * x4 - G_j_Xarr[7][4] * x5 - G_j_Xarr[7][5] * x6 - G_j_Xarr[7][6] * x7 - G_j_Xarr[7][0] * x1 - G_j_Xarr[7][8] * x9 - G_j_Xarr[7][9] * x10;
		x9 = bArrX[8] - G_j_Xarr[8][1] * x2 - G_j_Xarr[8][2] * x3 - G_j_Xarr[8][3] * x4 - G_j_Xarr[8][4] * x5 - G_j_Xarr[8][5] * x6 - G_j_Xarr[8][6] * x7 - G_j_Xarr[8][7] * x8 - G_j_Xarr[8][0] * x1 - G_j_Xarr[8][9] * x10;
		x10 = bArrX[9] - G_j_Xarr[9][1] * x2 - G_j_Xarr[9][2] * x3 - G_j_Xarr[9][3] * x4 - G_j_Xarr[9][4] * x5 - G_j_Xarr[9][5] * x6 - G_j_Xarr[9][6] * x7 - G_j_Xarr[9][7] * x8 - G_j_Xarr[9][8] * x9 - G_j_Xarr[9][0] * x1;

		if (std::abs(std::abs(x1_old) - std::abs(x1)) < EPSILON && std::abs(std::abs(x2_old) - std::abs(x2)) < EPSILON &&
			std::abs(std::abs(x3_old) - std::abs(x3)) < EPSILON && std::abs(std::abs(x4_old) - std::abs(x4)) < EPSILON &&
			std::abs(std::abs(x5_old) - std::abs(x5)) < EPSILON && std::abs(std::abs(x6_old) - std::abs(x6)) < EPSILON &&
			std::abs(std::abs(x7_old) - std::abs(x7)) < EPSILON && std::abs(std::abs(x8_old) - std::abs(x8)) < EPSILON &&
			std::abs(std::abs(x9_old) - std::abs(x9)) < EPSILON && std::abs(std::abs(x10_old) - std::abs(x10)) < EPSILON)
			break;
	}
	intensities[0] = x1; intensities[1] = x2; intensities[2] = x3;intensities[3] = x4;intensities[4] = x5;intensities[5] = x6;intensities[6] = x7;intensities[7] = x8;
	intensities[8] = x9;intensities[9] = x10;
	V_x(x, y);
	V_y(x, y);
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
	//y
	glVertex2f(0., -10.0f);		
	glVertex2f(0., 10.0f);
	//x
	glVertex2f(-10.0f, 0.);		
	glVertex2f(10.0f, 0.);
	glEnd();
	//*****************************
	glColor3d(1.0, 0.0, 0.0);
	glPointSize(3.f);
	glBegin(GL_POINTS);
	midpoints(vortXarr, vortYarr);
	normCoords(vortXarr, vortYarr);
	mass();
	for (int i = 0; i <=n; i++)
	{
		glVertex2d(vortXarr[i], vortYarr[i]);
	}
	glEnd();

	glColor3d(0.0, 1.0, 0.0);
	glPointSize(3.f);
	glBegin(GL_LINES);
	double width = 0.2;
	double xi = 0.2, yi = 0.2;
	for (int fstep = 0; fstep < m; fstep++)
	{
		for (int step = 0; step < m; step++) {
			count(xi, yi);
			glVertex2d(xi, yi);
			glVertex2d(xi + vectorScale * V_j_Xarr[step], yi + vectorScale * V_j_Yarr[step]);
			xi += width;
		}
		yi += width;
	}
	//
	//;
	for (int i = 0; i < n; i++) 
	{
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