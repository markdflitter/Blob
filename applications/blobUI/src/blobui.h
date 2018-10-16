#ifndef BLOBUI_M_H
#define BLOBUI_M_H

#include <QMainWindow>
#include "ui_blobui.h"
#include "field.h"
#include "fieldThreadWorker.h"
#include "redrawThreadWorker.h"
#include "setBlobs.h"
#include "rateListener.h"

namespace Ui {
	class BlobUI;
}

class EventFilter;

class BlobUI : public QMainWindow, public SetBlobs, public RateListener
{
	Q_OBJECT

	public:
		explicit BlobUI(QWidget *parent = nullptr);
		~BlobUI();

		void setBlobs (std::shared_ptr <std::vector<Blob>> blobs, unsigned int time);
		void onExit (void);

		void setRedrawRate (unsigned int redrawRate)
		{
			_redrawRate = redrawRate;
			redrawStatusBar ();
		}
		
		void setCalcRate (unsigned int calcRate)
		{
			_calcRate = calcRate;
			redrawStatusBar ();
		}
	private:
		Ui::BlobUI *ui;
		Field* _field;
		EventFilter* _eventFilter;

		FieldThreadWorker _fieldThreadWorker;
		std::thread _workerThread;
		RedrawThreadWorker _redrawThreadWorker;
		std::thread _redrawThread;

		std::atomic <unsigned int> _calcRate;
		std::atomic <unsigned int> _redrawRate;
		
		mutable std::mutex _mutex;
		unsigned int _time = 0U;
		size_t _liveBlobs = 0U;
		size_t _deadBlobs = 0U;
	
		void redrawStatusBar ()
		{
			std::string s = std::to_string (_time) + " : " + 
				std::to_string (_liveBlobs) + " alive, " + 
				std::to_string (_deadBlobs) + " dead" +
				"                    " + 
				"(calc = " + std::to_string (_calcRate) + " Hz / " +
				"redraw = " + std::to_string (_redrawRate) + " Hz)";
			QMetaObject::invokeMethod (ui->statusBar, "showMessage", Qt::QueuedConnection, Q_ARG (QString, s.c_str ()));
		}
};


#endif // BLOBUI_M_H
