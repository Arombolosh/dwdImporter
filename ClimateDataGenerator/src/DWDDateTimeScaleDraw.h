#ifndef DWDDateTimeScaleDrawH
#define DWDDateTimeScaleDrawH

#include <qwt_scale_draw.h>
#include <qwt_text.h>
#include <QCoreApplication>

class QPainter;
class QPalette;

class DWDDateTimeScaleDraw : public QwtScaleDraw {
	Q_DECLARE_TR_FUNCTIONS(QwtDateTimeScaleDraw)
public:
	DWDDateTimeScaleDraw();
	virtual void drawLabel(QPainter *p, double val) const;
	virtual void draw(QPainter * painter, const QPalette & pal) const;

	QString longMonthNames[12];
	QString shortMonthNames[12];
};

#endif // QwtDateTimeScaleDrawH
