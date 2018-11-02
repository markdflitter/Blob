#include "field.h"
#include <ctime>

const double STATS_BAR_HEIGHT = 15.0;
const double STATS_BAR_WIDTH = 100.0;
const double STATS_BAR_SPACE = 4.0;
const double STATS_BAR_TEXT_SPACE_X = 35.0;
const double STATS_BAR_TEXT_SPACE_Y = 12.0;
const unsigned int STATS_BAR_TEXT_SIZE = 14U;
const double STATS_BAR_GRADUATION_WIDTH = 10.0;

Field::Field (QWidget *parent) : 
	  QWidget(parent)
{
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_TransparentForMouseEvents);
}

void Field::setBlobs(std::shared_ptr <std::vector<Blob>> blobs, unsigned int time)
{
	std::lock_guard<std::mutex> guard(_blobsMutex);
	_blobs = blobs;
}

void Field::redrawPixmap ()
{
	std::shared_ptr <std::vector <Blob>> blobs = getBlobs ();
	if (!blobs)
	{
		return ;
	}

	std::time_t time = std::time (nullptr);
	numRedrawsThisSecond++;
	if (time != lastReportTime)
	{
		lastReportTime = time;
		if (_rateListener)
		{
			_rateListener->setRedrawRate (numRedrawsThisSecond);
		}
		numRedrawsThisSecond = 0;
	}

	double scale = calculateScale ();
	Pt <double> offset = calculateOffset ();

	std::shared_ptr <QPixmap> pixmap (new QPixmap(size ()));
	pixmap->fill (Qt::black);
	QPainter pixmapPainter (pixmap.get ());
	pixmapPainter.translate (offset.x (), offset.y ());
	pixmapPainter.scale (scale, scale);
	
	size_t numBlobs = 0;
	size_t numLiveBlobs = 0;
	for (auto it = blobs->begin (); it != blobs->end (); it++)
	{
		numBlobs++;
		if (!it->isDead ()) numLiveBlobs++;
	}

	double labelFadeAlpha = featureFadeAlpha (numLiveBlobs, 30.0, 20.0);
	double statsBarsFadeAlpha = featureFadeAlpha (numLiveBlobs, 30.0, 20.0);
	bool labelDeadBlobs = numBlobs <= 10;		

	for (auto it = blobs->begin (); it != blobs->end (); it++)
	{
		drawSmellRange (pixmapPainter, *it, blobColour (*it), scale);
	}

	for (auto it = blobs->begin (); it != blobs->end (); it++)
	{
		drawPath (pixmapPainter, *it, blobColour (*it), scale); 
	}

	if (statsBarsFadeAlpha > 0.0)
	{
		for (auto it = blobs->begin (); it != blobs->end (); it++)
		{
			if (!it->isDead ())
			{
				drawStatsBars (pixmapPainter, *it, statsBarsFadeAlpha);
			}
		}
	}

	if (labelFadeAlpha > 0.0)
	{
		for (auto it = blobs->begin (); it != blobs->end (); it++)
		{
			if (!it->isDead () || labelDeadBlobs)
			{
				QColor colour = blobColour (*it);
				drawLabel (pixmapPainter, *it, blobColour (*it), labelFadeAlpha);
			}
		}
	}

	{
		std::lock_guard<std::mutex> guard(_pixmapMutex);
		_pixmap = pixmap;
	}
	update ();
}

void Field::paintEvent(QPaintEvent *)
{
	std::shared_ptr <QPixmap> pixmap;
	{
		std::lock_guard<std::mutex> guard(_pixmapMutex);
		pixmap = _pixmap;
	}
	QPainter screenPainter (this);
	if (pixmap != 0)
	{
		screenPainter.drawPixmap (0, 0, *pixmap);
	}
}

double Field::calculateScale () const
{
	Pt<double> srcSize (2 * Blob::WORLD_SIZE ().x (), 2 * Blob::WORLD_SIZE (). y ());
	double xScale = size ().width () / srcSize.x ();
	double yScale = size ().height () / srcSize.y ();
	double s = std::min (std::min (xScale, yScale), 1000.0);
	return s;
}

Pt <double> Field::calculateOffset () const
{
	return Pt<double> (size ().width () / 2, size ().height () / 2);
}

