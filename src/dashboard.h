#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QDialog>

namespace Ui {
class dashboard;
}

class dashboard : public QDialog
{
	Q_OBJECT

public:
	explicit dashboard(QWidget *parent = nullptr);
	void addBgImage();
	~dashboard();

private:
	Ui::dashboard *ui;
};

#endif // DASHBOARD_H
