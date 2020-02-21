#include "dashboard.h"
#include "ui_dashboard.h"

dashboard::dashboard(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dashboard)
{
	ui->setupUi(this);
}

dashboard::~dashboard()
{
	delete ui;
}

void dashboard::addBgImage()
{
	QString pic_add = "/usr/share/iAloy/back.jpg";
	QPixmap bkgnd(pic_add);
	bkgnd = bkgnd.scaled(this->width(),this->height());
	QPalette palette;
	palette.setBrush(QPalette::Background, bkgnd);
	this->setPalette(palette);
}
