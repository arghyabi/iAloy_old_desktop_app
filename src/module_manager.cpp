#include "module_manager.h"
#include "ui_module_manager.h"
#include <QThread>

module_manager::module_manager(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::module_manager)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;

	module_index = 0;
	module_type_flag = ATTINY88;
	ui->setupUi(this);
	ui->refresh_tool_button->setIcon(QIcon(QString::fromStdString(module_manager::get_refresh_icon_path())));
	ui->progressBar->setValue(0);

	i2c_data *i2c_thread_obj = new i2c_data(this);
	connect(this, SIGNAL(read_all_i2c_module_state_signal(int*)), i2c_thread_obj, SLOT(read_all_i2c_module_state(int*)));

	module_manager_thread *mm_thread_obj = new module_manager_thread(this);

	connect(this, SIGNAL(burn_module_signal(int, int)), mm_thread_obj, SLOT(burn_module_slot(int, int)));
	connect(mm_thread_obj, SIGNAL(start_burning_signal()), this, SLOT(start_burning_slot()));
	connect(mm_thread_obj, SIGNAL(console_print_signal(QString)),this,SLOT(burning_module_slot(QString)));
	connect(mm_thread_obj, SIGNAL(burn_complete_signal()), this, SLOT(burn_complete_slot()));

}
void module_manager::add_new_module_api_resp_slot(string resp)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(resp == "1" || resp == "2")
		link_mod_add_array[module_index][1] = MODULE_LINKING_DONE;
	else
		link_mod_add_array[module_index][1] = MODULE_LINKING_FAILED;

	render_link_button_list();

	if(link_mod_add_array[module_index][1] == MODULE_LINKING_DONE)
		emit new_module_linked_signal();
}

module_manager::~module_manager()
{
	delete ui;
}

void module_manager::init(QLinkedList<btn_node*> tmp_btn_list, int tab_index)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	burn_btn_func = 0;
	ui->overall_module_status->setText("");
	int module_status_array[117];
	emit read_all_i2c_module_state_signal(module_status_array);
	ui->tabWidget->setTabEnabled(2,false);

	//////////////////////////////////////////////////////////////

	link_button_array_length = 0;
	for(int index = 0; index < 117; index++)
	{
		int mod_add = index + 3;
		struct btn_node *btn_nd;
		bool isFound = false;

		foreach(btn_nd, tmp_btn_list)
		{
			if(btn_nd->mod_add == mod_add)
			{
				isFound = true;
				break;
			}
		}

		if(!isFound && module_status_array[index] == 1)
		{
			link_mod_add_array[link_button_array_length][0] = mod_add;
			link_mod_add_array[link_button_array_length++][1] = MODULE_NEED_TO_BE_LINKED;
		}
	}
	render_link_button_list();

	//////////////////////////////////////////////////////////////
	int col = 0;
	int row = 0;
	int module_active_count = 0;
	int module_available_count = 0;
	int module_disconnected_count = 0;
	int module_not_available_count = 0;

	QString head_style = "padding-top: 6px; padding-bottom: 6px; padding-left: 6px; padding-right: 6px; text-align: center; background-color: #005DD5; color: #FFF;";
	QString body_style = "border: 1px solid #ddd; padding: 4px; text-align: center;";

	QString table_start = "<table style='border-collapse: collapse width: 100%; margin: 10px; margin-right: 30px;' border=1> <tr> <th style='" + head_style + "'>Device</th> <th style='" + head_style + "'>Pin</th> <th style='" + head_style + "'>Status</th> <th style='" + head_style + "'>Slider</th> </tr>";
	QString body_start = "<tr><td><center>";

	QString body_joint = "</center></td><td><center>";

	QString body_end = "</center></td></tr>";
	QString table_end = "</table>";

	for (int index = 0; index < 117; index++)
	{
		int mod_add = index + 3;
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

	ui->burn_button->setText("Burn");
	ui->tabWidget->setCurrentIndex(tab_index);
	this->show();
}