void Field::drawSmellRange (QPainter& painter,
			    const Blob& blob,
			    const QColor& colour,
			    double scale)
{
	painter.save ();

	const Pt<double>& centre = blob.history ().back ();

	if (!blob.isDead ())
	{
		QPen pen (colour);
		pen.setWidthF (1.0 / scale);	
		painter.setPen (pen);
		painter.drawEllipse (QPointF (centre.x (), centre.y ()),(double) blob.smell (),(double) blob.smell ());
	}

	painter.setBrush (QBrush (colour));
	
	QColor noColour (Qt::black);
	noColour.setAlphaF (0.0);
	painter.setPen (QPen (noColour));

	if (blob.size () > 0U)
	{
		painter.drawEllipse (QPointF (centre.x (), centre.y ()), (blob.size () / 60.0), (blob.size () / 60.0));
	}
	painter.restore ();
}

void Field::drawLabel (QPainter& painter,
		       const Blob& blob,
		       const QColor& colour,
		       double fadeAlpha)
{
	painter.save ();

	Pt<double> p = blob.history ().back ();

	QPointF targetPt = painter.worldTransform ().map (QPointF (p.x (), p.y ()));
	targetPt += QPointF (15.0, -15.0);

	painter.resetTransform ();

	QColor fadedColour = colour;
	fadedColour.setAlphaF (fadeAlpha);
	painter.setPen (QPen (fadedColour));
	painter.drawText (targetPt,  (blob.name () + " is " + blob.state ()).c_str ());

	painter.restore ();
}

void Field::drawPath (QPainter& painter,
		      const Blob& blob,
		      const QColor& colour,
		      double scale)
{	
	painter.save ();

	const std::vector<Pt<double>>& points = blob.history ();

	Pt<double> last = points.back ();

	QColor fadedColour = colour;
	size_t count = 0;
	for (auto it = points.rbegin () + 1; it != points.rend (); ++it)
	{
		if (blob.isDead ()) break;
		
		const Pt <double>& p = *it;
		fadedColour.setAlphaF (limit (1.0 - ((double) count / points.size ()), 0.0, 1.0));
		QPen pen (fadedColour);
		pen.setWidthF (1.0 / scale);
		painter.setPen (pen);
	
		painter.drawLine(QPointF (last.x (), last.y ()), QPointF (p.x (), p.y ()));

		last = p;
		count++;

	}
	painter.restore ();
}

void Field::drawStatsBars (QPainter& painter,
			   const Blob& blob,
			   double fadeAlpha)
{
	painter.save ();

	Pt<double> p = blob.history ().back ();

	QPointF targetPt = painter.worldTransform ().map (QPointF (p.x (), p.y ()));
	targetPt += QPointF (30.0, -10.0);

	painter.resetTransform ();

	QColor colour (Qt::black);
	colour.setAlphaF (0.5 * fadeAlpha);
	double panelWidth = STATS_BAR_WIDTH + 4 * STATS_BAR_SPACE + STATS_BAR_TEXT_SPACE_X;
	
	painter.fillRect (QRectF (targetPt.x () - 2 * STATS_BAR_SPACE,
				  targetPt.y () - 2 * STATS_BAR_SPACE, 
				  panelWidth,
				  5 * STATS_BAR_HEIGHT + 8 * STATS_BAR_SPACE),
				  colour);

	drawStatsBar (painter, QPointF (targetPt.x (), targetPt.y () + 0 * (STATS_BAR_HEIGHT + STATS_BAR_SPACE)),
		blob.currentAge (), blob.lifespan (), fadeAlpha, true, "age");
	drawStatsBar (painter, QPointF (targetPt.x (), targetPt.y () + 1 * (STATS_BAR_HEIGHT + STATS_BAR_SPACE)),
		blob.hunger (), blob.maxHunger (), fadeAlpha, true, "eat");
		drawStatsBar (painter, QPointF (targetPt.x (), targetPt.y () + 2 * (STATS_BAR_HEIGHT + STATS_BAR_SPACE)),
		blob.HP (), blob.baseHP (), fadeAlpha, false, "HP");
	drawStatsBar (painter, QPointF (targetPt.x (), targetPt.y () + 3 * (STATS_BAR_HEIGHT + STATS_BAR_SPACE)),
		blob.damage (), blob.baseDamage (), fadeAlpha, false, "dmg");
	drawStatsBar (painter, QPointF (targetPt.x (), targetPt.y () + 4 * (STATS_BAR_HEIGHT + STATS_BAR_SPACE)),
		blob.endurance () - blob.fatigue (), blob.endurance (), fadeAlpha, false, "nrg");

	painter.restore ();
}

