#ifndef MSIMVIEWSLAVES_H
#define MSIMVIEWSLAVES_H

#include <QWidget>

namespace Ui {
class MSIMViewSlaves;
}

/*! The view containing FMU and slave definition tables. */
class MSIMViewSlaves : public QWidget {
	Q_OBJECT
public:
	explicit MSIMViewSlaves(QWidget *parent = 0);
	~MSIMViewSlaves();

public slots:
	/*! Connected to MSIMProjectHandler::modified() */
	void onModified( int modificationType, void * data );

private:
	Ui::MSIMViewSlaves *m_ui;
};

#endif // MSIMVIEWSLAVES_H
