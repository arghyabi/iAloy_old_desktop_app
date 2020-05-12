#include "module_manager.h"
#include "ui_module_manager.h"
#include <QThread>

module_manager::module_manager(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::module_manager)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	ui->setupUi(this);
	ui->refresh_tool_button->setIcon(QIcon(QString::fromStdString(module_manager::get_refresh_icon_path())));
	ui->usb_refresh_tool_button->setIcon(QIcon(QString::fromStdString(module_manager::get_refresh_icon_path())));
	ui->progressBar->setValue(0);

	i2c_data *i2c_thread_obj = new i2c_data(this);
	connect(this, SIGNAL(read_all_i2c_module_state_signal(int*)), i2c_thread_obj, SLOT(read_all_i2c_module_state(int*)));

	module_manager_thread *mm_thread_obj = new module_manager_thread(this);

	connect(this, SIGNAL(burn_module_signal(int)), mm_thread_obj, SLOT(burn_module_slot(int)));
	connect(mm_thread_obj, SIGNAL(start_burning_signal()), this, SLOT(start_burning_slot()));
	connect(mm_thread_obj, SIGNAL(console_print_signal(QString)),this,SLOT(burning_module_slot(QString)));
	connect(mm_thread_obj, SIGNAL(burn_complete_signal()), this, SLOT(burn_complete_slot()));
}

module_manager::~module_manager()
{
	delete ui;
}

void module_manager::init(QLinkedList<btn_node*> tmp_btn_list)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->show();
	ui->tabWidget->setTabEnabled(2,false);

	int col = 0;
	int row = 0;
	int module_status_array[117];
	int module_active_count = 0;
	int module_available_count = 0;
	int module_disconnected_count = 0;
	int module_not_available_count = 0;

	QString head_style = "padding-top: 6px; padding-bottom: 6px; padding-left: 6px; padding-right: 6px; text-align: center; background-color: #005DD5; color: #FFF;";
	QString body_style = "border: 1px solid #ddd; padding: 4px; text-align: center;";

	QString table_start = "<table style='border-collapse: collapse width: 100%; margin: 10px; margin-right: 30px;' border=1> <tr> <th style='" + head_style + "'>Device</th> <th style='" + head_style + "'>Pin</th> <th style='" + head_style + "'>Status</th> <th style='" + head_style + "'>Slider</th> </tr>";
	QString body_start = "<tr><td><center>";
	QString body_end = "</center></td></tr>";
	QString table_end = "</table>";
	QString body_joint = "</center></td><td><center>";

	emit read_all_i2c_module_state_signal(module_status_array);

	for (int index = 0; index < 117; index++)
	{
		int mod_add = index + 3;

		struct btn_node *btn_nd;
		bool isFound = false;
		QString module_info;
		QString room_name_array[8];

		int room_count = 0;
		QString mod_add_str;
		if(module_status_array[index] == 1)
		{
			mod_add_str = "0" + QString::number(mod_add);
			add_btn = new QPushButton(mod_add_str);
			add_btn->setStyleSheet(QString::fromUtf8("padding: 5%;background-color: #525252; color: #FFF;"));
		}
		else if(module_status_array[index] == 0)
		{
			mod_add_str = "1" + QString::number(mod_add);
			add_btn = new QPushButton(mod_add_str);
			add_btn->setStyleSheet(QString::fromUtf8("padding: 5%;background-color: #00FF00; color: #000;"));
		}
		add_btn->setText(QString::fromStdString(int_to_hex(mod_add)));
		ui->address_list_grid_layout->addWidget(add_btn, row, col, 1,1);
		cout << "Button added : " << int_to_hex(mod_add) << endl;

		signalMapper = new QSignalMapper(this);
		signalMapper->setMapping(add_btn, mod_add_str);

		connect(add_btn, SIGNAL(clicked()), signalMapper, SLOT(map()));
		connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(button_clicked_slot(QString)));

		if(col == 10)
		{
			col = 0;
			row++;
		}
		else
			col++;
	}
}

void module_manager::button_clicked_slot(QString mod_add)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	ui->progressBar->setValue(0);
	QString btn_state = mod_add.mid(0,1);
	mod_add = mod_add.mid(1, -1);
	selected_mod_address = mod_add.toInt();
	QString hex_mod_add = QString::fromStdString(int_to_hex(selected_mod_address));
	ui->burn_console->setText("");
	console_print("Selected mod address : " + hex_mod_add);

	if(btn_state == "0")
	{
		QMessageBox msgBox;
		msgBox.setText("<font>" + hex_mod_add + " is not available to burn.</font>");
		msgBox.exec();
	}
	else
	{
		QMessageBox::StandardButton confirmBox;
		confirmBox = QMessageBox::question(this, "Confirm", "Do you want to burn " + hex_mod_add + "?", QMessageBox::Yes|QMessageBox::No);
		if (confirmBox == QMessageBox::Yes)
		{
			ui->tabWidget->setCurrentIndex(2);
			ui->burn_status_label->setText("<font color=#3465A4>Selected address : </font><font color=#3465A4 size=4><b>" + hex_mod_add + "&nbsp;&nbsp;</b></font>");
			ui->overall_module_status->setText("Selected mod address : " + hex_mod_add);
			ui->tabWidget->setTabEnabled(2,true);
		}
	}
}

void module_manager::on_burn_button_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	emit burn_module_signal(selected_mod_address);
}

void module_manager::start_burning_slot()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	ui->burn_button->setEnabled(false);
	console_print("Burning...");
}

void module_manager::burning_module_slot(QString data)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	console_print(data);
}

void module_manager::burn_complete_slot()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;

	QFile file;
	file.setFileName("/usr/share/iAloy/.temp/avrdude.log");

	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QString log = file.readAll();
	file.close();


	ui->burn_button->setEnabled(true);
	ui->progressBar->setValue(100);
	console_print("Burn Completed");
	console_print(log);
}

void module_manager::console_print(QString data)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	QString curr_data = ui->burn_console->toPlainText();
	if(curr_data != "")
		ui->burn_console->setText(curr_data + "\n" + data);
	else
		ui->burn_console->setText(data);
}