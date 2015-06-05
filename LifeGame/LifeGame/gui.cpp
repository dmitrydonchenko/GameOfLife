#include "gui.h"
#include <QMessageBox>

gui::gui(QMainWindow *parent)
	: QMainWindow(parent)
{
	GameStarted = false;
	speed = 1000;
	path = QApplication::applicationDirPath();
	ui.setupUi(this);
	// устанавливаем диапазон, шаг и значение ползунка со скоростью
	ui.speed->setRange(-2000, -25);
	ui.speed->setSingleStep(100);
	ui.speed->setValue(-1000);
	// устанавливаем иконки дл€ кнопок
	ui.start->setIcon(QIcon(path + "//Play.png"));
	ui.stop->setIcon(QIcon(path + "//Pause.png"));
	ui.clear->setIcon(QIcon(path + "//Stop.png"));
	// соедин€ем слоты с сигналами
	connect(ui.start, SIGNAL(clicked()), this, SLOT(start()));
	connect(ui.stop, SIGNAL(clicked()), this, SLOT(stop()));
	connect(ui.clear, SIGNAL(clicked()), this, SLOT(clear()));
	connect(ui.speed, SIGNAL(valueChanged(int)), this, SLOT(changeSpeed()));
	connect(ui.SaveMap, SIGNAL(triggered()), this, SLOT(saveMap()));
	connect(ui.LoadMap, SIGNAL(triggered()), this, SLOT(loadMap()));
	connect(ui.NewGame, SIGNAL(triggered()), this, SLOT(clear()));
	connect(ui.changeMapSize, SIGNAL(clicked()), this, SLOT(changeMapSize()));
	// создаем таймер
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(go())); 
	ui.stop->setEnabled(false);
}

gui::~gui()
{
	delete timer;
}

/*! слот, запускающий игру
*/
void gui::start()
{
	GameStarted = true;
	ui.widget->setEnabled(false);
	timer->start(speed);
	ui.start->setEnabled(false);
	ui.stop->setEnabled(true);
	ui.clear->setEnabled(true);
	ui.changeMapSize->setEnabled(false);
}

/*! слот, останавливающий игру
*/
void gui::stop()
{
	GameStarted = false;
	ui.stop->setEnabled(false);
	ui.start->setEnabled(true);
	ui.widget->setEnabled(true);
}

/*! слот, вызывающий по таймеру функцию получени€ нового поколени€ клеток
*/
void gui::go()
{
	if(GameStarted)
	{
		bool flag = ui.widget->nextGeneration();
		ui.generationNumber->setValue(ui.widget->generationNumber);
		if(!flag)
		{
			GameStarted = false;
			ui.start->setEnabled(true);
			ui.stop->setEnabled(false);
			QString messageText;
			// проверка на конец игры:
			if(ui.widget->isEmpty())
				messageText = "¬се клетки вымерли";
			else
				messageText = "ƒостигнута устойчива€ конфигураци€";
			int over = QMessageBox::information(this, "»гра закончена", messageText, QMessageBox::Ok);
			ui.widget->setEnabled(true);
		}
		timer->start(speed);
	}
}

/*! слот, очищающий карту
*/
void gui::clear()
{
	GameStarted = false;
	ui.widget->clear();
	ui.generationNumber->setValue(ui.widget->generationNumber);
	ui.widget->setEnabled(true);
	ui.start->setEnabled(true);
	ui.stop->setEnabled(false);
	ui.changeMapSize->setEnabled(true);
}

/*! слот изменени€ скорости
*/
void gui::changeSpeed()
{
	speed = - (ui.speed->value());
}

/*! слот сохранени€
*/
void gui::saveMap()
{
	ui.widget->saveMap();
}

/*! слот загрузки
*/
void gui::loadMap()
{
	ui.widget->loadMap();
}

/*! слот изменени€ размера карты
*/
void gui::changeMapSize()
{
	ui.widget->changeMapSize(ui.mapSize->value());
}
