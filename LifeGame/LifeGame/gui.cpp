#include "gui.h"
#include <QMessageBox>

gui::gui(QMainWindow *parent)
	: QMainWindow(parent)
{
	GameStarted = false;
	speed = 1000;
	path = QApplication::applicationDirPath();
	ui.setupUi(this);
	// ������������� ��������, ��� � �������� �������� �� ���������
	ui.speed->setRange(-2000, -25);
	ui.speed->setSingleStep(100);
	ui.speed->setValue(-1000);
	// ������������� ������ ��� ������
	ui.start->setIcon(QIcon(path + "//Play.png"));
	ui.stop->setIcon(QIcon(path + "//Pause.png"));
	ui.clear->setIcon(QIcon(path + "//Stop.png"));
	// ��������� ����� � ���������
	connect(ui.start, SIGNAL(clicked()), this, SLOT(start()));
	connect(ui.stop, SIGNAL(clicked()), this, SLOT(stop()));
	connect(ui.clear, SIGNAL(clicked()), this, SLOT(clear()));
	connect(ui.speed, SIGNAL(valueChanged(int)), this, SLOT(changeSpeed()));
	connect(ui.SaveMap, SIGNAL(triggered()), this, SLOT(saveMap()));
	connect(ui.LoadMap, SIGNAL(triggered()), this, SLOT(loadMap()));
	connect(ui.NewGame, SIGNAL(triggered()), this, SLOT(clear()));
	connect(ui.changeMapSize, SIGNAL(clicked()), this, SLOT(changeMapSize()));
	// ������� ������
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(go())); 
	ui.stop->setEnabled(false);
}

gui::~gui()
{
	delete timer;
}

/*! ����, ����������� ����
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

/*! ����, ��������������� ����
*/
void gui::stop()
{
	GameStarted = false;
	ui.stop->setEnabled(false);
	ui.start->setEnabled(true);
	ui.widget->setEnabled(true);
}

/*! ����, ���������� �� ������� ������� ��������� ������ ��������� ������
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
			// �������� �� ����� ����:
			if(ui.widget->isEmpty())
				messageText = "��� ������ �������";
			else
				messageText = "���������� ���������� ������������";
			int over = QMessageBox::information(this, "���� ���������", messageText, QMessageBox::Ok);
			ui.widget->setEnabled(true);
		}
		timer->start(speed);
	}
}

/*! ����, ��������� �����
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

/*! ���� ��������� ��������
*/
void gui::changeSpeed()
{
	speed = - (ui.speed->value());
}

/*! ���� ����������
*/
void gui::saveMap()
{
	ui.widget->saveMap();
}

/*! ���� ��������
*/
void gui::loadMap()
{
	ui.widget->loadMap();
}

/*! ���� ��������� ������� �����
*/
void gui::changeMapSize()
{
	ui.widget->changeMapSize(ui.mapSize->value());
}
