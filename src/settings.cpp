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
	emit get_other_users_info_signal();
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

void settings::get_other_user_info_response_slot(string response)
{
	switch(stoi(response.substr(0,1)))
	{
		case RESPONSE_SUCCESS:
			render_other_user_details(response.substr(1));
			break;

		case RESPONSE_FAILED:
			break;

		default:
			break;
	}
}

void settings::render_other_user_details(string userInfo)
{
	QString user_info = QString::fromStdString(userInfo);
	QJsonDocument jsonDocument = QJsonDocument::fromJson(user_info.toUtf8());
	QJsonObject jsonObject = jsonDocument.object();
	QJsonValue ArrayValue;
	int is_su = jsonObject.value("request_user_type").toInt();
	ArrayValue = jsonObject.value("userInfo");
	QJsonArray user_array = ArrayValue.toArray();

	if(!other_user_info_list.isEmpty())
		other_user_info_list.clear();
	clearLayout(ui->other_user_vLayout);

	foreach(const QJsonValue user, user_array)
	{
		struct other_user_info_node *other_user_nd = new struct other_user_info_node;

		other_user_nd->u_id = user.toObject().value("user_id").toInt();
		other_user_nd->first_name = user.toObject().value("first_name").toString();
		other_user_nd->last_name = user.toObject().value("last_name").toString();
		other_user_nd->email = user.toObject().value("email").toString();

		// generate ui
		horizontalLayout = new QHBoxLayout();
		horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
		verticalLayout = new QVBoxLayout();
		verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
		other_user_name_label = new QLabel();
		other_user_name_label->setObjectName(QString::fromUtf8("other_user_name_label"));

		if(other_user_nd->last_name == "")
			other_user_name_label->setText("<font color=#0000FF>"+other_user_nd->first_name+"</font>");
		else
			other_user_name_label->setText("<font color=#0000FF>"+other_user_nd->first_name+" "+other_user_nd->last_name+"</font>");

		verticalLayout->addWidget(other_user_name_label);

		other_user_email_label = new QLabel();
		other_user_email_label->setObjectName(QString::fromUtf8("other_user_email_label"));
		other_user_email_label->setText("<font color=#000000>"+other_user_nd->email+"</font>");

		verticalLayout->addWidget(other_user_email_label);
		horizontalLayout->addLayout(verticalLayout);

		horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

		horizontalLayout->addItem(horizontalSpacer);

		btnHorizontalLayout = new QHBoxLayout();
		btnHorizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

		other_user_login_button = new QPushButton(other_user_nd->email);
		other_user_login_button->setObjectName(QString::fromUtf8("other_user_login_button"));

		signalMapper = new QSignalMapper(this);
		signalMapper->setMapping(other_user_login_button, other_user_nd->email);

		if(is_su)
			other_user_login_button->setText("Login");
		else
		{
			if(user.toObject().value("su").toInt() == 1)
				other_user_login_button->setText("Super User Login");
			else
				other_user_login_button->setText("Login");
		}

		btnHorizontalLayout->addWidget(other_user_login_button);

		if(is_su)
		{
			other_user_remove_button = new QPushButton();
			other_user_remove_button->setObjectName(QString::fromUtf8("other_user_remove_button"));
			other_user_remove_button->setText("Remove");

			btnHorizontalLayout->addWidget(other_user_remove_button);
		}

		connect(other_user_login_button, SIGNAL(clicked()), signalMapper, SLOT(map()));
		connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(other_user_login_button_clicked_slot(QString)));

		horizontalLayout->addLayout(btnHorizontalLayout);
		ui->other_user_vLayout->addLayout(horizontalLayout);
		other_user_info_list.append(other_user_nd);
	}

	verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	ui->other_user_vLayout->addItem(verticalSpacer);

}

void settings::other_user_login_button_clicked_slot(QString email)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	emit dashboard_request_other_user_login_signal(email);
	cout << "Closing settings..." << endl;
	this->close();
}

void settings::clearLayout(QLayout* layout)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;

	while (QLayoutItem* item = layout->takeAt(0))
	{
		if (QWidget* widget = item->widget())
			widget->deleteLater();
		if (QLayout* childLayout = item->layout())
			clearLayout(childLayout);
		delete item;
	}
}

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
		{
			if(password != "")
			{
				ui->passwordLineEdit->setEnabled(false);
				ui->passwordUpdatePushButton->setEnabled(false);
				ui->updatePasswordStatus->setText("<font color=#0000FF>Verifying... </font>");
				emit verify_password_signal(password.toStdString());
			}
			else
				ui->updatePasswordStatus->setText("<font color=#FF0000>Can't be blank </font>");
		}
		break;

		case READY_TO_SET_NEW:
		{
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
			else
				ui->updatePasswordStatus->setText("<font color=#FF0000>Can't be blank </font>");
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
