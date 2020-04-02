#include <QtWidgets/QFrame>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>

#include "dashboard.h"
#include "ui_dashboard.h"

dashboard::dashboard(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dashboard)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	ui->setupUi(this);

	for (int i = 0; i < 10; ++i){

		QPushButton *pushButton_d;
		QVBoxLayout *verticalLayout_d;
		QFrame *frame_d;
		QSlider *slider_d;

		frame_d = new QFrame();
		frame_d->setObjectName(QString::fromUtf8("frame_d"));
		frame_d->setFrameShape(QFrame::StyledPanel);
		frame_d->setFrameShadow(QFrame::Raised);

		verticalLayout_d = new QVBoxLayout(frame_d);
		pushButton_d = new QPushButton(frame_d);
		slider_d = new QSlider(Qt::Orientation::Horizontal,frame_d);

		verticalLayout_d->addWidget(pushButton_d);
		verticalLayout_d->addWidget(slider_d);

		verticalLayout_d->setObjectName(QString::fromUtf8("verticalLayout_d"));
		pushButton_d->setObjectName(QString::fromUtf8("pushButton_d"));
		slider_d->setObjectName(QString::fromUtf8("slider_d"));

		pushButton_d->setText(QString::number(i));

		ui->gridLayout_2->addWidget(frame_d,1,i+2);
	}

}

dashboard::~dashboard()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	delete ui;
}

void dashboard::addBgImage()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	QPixmap bkgnd(QString::fromStdString(dashboard::get_dashboard_bg_file_path()));
	bkgnd = bkgnd.scaled(this->width(),this->height());
	QPalette palette;
	palette.setBrush(QPalette::Background, bkgnd);
	this->setPalette(palette);
}
