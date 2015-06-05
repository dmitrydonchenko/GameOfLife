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

/*! функция, которая при нажатии мышкой по клетке меняет значение соответствующего элемента
массива с 0 на 1 или наоборот
*\param pe - событие мыши
*/
void LifeGame::mousePressEvent(QMouseEvent *pe)
{
	int x = pe->x(), y = pe->y(); // координаты точки, в которой был курсор
	if(pe->button()==Qt::LeftButton) // если нажата левая кнопка
	{
		int j = x / cellWidth, i = y / cellHeight; // узнаем в какой клетке был курсор
		if(j >= mapSize) j = mapSize - 1; 
			if(i >= mapSize) i = mapSize - 1;
			//изменяем значение элемента массива, соответствующего данной клетке:
				mass[i][j] = (mass[i][j] == 0)? 1 : 0; 
	}
	paintGL(); // вызываем функцию отрисовки
}

/*! функция, отвечающая за корректное изменение размера виджета OpenGL
*\param w - ширина, h - высота
*/
void LifeGame::resizeGL(int w, int h)
{
	nWidth = w;
	nHeight = h;
	glViewport(0.0f, 0.0f, w, h); // устанавливает поле просмотра с размерами окна
	glMatrixMode(GL_PROJECTION); // обновляет систему координат
	glLoadIdentity();
	glOrtho(0.0f, w, h, 0.0f, -5.0f, 5.0f); // модифицируем границы объема отсечения
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// меняем ширину и высоту клетки:
	cellWidth = (GLfloat)w/(GLfloat)mapSize; 
	cellHeight = (GLfloat)h/(GLfloat)mapSize;
}

/*! функция инициализации
*/
void LifeGame::initializeGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

/*! функция отрисовки
*/
void LifeGame::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT); // очищаем экран текущим цветом
	// закрашиваем клетки белым или черным цветом в зависимости от 
	// значения соответствующих элементов массива
	for(int i = 0; i < mapSize; i++)
	{
		for(int j = 0; j < mapSize; j++)
		{
			if(!mass[i][j])
				glColor3f(1.0f, 1.0f, 1.0f); // если элемент массива = 0, то цвет - белый
			else if(mass[i][j])
				glColor3f(0.0f, 0.0f, 0.0f); // иначе - черный
			// закрашиваем клетку
			glRectf((GLfloat)j * cellWidth, (GLfloat)i * cellHeight, (GLfloat)(j + 1) * cellWidth, (GLfloat)(i + 1) * cellHeight);
		}
	}
	glColor3f(0.75f, 0.75f, 0.75f); // устанавливаем серый цвет
	// рисуем сетку:
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

/*! функция, получающая следующее поколение клеток
\* return cellsChanged - количество измененных клеток
*/
bool LifeGame::nextGeneration()
{
	int count, i, j, cellsChanged = 0;
	QVector< QVector<int> > tmpMass(mapSize, QVector<int>(mapSize, 0)); // вспомогательный массив
	// перебираем все клетки
	for(i = 0; i<mapSize; i++)
	{
		for(j = 0; j<mapSize; j++)
		{
			count = neighbours(i, j); // узнаем число живых соседей для клетки
			// если клетка живая:
			if(mass[i][j] == 1)
			{
				if(count == 2 || count == 3)
					tmpMass[i][j] = SURVIVE; // если у нее 2 или 3 соседа - она выживает
				else
				{
					tmpMass[i][j] = DIE; // иначе - умирает
					cellsChanged++;
				}
			}
			// если клетка неживая
			else
			{
				if(count == 3)
				{
					tmpMass[i][j] = BORN; // если у нее 3 соседа - она оживает
					cellsChanged++;
				}
			}
		}
	}
	// изменяем главный массив
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
	paintGL(); // вызываем функцию отрисовки
	generationNumber++;
	return cellsChanged; // возвращаем количество измененных клеток
}

/*! функция, которая узнает число живых соседей у клетки
\* param i - номер строки в которой находится клетка
\* param j - номер столбца
\* return ceount - число соседей
*/
int LifeGame::neighbours(int i, int j)
{
	int count = 0, k, m, k1, m1;
	// перебираем все клетки вокруг данной клетки
	for(k = i-1; k<i+2; k++)
	{
		for(m = j-1; m<j+2; m++)
		{
			if(k!=i || m!=j)
			{
				k1 = k;
				m1 = m; 
				// если проверяемая клетка вышла за пределы карты, 
				// то проверяем клетку на другой стороне карты
				if(k<0)
					k1 = mapSize - 1;
				if(m<0)
					m1 = mapSize - 1;
				if(k>mapSize-1)
					k1 = 0;
				if(m>mapSize-1)
					m1 = 0;
				// если клетка живая - увеличиваем число соседей
				if(mass[k1][m1]==1)
					count++;
			}
		}
	}
	return count;
}

/*! функция, проверяющая, пустое ли поле 
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

/*! функция, обнуляющая массив
*/
void LifeGame::clear()
{
	for(int i=0; i<mapSize; i++)
		for(int j=0; j<mapSize; j++)
			mass[i][j]=0;
	generationNumber = 0;
	paintGL();
}

/*! функция сохранения карты
*/
void LifeGame::saveMap()
{
	QString filename = QFileDialog::getSaveFileName(this, "Выберите файл для сохранения", "", "Бинарный(*.bin)");
	QFile file(filename);
	file.open(QIODevice::WriteOnly);
	QDataStream out(&file);
	out << (int)mapSize;
	for(int i = 0; i<mapSize; i++)
		for(int j = 0; j<mapSize; j++)
			out << mass[i][j]; // записываем в файл массив
	file.close();
}

/*! функция загрузки карты
*/
void LifeGame::loadMap()
{
	QString filename = QFileDialog::getOpenFileName(this, "Выберите файл для загрузки", "", "Бинарный(*.bin)");
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

/*! функция изменяющая размеры карты
\* param value - новый размер
*/
void LifeGame::changeMapSize(int value)
{
	mapSize = value;
	clear();
	resizeGL(nWidth, nHeight);
	paintGL();
}

