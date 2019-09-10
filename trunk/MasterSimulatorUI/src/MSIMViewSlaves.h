#ifndef MSIMVIEWSLAVES_H
#define MSIMVIEWSLAVES_H

#include <QWidget>

namespace QPW {
	class VariantPropertyManager;
}

namespace Ui {
	class MSIMViewSlaves;
}

namespace BLOCKMOD {
	class SceneManager;
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

private slots:
	void on_toolButtonAddSlave_clicked();

	void on_toolButtonRemoveSlave_clicked();

	void on_tableWidgetSlaves_cellChanged(int row, int column);

	void on_checkBoxRelativeFMUPaths_toggled(bool checked);

	void on_tableWidgetSlaves_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

	void on_toolButtonCreateConnection_clicked();

private:
	void updateSlaveTable();

	/*! This function is called from onModified(), whenever slave data or FMU specs have changed.
		The function processes all slaves and associated FMU data (if read), and checks if
		the block names and socket number/types/names match those of the FMU slave.
		If not, the block is marked as "outdated"/not configured and the user is requested to
		configure the blocks one-by-one.
	*/
	void syncCoSimNetworkToBlocks();

	Ui::MSIMViewSlaves				*m_ui;

	/*! The manager for the properties. */
	QPW::VariantPropertyManager		*m_variantManager;

};

#endif // MSIMVIEWSLAVES_H