void Field::drawStatsBar (QPainter& painter,
			  const QPointF& pt,
			  unsigned int current,
			  unsigned int max,
			  double fadeAlpha,
			  bool inverse,
			  const std::string& label)
{
	painter.save ();

	unsigned int v = current;
	double percent = 100.0 * v / (double) max;
	if (max == 0U) percent = 100.0;

	QColor labelColour (0.0, 128.0, 0.0);
	labelColour.setAlphaF (fadeAlpha);
	painter.setPen (QPen (labelColour));

	QFont font = painter.font() ;
	font.setPixelSize (STATS_BAR_TEXT_SIZE);
	painter.setFont(font);
	
	painter.drawText (QPointF (pt.x (), pt.y () + STATS_BAR_TEXT_SPACE_Y),  label.c_str ());

	double colourRatio = (inverse? 100.0 - percent : percent) / 100.0;
	double r = std::min (128.0, (1.0 - colourRatio) * 128.0); 
	double g = std::min (128.0, colourRatio * 128.0);
	QColor fadedColour (r, g, 0.0);
	fadedColour.setAlphaF (fadeAlpha);
	painter.setPen (QPen (fadedColour));

	QColor fadedBackgroundColour = Qt::black;
	fadedBackgroundColour.setAlphaF (fadeAlpha);
	painter.setBrush (QBrush (fadedBackgroundColour));
	
	painter.drawRect (pt.x () + STATS_BAR_TEXT_SPACE_X, pt.y (), STATS_BAR_WIDTH, STATS_BAR_HEIGHT);

	if (percent > 0.0)
	{
		painter.fillRect (QRectF (pt.x () + STATS_BAR_TEXT_SPACE_X,
					  pt.y (),
					  STATS_BAR_WIDTH * percent / 100.0,
				          STATS_BAR_HEIGHT),
  				  fadedColour);
	}

	double x = STATS_BAR_GRADUATION_WIDTH;
	while (x < STATS_BAR_WIDTH * percent / 100.0)
	{
		QColor graduationColour (Qt::black);
		graduationColour.setAlphaF (fadeAlpha);
		painter.fillRect (QRectF (pt.x () + STATS_BAR_TEXT_SPACE_X + x,
					  pt.y (),
					  1.0,
					  STATS_BAR_HEIGHT),
				  graduationColour);
		x += STATS_BAR_GRADUATION_WIDTH;
	}

	std::stringstream ss;
	ss << v;
	QColor numberColour (Qt::white);
	numberColour.setAlphaF (fadeAlpha);
	painter.setPen (numberColour);
	font.setPixelSize (STATS_BAR_TEXT_SIZE - 2);
	painter.setFont(font);
	painter.drawText (QPointF (pt.x () + STATS_BAR_TEXT_SPACE_X + STATS_BAR_SPACE / 2, pt.y () + STATS_BAR_TEXT_SPACE_Y),
			  ss.str ().c_str ());


	painter.restore ();
}

double Field::limit (double v, double min, double max)
{
	return std::min (std::max (v, min), max);
}

double Field::featureFadeAlpha (double current, double min, double max) 
{
	return limit ((current - min) / (max - min), 0.0, 1.0);
}

QColor Field::blobColour (const Blob& blob)
{
	double a = blob.aggression () + 1.0;
	unsigned int red = (unsigned) limit (a * 255.0, 0.0, 255.0);
	unsigned int blue = (unsigned) limit (blob.maxWanderingSpeed () * 100.0, 0.0, 255.0);
	unsigned int green = 0U;
	
	if ((red < 128U) && (blue < 128U))
	{		
		green = 255U;
	}
	else
	{
		green = 128U;
	}
	//std::cout << red << ", " << green << ", " << blue << std::endl;
	QColor result (red, green, blue);

	result.setAlphaF (blob.fade ());

	return result;
}

