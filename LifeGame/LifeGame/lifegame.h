#ifndef LIFEGAME_H
#define LIFEGAME_H

#include <QtGui/QMainWindow>
#include <QGLWidget>

class LifeGame : public QGLWidget
{
	Q_OBJECT

public:
	 LifeGame(QWidget* parent = 0);
      ~LifeGame();
	  
	  
	  bool nextGeneration();
	  int neighbours(int i, int j);
	  bool isEmpty();
	  void clear();
	  void saveMap();
	  void loadMap();
	  void changeMapSize(int value);
	  int generationNumber;
	  	  	  
private:
	QVector< QVector<int> > mass;
	GLfloat mapSize, nWidth, nHeight, cellWidth, cellHeight;
protected:
	void initializeGL();           
    void resizeGL(int nWidth, int nHeight); 
    void paintGL();  
    void mousePressEvent(QMouseEvent* pe);
};

#endif // LIFEGAME_H
