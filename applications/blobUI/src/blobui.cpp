#include "blobui.h"
#include <iostream>

class EventFilter : public QObject
{
	public:
		EventFilter (QWidget* field) : _field (field) {}

	protected:
		bool eventFilter(QObject *obj, QEvent *ev) override {
		if (!obj->isWidgetType()) return false;
			auto w = static_cast<QWidget*>(obj);
			if (ev->type() == QEvent::Resize) {
				if (_field->parent () == w) {
					_field->resize(w->size());
				}
			}
			return false;
		}
	private:
		QWidget* _field;
};


BlobUI::BlobUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BlobUI)
{
	_calcRate = 0U;
	_redrawRate = 0U;
	_time = 0U;
	_liveBlobs = 0U;
	_deadBlobs = 0U;

    	ui->setupUi(this);
	connect (ui->actionExit, &QAction::triggered, this, &BlobUI::onExit);

	_field = new Field (ui->centralWidget);
	_field->setRateListener (this);
	_field->show ();
	_eventFilter = new EventFilter (_field);
	ui->centralWidget->installEventFilter (_eventFilter);

	_fieldThreadWorker.setRateListener (this);
	_workerThread = _fieldThreadWorker.start (this);
	_redrawThread = _redrawThreadWorker.start (_field);

}

BlobUI::~BlobUI()
{
	_redrawThreadWorker.stop ();
	_fieldThreadWorker.stop ();
 	_workerThread.join ();
	_redrawThread.join ();

	delete ui;
	delete _field;
	delete _eventFilter;
}

void BlobUI::setBlobs (std::shared_ptr <std::vector<Blob>> blobs, unsigned int time)
{
	size_t liveBlobs = 0;
	size_t deadBlobs = 0;
	
	for (auto it = blobs->begin (); it != blobs->end (); it++)
	{
		if (it->isDead ())
			deadBlobs++;
		else
			liveBlobs++;
	}

	{
		std::lock_guard <std::mutex> guard (_mutex);
		_liveBlobs = liveBlobs;
		_deadBlobs = deadBlobs;
		_time = time;
	}

	redrawStatusBar ();
	
	_field->setBlobs (blobs, time);
}

void BlobUI::onExit (void)
{
	exit (0);
}


