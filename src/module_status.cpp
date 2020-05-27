#include "module_status.h"
#include "ui_module_status.h"

module_status::module_status(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::module_status)
{
	ui->setupUi(this);

	i2c_data *i2c_thread_obj = new i2c_data(this);
	connect(this, SIGNAL(read_all_i2c_module_state_signal(int*)), i2c_thread_obj, SLOT(read_all_i2c_module_state(int*)));
}

module_status::~module_status()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	delete ui;
}

void module_status::button_clicked_slot(QString btn_msg)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	cout << "buttton pressed: " << btn_msg.toStdString() << endl;
	QMessageBox msgBox;
	QAbstractButton *link_button, *burn_button;
	if(btn_msg == "<font><b>Module connected, but not linked</b></font>")
	{
		link_button = msgBox.addButton(tr("Link Module"), QMessageBox::YesRole);
	}
	else if(btn_msg == "<font><b>Address is available to burn new Module</b></font>")
	{
		burn_button = msgBox.addButton(tr("Burn Module"), QMessageBox::YesRole);
	}

	msgBox.setStyleSheet("QPushButton{padding: 10px; background-color:#3465A4; color: #FFF;}QMessageBox{height: auto; width: auto; padding-left: 20px; padding-right: 20px;}");
	msgBox.setText("<font>" + btn_msg + "</font>");
	msgBox.addButton(tr("Ok"), QMessageBox::NoRole);
	msgBox.exec();

	if (msgBox.clickedButton() == link_button)
	{
		this->hide();
		emit link_module_manager_signal(LINK_MODULE_TAB);
	}
	else if(msgBox.clickedButton() == burn_button)
	{
		this->hide();
		emit link_module_manager_signal(BURN_MODULE_TAB);
	}
}

