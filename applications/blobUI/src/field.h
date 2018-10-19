#ifndef INCLUDED_FIELD_H
#define INCLUDED_FIELD_H

#include <QtWidgets>
#include <qcolor.h>
#include "setBlobs.h"
#include "redraw.h"
#include <mutex>
#include <thread>
#include <limits>
#include <vector>
#include <blob.h>
#include "rateListener.h"

class Field : public QWidget, public SetBlobs, public Redraw
{
	public:
		Field(QWidget *parent = nullptr);

		void setBlobs (std::shared_ptr <std::vector<Blob>> blobs, unsigned int time);
		void redrawPixmap ();
		void setRateListener (RateListener* rateListener) {_rateListener = rateListener;}
	protected:
		void paintEvent(QPaintEvent *) override;
	private:
		mutable std::mutex _blobsMutex;
		std::shared_ptr <std::vector <Blob>> _blobs = nullptr;

		mutable std::mutex _pixmapMutex;
		std::shared_ptr <QPixmap> _pixmap;

		unsigned int numRedrawsThisSecond = 0U;
		std::time_t lastReportTime = std::time (nullptr);

		RateListener* _rateListener = 0;

		std::shared_ptr <std::vector <Blob>> getBlobs ()
		{
			std::lock_guard<std::mutex> guard(_blobsMutex);
			auto result = _blobs;
			_blobs = nullptr;
			return result;	
		}

		double calculateScale () const;
		Pt<double> calculateOffset () const;

		void drawSmellRange (QPainter& painter,
				     const Blob& blob,
				     const QColor& colour,
				     double scale);

		void drawLabel (QPainter& painter,
				const Blob& blob,
				const QColor& colour,
			        double fadeAlpha);

		void drawPath (QPainter& painter,
			       const Blob& blob,
			       const QColor& colour,
			       double scale);
	
		void drawStatsBars (QPainter& painter,
				    const Blob& blob,
				    double fadeAlpha);

		void  drawStatsBar (QPainter& painter,
				    const QPointF& pt,
				    unsigned int current,
				    unsigned int max,
				    double fadeAlpha,
				    bool inverse,
				    const std::string& label);

		static double limit (double v, double min = 0.0, double max = 1.0);
		static double featureFadeAlpha (double current, double min, double max); 
		static QColor blobColour (const Blob& blob);
};

#endif // FIELD_M_H
