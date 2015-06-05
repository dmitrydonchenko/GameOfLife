#include "lifegame.h"
#include <QMouseEvent>
#include <QtOpenGL>
#include <QVector>
#include <QFileDialog>
#define SURVIVE 2
#define BORN 3
#define DIE -1

LifeGame::LifeGame(QWidget *parent)
			:QGLWidget(QGLFormat(QGL::SingleBuffer), parent)
{
	mapSize = 50.0f;
	nWidth = mapSize*10.0f;
	nHeight = mapSize*10.0f;
	cellWidth = 50.0f;
	cellHeight = 50.0f;
	mass.resize(200);
	generationNumber = 0;
	for(int i=0; i<200; i++)
		mass[i].resize(200);
}

LifeGame::~LifeGame()
{

}

/*! �������, ������� ��� ������� ������ �� ������ ������ �������� ���������������� ��������
������� � 0 �� 1 ��� ��������
*\param pe - ������� ����
*/
void LifeGame::mousePressEvent(QMouseEvent *pe)
{
	int x = pe->x(), y = pe->y(); // ���������� �����, � ������� ��� ������
	if(pe->button()==Qt::LeftButton) // ���� ������ ����� ������
	{
		int j = x / cellWidth, i = y / cellHeight; // ������ � ����� ������ ��� ������
		if(j >= mapSize) j = mapSize - 1; 
			if(i >= mapSize) i = mapSize - 1;
			//�������� �������� �������� �������, ���������������� ������ ������:
				mass[i][j] = (mass[i][j] == 0)? 1 : 0; 
	}
	paintGL(); // �������� ������� ���������
}

/*! �������, ���������� �� ���������� ��������� ������� ������� OpenGL
*\param w - ������, h - ������
*/
void LifeGame::resizeGL(int w, int h)
{
	nWidth = w;
	nHeight = h;
	glViewport(0.0f, 0.0f, w, h); // ������������� ���� ��������� � ��������� ����
	glMatrixMode(GL_PROJECTION); // ��������� ������� ���������
	glLoadIdentity();
	glOrtho(0.0f, w, h, 0.0f, -5.0f, 5.0f); // ������������ ������� ������ ���������
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// ������ ������ � ������ ������:
	cellWidth = (GLfloat)w/(GLfloat)mapSize; 
	cellHeight = (GLfloat)h/(GLfloat)mapSize;
}

/*! ������� �������������
*/
void LifeGame::initializeGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

/*! ������� ���������
*/
void LifeGame::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT); // ������� ����� ������� ������
	// ����������� ������ ����� ��� ������ ������ � ����������� �� 
	// �������� ��������������� ��������� �������
	for(int i = 0; i < mapSize; i++)
	{
		for(int j = 0; j < mapSize; j++)
		{
			if(!mass[i][j])
				glColor3f(1.0f, 1.0f, 1.0f); // ���� ������� ������� = 0, �� ���� - �����
			else if(mass[i][j])
				glColor3f(0.0f, 0.0f, 0.0f); // ����� - ������
			// ����������� ������
			glRectf((GLfloat)j * cellWidth, (GLfloat)i * cellHeight, (GLfloat)(j + 1) * cellWidth, (GLfloat)(i + 1) * cellHeight);
		}
	}
	glColor3f(0.75f, 0.75f, 0.75f); // ������������� ����� ����
	// ������ �����:
	glBegin(GL_LINES);
	for(GLfloat i = cellWidth; i < nWidth; i += cellWidth)
	{
		glVertex2f(i, 0.0f);
		glVertex2f(i, nHeight);
	}
	for(GLfloat i = cellHeight; i < nHeight; i += cellHeight)
	{
		glVertex2f(0.0f, i);
		glVertex2f(nWidth, i);
	}
	glEnd();
	glFlush();
}