void module_status::init(QLinkedList<btn_node*> tmp_btn_list)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->show();

	int col = 0;
	int row = 0;
	int status_array[117];
	int module_active_count = 0;
	int module_available_count = 0;
	int module_disconnected_count = 0;
	int module_not_available_count = 0;
	int linked_module_available_count = 0;
	int linked_module_not_available_count = 0;

	QString head_style = "padding-top: 6px; padding-bottom: 6px; padding-left: 6px; padding-right: 6px; text-align: center; background-color: #005DD5; color: #FFF;";
	QString body_style = "border: 1px solid #ddd; padding: 4px; text-align: center;";
	QString table_start = "<table style='border-collapse: collapse width: 100%; margin: 10px; margin-right: 30px;' border=1> <tr> <th style='" + head_style + "'>Device</th> <th style='" + head_style + "'>Pin</th> <th style='" + head_style + "'>Status</th> <th style='" + head_style + "'>Slider</th> </tr>";
	QString body_start = "<tr><td><center>";
	QString body_end = "</center></td></tr>";
	QString table_end = "</table>";
	QString body_joint = "</center></td><td><center>";

	emit read_all_i2c_module_state_signal(status_array);

	for (int index = 0; index < 117; index++)
	{
		// adding pushbutton to grid
		int mod_add = index+3;
		struct btn_node *btn_nd;
		int isFound = 0;
		QString module_info;

		QString room_name_array[8];
		QString final_module_details_string;
		int room_count = 0;

		foreach(btn_nd, tmp_btn_list)
		{
			if(btn_nd->mod_add == mod_add)
			{
				if(btn_nd->device_id == "-1")
					isFound = -1;
				else
					isFound = 1;
				QString tmp_room_name = btn_nd->room_name;
				bool roomFound = false;

				for(int i = 0; i <= room_count ; i++)
				{
					if(room_name_array[i] != NULL)
					{
						if(room_name_array[i].mid(3,tmp_room_name.length()) == tmp_room_name)
						{
							QString state = "";
							QString slider = "-";

							if(btn_nd->btn_state)
								state = "<font color='#005DD5'>ON</font>";
							else
								state = "<font color='#525252'>OFF</font>";

							if(btn_nd->is_var)
								slider = QString::number(btn_nd->slider_val)+"%";

							room_name_array[i] = room_name_array[i] + body_start + btn_nd->device_name + body_joint + QString::number(btn_nd->pin_num) + body_joint + state + body_joint + slider + body_end;
							roomFound = true;
							break;
						}
					}
				}

				if(!roomFound)
				{
					if(room_count > 0)
						room_name_array[room_count - 1] += table_end;

					QString state = "";
					QString slider = "-";

					if(btn_nd->btn_state)
						state = "<font color='#005DD5'>ON</font>";
					else
						state = "<font color='#525252'>OFF</font>";

					if(btn_nd->is_var)
						slider = QString::number(btn_nd->slider_val)+"%";

					room_name_array[room_count++] = "<b>" + tmp_room_name + "</b>:<br/>" + table_start + body_start + btn_nd->device_name + body_joint + QString::number(btn_nd->pin_num) + body_joint + state + body_joint + slider + body_end;
				}
				roomFound = false;
			}
		}

		for(int i = 0; i < room_count; i++)
		{
			if(i != 0)
				final_module_details_string += "<br/><br/>" + room_name_array[i];
			else
				final_module_details_string = room_name_array[i];
		}

		if(status_array[index] == 1)
			final_module_details_string += table_end + "<br/><br/><font color='#1F6600'><b>Module perfectly working.</b></font>";
		else
			final_module_details_string += table_end + "<br/><br/><font color='#FF0000'><b>Module is not working.</b></font>";

		if(isFound == 0 && room_count == 0)
		{
			if(status_array[index] == 1)
				final_module_details_string = "<font><b>Module connected, but not linked</b></font>";
			else if(status_array[index] == 0)
				final_module_details_string = "<font><b>Address is available to burn new Module</b></font>";
		}

		if(isFound == -1)
		{
			if(status_array[index] == 1)
				final_module_details_string = "<center><font color='#0000FF'><b>Module connected & linked<br/>Add button from Web</b></font></center>";
			else if(status_array[index] == 0)
				final_module_details_string = "<font><b>Module linked but not connected.</b></font>";
		}


		btn = new QPushButton(final_module_details_string);
		if(isFound == 1)
		{
			if(status_array[index] == 1)
			{
				btn->setStyleSheet(QString::fromUtf8("height: 16px; padding: 5%; background-color: #00FF00; color: #000;"));
				module_active_count++;
			}
			else if(status_array[index] == 0)
			{
				btn->setStyleSheet(QString::fromUtf8("height: 16px; padding: 5%; background-color: #FF0000; color: #FFF;"));
				module_disconnected_count++;
			}
		}
		else if(isFound == 0)
		{
			if(status_array[index] == 1)
			{
				btn->setStyleSheet(QString::fromUtf8("height: 16px; padding: 5%; background-color: #FFFF00; color: #000;"));
				module_available_count++;
			}
			else if(status_array[index] == 0)
			{
				btn->setStyleSheet(QString::fromUtf8("height: 16px; padding: 5%; background-color: #525252; color: #FFF;"));
				module_not_available_count++;
			}
		}
		else
		{
			if(status_array[index] == 1)
			{
				btn->setStyleSheet(QString::fromUtf8("height: 16px; padding: 5%; background-color: #0000FF; color: #FFF;"));
				linked_module_available_count++;
			}
			else if(status_array[index] == 0)
			{
				btn->setStyleSheet(QString::fromUtf8("height: 16px; padding: 5%; background-color: #FFFFFF; color: #000;"));
				linked_module_not_available_count++;
			}
		}


		btn->setText(QString::fromStdString(int_to_hex(mod_add)));
		ui->status_grid_layout->addWidget(btn, row, col, 1,1);

		QString active_module_status_msg = "<font color=#000000>Active : </font><font style=\"color:#FFFFFF; background-color:#1b9300;\">&nbsp;&nbsp;"+QString::number(module_active_count)+"&nbsp;&nbsp;</font>";
		QString inactive_module_status_msg = "<font color=#000000>Inactive : </font><font style=\"color:#FFFFFF; background-color:#BC0000;\">&nbsp;&nbsp;"+QString::number(module_disconnected_count)+"&nbsp;&nbsp;</font>";
		QString unlinked_module_status_msg = "<font color=#000000>Unlinked : </font><font style=\"color:#FFFFFF; background-color:#BFBF00;\">&nbsp;&nbsp;"+QString::number(module_available_count)+"&nbsp;&nbsp;</font>";
		QString not_available_module_status_msg = "<font color=#000000>Not available : </font><font style=\"color:#FFFFFF; background-color:#434338;\">&nbsp;&nbsp;"+QString::number(module_not_available_count)+"&nbsp;&nbsp;</font>";
		QString linked_connected_module_status_msg = "<font color=#000000>Linked & connected : </font><font style=\"color:#FFFFFF; background-color:#0000FF;\">&nbsp;&nbsp;"+QString::number(linked_module_available_count)+"&nbsp;&nbsp;</font>";
		QString linked_not_connected_module_status_msg = "<font color=#000000>Linked & not connected : </font><font style=\"color:#000000; background-color:#FFFFFF;\">&nbsp;&nbsp;"+QString::number(linked_module_not_available_count)+"&nbsp;&nbsp;</font>";

		QString overall_module_status_msg = active_module_status_msg+"&nbsp;&nbsp;&nbsp;&nbsp;"+inactive_module_status_msg+"<br/>"+unlinked_module_status_msg+"&nbsp;&nbsp;&nbsp;&nbsp;"+not_available_module_status_msg \
				+"<br/>"+linked_connected_module_status_msg+"&nbsp;&nbsp;&nbsp;&nbsp;"+linked_not_connected_module_status_msg;
		ui->overall_module_status->setText(overall_module_status_msg);

		signalMapper = new QSignalMapper(this);
		signalMapper->setMapping(btn, final_module_details_string);

		connect(btn, SIGNAL(clicked()), signalMapper, SLOT(map()));
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