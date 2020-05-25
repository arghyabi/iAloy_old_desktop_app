#include<iostream>
#include <QApplication>
#include <QFontDatabase>
#include <QProxyStyle>
#include <QStyleOptionTab>

#include "settings.h"
#include "ui_settings.h"

using namespace std;
//https://stackoverflow.com/questions/51892400/qt-use-palette-color-in-stylesheet

class CustomTabStyle : public QProxyStyle {
public:
	QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget)
	const
	{
		QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
		if (type == QStyle::CT_TabBarTab)
			s.transpose();
		return s;
	}

	void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget)
	const
	{
		if (element == CE_TabBarTabLabel)
		{
			if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option))
			{
				QStyleOptionTab opt(*tab);
				opt.shape = QTabBar::RoundedNorth;
				QProxyStyle::drawControl(element, &opt, painter, widget);
				return;
			}
		}
		QProxyStyle::drawControl(element, option, painter, widget);
	}
};

settings::settings(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::settings)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	ui->setupUi(this);
	ui->tabWidget->tabBar()->setStyle(new CustomTabStyle);
	ui->passwordLineEdit->hide();
	ui->updatePasswordStatus->setText("");
	updatePasswordState_flag = INITIALIZED;

	password_matched = -1;

	render_update_password();
}

settings::~settings()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	delete ui;
}

void settings::init(int mode)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;

	ui->confirmPasswordLineEdit->hide();
	ui->updateCancelpushButton->hide();

	// int font_size[] = {8,9,10,11,12,14,16,18,20};

	// system("basename -a -s .ttf /usr/share/iAloy/fonts/*.ttf > font.txt");

	// FILE * fp;
	// char * line = NULL;
	// size_t len = 0;
	// ssize_t read;

	// fp = fopen("font.txt", "r");
	// if (fp != NULL)
	// {
	// 	while ((read = getline(&line, &len, fp)) != -1)
	// 	{
	// 		line[strlen(line)-1] = '\0';
	// 		//ui->font_name_select_combobox->addItem(QString::fromUtf8(line));
	// 	}

	// 	fclose(fp);
	// 	if (line)
	// 		free(line);
	// }

	// system("rm font.txt");

	// for(int i = 0; i < sizeof(font_size)/sizeof(int); i++)
	// {
	// 	//ui->font_size_select_combobox->addItem(QString::number(font_size[i]));
	// }
	this->show();
}

// void settings::set_font_name_and_size()
// {
// 	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
// // 	QString fname = ui->font_name_select_combobox->currentText();
// // 	QString fsize = ui->font_size_select_combobox->currentText();

// // 	QFontDatabase::addApplicationFont("/usr/share/iAloy/fonts/" + fname + ".ttf");
// // 	QApplication::setFont(QFont(fname, fsize.toInt(), QFont::Normal, false));
// }

// void settings::on_buttonBox_accepted()
// {
// 	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
// 	set_font_name_and_size();
// }

// void settings::on_pushButton_clicked()
// {
// 	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
// 	set_font_name_and_size();
// }

void settings::render_update_password()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	switch(updatePasswordState_flag)
	{
		case INITIALIZED:
		{
			ui->passwordLineEdit->hide();
			ui->confirmPasswordLineEdit->hide();
			ui->updateCancelpushButton->hide();

			ui->passwordLineEdit->setEnabled(true);
			ui->passwordUpdatePushButton->setEnabled(true);
			ui->confirmPasswordLineEdit->setEnabled(true);
			ui->updateCancelpushButton->setEnabled(true);

			ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
			ui->passwordLineEdit->setText("");
			ui->passwordUpdatePushButton->setText("Update");

			if(password_matched == 1)
				ui->updatePasswordStatus->setText("<font color=#00FF00>Updated successfully</font>");

			break;
		}
		case READY_TO_VERIFY:
		{
			ui->passwordLineEdit->show();
			ui->passwordLineEdit->setPlaceholderText("Current Password");
			ui->passwordLineEdit->setText("");
			ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
			ui->passwordUpdatePushButton->setText("Verify");
			ui->passwordLineEdit->setEnabled(true);
			ui->passwordUpdatePushButton->setEnabled(true);

			if(password_matched != 0)
				ui->updatePasswordStatus->setText("");
			else
				ui->updatePasswordStatus->setText("<font color=#FF0000>Not matched</font>");

			break;
		}
		case READY_TO_SET_NEW:
		{
			ui->passwordLineEdit->setPlaceholderText("New Password");
			ui->confirmPasswordLineEdit->show();
			ui->confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
			ui->confirmPasswordLineEdit->setPlaceholderText("Confirm Password");
			ui->updateCancelpushButton->show();
			ui->passwordUpdatePushButton->setText("Set");
			ui->confirmPasswordLineEdit->setText("");
			ui->passwordLineEdit->setText("");
			ui->passwordLineEdit->setEnabled(true);
			ui->passwordUpdatePushButton->setEnabled(true);

			if(password_matched != 0)
				ui->updatePasswordStatus->setText("");
			else
				ui->updatePasswordStatus->setText("<font color=#FF0000>Update failed</font>");

			break;
		}
	}
}

void settings::on_passwordUpdatePushButton_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;

	QString password = ui->passwordLineEdit->text();
	QString confirm_password = ui->confirmPasswordLineEdit->text();

	switch(updatePasswordState_flag)
	{
		case INITIALIZED:
		{
			updatePasswordState_flag = READY_TO_VERIFY;
			render_update_password();
		}
		break;
		case READY_TO_VERIFY:
		if(password != "")
		{
			ui->passwordLineEdit->setEnabled(false);
			ui->passwordUpdatePushButton->setEnabled(false);
			ui->updatePasswordStatus->setText("<font color=#0000FF>Verifying... </font>");
			emit verify_password_signal(password.toStdString());
		}
		break;
		case READY_TO_SET_NEW:
		if(password != "" && confirm_password != "")
		{
			if(password == confirm_password)
			{
				password_matched = 1;
				ui->passwordLineEdit->setEnabled(false);
				ui->passwordUpdatePushButton->setEnabled(false);
				ui->confirmPasswordLineEdit->setEnabled(false);
				ui->updateCancelpushButton->setEnabled(false);
				ui->updatePasswordStatus->setText("<font color=#0000FF>Updating... </font>");
				emit update_password_signal(password.toStdString());
			}
			else
			{
				password_matched = 0;
				ui->confirmPasswordLineEdit->setText("");
				ui->updatePasswordStatus->setText("<font color=#FF0000>Mismatched</font>");
			}
		}
		break;

	}
}

void settings::verify_password_response_slot(bool resp)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(resp)
	{
		password_matched = 1;
		updatePasswordState_flag = READY_TO_SET_NEW;
	}
	else
	{
		password_matched = 0;
		updatePasswordState_flag = READY_TO_VERIFY;
	}
	render_update_password();
}

void settings::update_password_response_slot(bool resp)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(resp)
	{
		password_matched = 1;
		updatePasswordState_flag = INITIALIZED;
	}
	else
	{
		password_matched = 0;
		updatePasswordState_flag = READY_TO_SET_NEW;
	}
	render_update_password();
}

void settings::on_updateCancelpushButton_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	updatePasswordState_flag = INITIALIZED;
	render_update_password();
}

void settings::on_pushButton_10_clicked()
{
	this->close();
}