/*! �������, ���������� ��������� ��������� ������
\* return cellsChanged - ���������� ���������� ������
*/
bool LifeGame::nextGeneration()
{
	int count, i, j, cellsChanged = 0;
	QVector< QVector<int> > tmpMass(mapSize, QVector<int>(mapSize, 0)); // ��������������� ������
	// ���������� ��� ������
	for(i = 0; i<mapSize; i++)
	{
		for(j = 0; j<mapSize; j++)
		{
			count = neighbours(i, j); // ������ ����� ����� ������� ��� ������
			// ���� ������ �����:
			if(mass[i][j] == 1)
			{
				if(count == 2 || count == 3)
					tmpMass[i][j] = SURVIVE; // ���� � ��� 2 ��� 3 ������ - ��� ��������
				else
				{
					tmpMass[i][j] = DIE; // ����� - �������
					cellsChanged++;
				}
			}
			// ���� ������ �������
			else
			{
				if(count == 3)
				{
					tmpMass[i][j] = BORN; // ���� � ��� 3 ������ - ��� �������
					cellsChanged++;
				}
			}
		}
	}
	// �������� ������� ������
	for(i = 0; i<mapSize; i++)
	{
		for(j = 0; j<mapSize; j++)
		{
			if(tmpMass[i][j] == DIE)
				mass[i][j] = 0;
			else if(tmpMass[i][j] == BORN)
				mass[i][j] = 1;
		}
	}
	paintGL(); // �������� ������� ���������
	generationNumber++;
	return cellsChanged; // ���������� ���������� ���������� ������
}

/*! �������, ������� ������ ����� ����� ������� � ������
\* param i - ����� ������ � ������� ��������� ������
\* param j - ����� �������
\* return ceount - ����� �������
*/
int LifeGame::neighbours(int i, int j)
{
	int count = 0, k, m, k1, m1;
	// ���������� ��� ������ ������ ������ ������
	for(k = i-1; k<i+2; k++)
	{
		for(m = j-1; m<j+2; m++)
		{
			if(k!=i || m!=j)
			{
				k1 = k;
				m1 = m; 
				// ���� ����������� ������ ����� �� ������� �����, 
				// �� ��������� ������ �� ������ ������� �����
				if(k<0)
					k1 = mapSize - 1;
				if(m<0)
					m1 = mapSize - 1;
				if(k>mapSize-1)
					k1 = 0;
				if(m>mapSize-1)
					m1 = 0;
				// ���� ������ ����� - ����������� ����� �������
				if(mass[k1][m1]==1)
					count++;
			}
		}
	}
	return count;
}

/*! �������, �����������, ������ �� ���� 
\* return true/false
*/
bool LifeGame::isEmpty()
{
	for(int i=0; i<mapSize; i++)
		for(int j=0; j<mapSize; j++)
			if(mass[i][j]==1)
				return false;
	return true;
}

/*! �������, ���������� ������
*/
void LifeGame::clear()
{
	for(int i=0; i<mapSize; i++)
		for(int j=0; j<mapSize; j++)
			mass[i][j]=0;
	generationNumber = 0;
	paintGL();
}

/*! ������� ���������� �����
*/
void LifeGame::saveMap()
{
	QString filename = QFileDialog::getSaveFileName(this, "�������� ���� ��� ����������", "", "��������(*.bin)");
	QFile file(filename);
	file.open(QIODevice::WriteOnly);
	QDataStream out(&file);
	out << (int)mapSize;
	for(int i = 0; i<mapSize; i++)
		for(int j = 0; j<mapSize; j++)
			out << mass[i][j]; // ���������� � ���� ������
	file.close();
}

/*! ������� �������� �����
*/
void LifeGame::loadMap()
{
	QString filename = QFileDialog::getOpenFileName(this, "�������� ���� ��� ��������", "", "��������(*.bin)");
	if(filename.isEmpty())
		return;
	QFile file(filename);
	file.open(QIODevice::ReadOnly);
	QDataStream in(&file);
	int readed;
	in >> readed;
	mapSize = readed;
	changeMapSize(mapSize);
	for(int i = 0; i<mapSize; i++)
		for(int j = 0; j<mapSize; j++)
			in >> mass[i][j];
	file.close();
	paintGL();
}

/*! ������� ���������� ������� �����
\* param value - ����� ������
*/
void LifeGame::changeMapSize(int value)
{
	mapSize = value;
	clear();
	resizeGL(nWidth, nHeight);
	paintGL();
}

