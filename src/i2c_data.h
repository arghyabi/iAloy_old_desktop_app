#ifndef I2C_DATA_H
#define I2C_DATA_H

#include <QObject>

class i2c_data : public QObject
{
	Q_OBJECT
public:
	explicit i2c_data(QObject *parent = nullptr);

signals:
	void receive_i2c_data_from_module(int, int);


public slots:
	void write_i2c_data(int, int);
	void read_i2c_data(int, int);
};

#endif // I2C_DATA_H