void module_manager::render_link_button_list()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	clearLayout(ui->vLayout_linking_area);
	scrollAreaWidgetContents = new QWidget();

	for(int count = 0; count < link_button_array_length; count++)
	{
		hLayoutLinkModule = new QHBoxLayout();
		CountLabel = new QLabel(scrollAreaWidgetContents);
		ModAddLabel = new QLabel(scrollAreaWidgetContents);
		hSpacerLinkModuleMid = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		hSpacerLinkModuleLeft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		hSpacerLinkModuleRight = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

		QString mod_add_str = QString::fromStdString(int_to_hex(link_mod_add_array[count][0]));
		CountLabel->setText("<font style=\"font-size:14pt; font-weight:600; color:#3465a4;\">"+QString::number(count+1)+" : </font>");

		hLayoutLinkModule->addItem(hSpacerLinkModuleLeft);
		hLayoutLinkModule->addWidget(CountLabel);
		ModAddLabel->setText(mod_add_str);
		hLayoutLinkModule->addWidget(ModAddLabel);
		hLayoutLinkModule->addItem(hSpacerLinkModuleMid);

		if(link_mod_add_array[count][1] == MODULE_NEED_TO_BE_LINKED)
		{
			LinkButton = new QPushButton(mod_add_str);
			linkSignalMapper = new QSignalMapper(this);

			LinkButton->setStyleSheet("background-color: rgb(52, 101, 164); color: rgb(255, 255, 255)");
			LinkButton->setText("Link");
			linkSignalMapper->setMapping(LinkButton, mod_add_str);

			connect(LinkButton, SIGNAL(clicked()), linkSignalMapper, SLOT(map()));
			connect(linkSignalMapper, SIGNAL(mapped(QString)), this, SLOT(link_button_clicked_slot(QString)));

			hLayoutLinkModule->addWidget(LinkButton);
		}
		else if(link_mod_add_array[count][1] == MODULE_LINKING_PROGRESS)
		{
			linkingLabel = new QLabel(scrollAreaWidgetContents);
			linkingLabel->setText("<font style=\"font-size:11pt; font-weight:600; color:#3465a4;\"> Linking...</font>");

			hLayoutLinkModule->addWidget(linkingLabel);
		}
		else if(link_mod_add_array[count][1] == MODULE_LINKING_DONE)
		{
			linkingLabel = new QLabel(scrollAreaWidgetContents);
			linkingLabel->setText("<font style=\"font-size:11pt; font-weight:600; color:#3465a4;\"> Done </font>");

			hLayoutLinkModule->addWidget(linkingLabel);
		}
		else
		{
			linkingLabel = new QLabel(scrollAreaWidgetContents);
			LinkButton = new QPushButton(mod_add_str);
			linkSignalMapper = new QSignalMapper(this);

			linkingLabel->setText("<font style=\"font-size:11pt; font-weight:600; color:#3465a4;\"> Failed </font>");
			hLayoutLinkModule->addWidget(linkingLabel);

			LinkButton->setStyleSheet("background-color: rgb(52, 101, 164); color: rgb(255, 255, 255)");
			LinkButton->setText("Retry");
			linkSignalMapper->setMapping(LinkButton, mod_add_str);

			connect(LinkButton, SIGNAL(clicked()), linkSignalMapper, SLOT(map()));
			connect(linkSignalMapper, SIGNAL(mapped(QString)), this, SLOT(link_button_clicked_slot(QString)));

			hLayoutLinkModule->addWidget(LinkButton);
		}

		hLayoutLinkModule->addItem(hSpacerLinkModuleRight);

		ui->vLayout_linking_area->addLayout(hLayoutLinkModule);
	}

	verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	ui->vLayout_linking_area->addItem(verticalSpacer);
}

void module_manager::clearLayout(QLayout* layout)
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

void module_manager::link_button_clicked_slot(QString mod_add)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;

	int mod_add_int = stoi(mod_add.toStdString().substr(2, 2), 0, 16);

	for(int index = 0; index < 117; index++)
	{
		if(mod_add_int == link_mod_add_array[index][0])
		{
			link_mod_add_array[index][1] = MODULE_LINKING_PROGRESS;
			module_index = index;
			break;
		}
	}

	render_link_button_list();

	emit add_new_module_api_request_signal(mod_add.toStdString());
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
	console_print("Selected module address : " + hex_mod_add);
	ui->burn_button->setText("Burn");
	burn_btn_func = 0;

	switch (module_type_flag)
	{
	case ATTINY88:
		console_print("Selected module type: Attiny88");
		break;

	case ATMEGA328P:
		console_print("Selected module type: Atmega328p");
		break;
	}

	if(btn_state == "0")
	{
		QMessageBox msgBox;
		msgBox.setText("<font>" + hex_mod_add + " is not available to burn.</font>");
		msgBox.addButton(tr("Ok"), QMessageBox::NoRole);
		msgBox.exec();
	}
	else
	{
		QMessageBox::StandardButton confirmBox;
		confirmBox = QMessageBox::question(this, "Confirm", "Do you want to burn " + hex_mod_add + "?", QMessageBox::Yes|QMessageBox::No);
		if (confirmBox == QMessageBox::Yes)
		{
			ui->tabWidget->setCurrentIndex(BURN_TAB);
			ui->burn_status_label->setText("<font color=#3465A4>Selected address : </font><font color=#3465A4 size=4><b>" + hex_mod_add + "&nbsp;&nbsp;</b></font>");
			ui->overall_module_status->setText("Selected mod address : " + hex_mod_add);
			ui->tabWidget->setTabEnabled(BURN_TAB,true);
		}
	}
}

void module_manager::on_burn_button_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(burn_btn_func == 0)
		emit burn_module_signal(selected_mod_address, module_type_flag);
	else
		emit refresh_module_manager_signal(LINK_MODULE_TAB);
}

void module_manager::start_burning_slot()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	ui->burn_button->setDisabled(true);
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


	ui->burn_button->setDisabled(false);
	ui->progressBar->setValue(100);
	console_print("Burn Completed");
	console_print(log);

	ui->burn_status_label->setText("<font color=#3465A4>Module burn successful</font>");
	ui->burn_button->setText("Link Module");
	burn_btn_func = 1;
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

void module_manager::on_refresh_tool_button_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	int index = ui->tabWidget->currentIndex();
	if(index != BURN_TAB)
		emit refresh_module_manager_signal(index);
	else
		emit refresh_module_manager_signal(BURN_MODULE_TAB);
}

void module_manager::on_moudle_type_comboBox_currentIndexChanged(int index)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	cout << "Selected Index : " << index << endl;
	module_type_flag = (module_type)index;
	switch (module_type_flag)
	{
	case ATTINY88:
		console_print("Selected module type: Attiny88");
		break;

	case ATMEGA328P:
		console_print("Selected module type: Atmega328p");
		break;
	}
}
