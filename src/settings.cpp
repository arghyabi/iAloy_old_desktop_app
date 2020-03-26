#include<iostream>
#include <QApplication>
#include <QFontDatabase>

#include "settings.h"
#include "ui_settings.h"

using namespace std;

settings::settings(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::settings)
{
	ui->setupUi(this);
	settings::init();
}

settings::~settings()
{
	delete ui;
}

void settings::init()
{
	int font_size[] = {8,9,10,11,12,14,16,18,20};

	system("basename -a -s .ttf /usr/share/iAloy/fonts/*.ttf > font.txt");

	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen("font.txt", "r");
	if (fp != NULL)
	{
		while ((read = getline(&line, &len, fp)) != -1)
		{
			line[strlen(line)-1] = '\0';
			ui->font_name_select_combobox->addItem(QString::fromUtf8(line));
		}

		fclose(fp);
		if (line)
			free(line);
	}

	system("rm font.txt");

	for(int i = 0; i < sizeof(font_size)/sizeof(int); i++)
	{
		ui->font_size_select_combobox->addItem(QString::number(font_size[i]));
	}
}

void settings::set_font_name_and_size()
{
	QString fname = ui->font_name_select_combobox->currentText();
	QString fsize = ui->font_size_select_combobox->currentText();

	QFontDatabase::addApplicationFont("/usr/share/iAloy/fonts/" + fname + ".ttf");
	QApplication::setFont(QFont(fname, fsize.toInt(), QFont::Normal, false));
}

void settings::on_buttonBox_accepted()
{
	set_font_name_and_size();
}

void settings::on_pushButton_clicked()
{
	set_font_name_and_size();
}
