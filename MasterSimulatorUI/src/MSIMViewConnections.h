#ifndef MSIMVIEWCONNECTIONS_H
#define MSIMVIEWCONNECTIONS_H

#include <QWidget>

namespace Ui {
class MSIMViewConnections;
}

class MSIMViewConnections : public QWidget
{
	Q_OBJECT

public:
	explicit MSIMViewConnections(QWidget *parent = 0);
	~MSIMViewConnections();

public slots:
	/*! Connected to MSIMProjectHandler::modified() */
	void onModified( int modificationType, void * data );

private slots:
	void on_pushButton_clicked();

private:
	Ui::MSIMViewConnections *m_ui;
};

#endif // MSIMVIEWCONNECTIONS_H