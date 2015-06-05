#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QMainWindow>
#include "ui_gui.h"
#include "LifeGame.h"
#include <QTimer>
#include <QSlider>

class gui : public QMainWindow
{
	Q_OBJECT

public:
	gui(QMainWindow *parent = 0);
	~gui();
	QTimer *timer;
	QAbstractSlider *slider;
	
private:
	Ui::gui ui;
	
	bool GameStarted;
	int speed;
	QString path;

private slots:
	void start();
	void go();
	void stop();
	void clear();
	void changeSpeed();
	void saveMap();
	void loadMap();
	void changeMapSize();
};

#endif // GUI_H
