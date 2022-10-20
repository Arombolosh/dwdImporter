/*	The Thermal Room Model
	Copyright (C) 2010  Andreas Nicolai <andreas.nicolai -[at]- tu-dresden.de>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DWDDateTimeScaleDraw.h"

#include <QDateTime>
#include <QPainter>
#include <QFontMetrics>
#include <QPalette>

#include "Utilities.h"

DWDDateTimeScaleDraw::DWDDateTimeScaleDraw() {
		longMonthNames[0] = tr("January");
		longMonthNames[1] = tr("February");
		longMonthNames[2] = tr("March");
		longMonthNames[3] = tr("April");
		longMonthNames[4] = tr("May");
		longMonthNames[5] = tr("June");
		longMonthNames[6] = tr("July");
		longMonthNames[7] = tr("August");
		longMonthNames[8] = tr("September");
		longMonthNames[9] = tr("October");
		longMonthNames[10] = tr("November");
		longMonthNames[11] = tr("December");
		shortMonthNames[0] = tr("Jan");
		shortMonthNames[1] = tr("Feb");
		shortMonthNames[2] = tr("Mar");
		shortMonthNames[3] = tr("Apr");
		shortMonthNames[4] = tr("May");
		shortMonthNames[5] = tr("Jun");
		shortMonthNames[6] = tr("Jul");
		shortMonthNames[7] = tr("Aug");
		shortMonthNames[8] = tr("Sep");
		shortMonthNames[9] = tr("Oct");
		shortMonthNames[10] = tr("Nov");
		shortMonthNames[11] = tr("Dec");
}

void DWDDateTimeScaleDraw::drawLabel(QPainter *, double ) const {
	// only called for major tick labels
}

void DWDDateTimeScaleDraw::draw(QPainter * painter, const QPalette & pal) const {
	QwtScaleDraw::draw(painter, pal);
	// now draw the labels
	const QList<double> &majorTicks =
		scaleDiv().ticks(QwtScaleDiv::MajorTick);

	if (majorTicks.count() == 0) {
		// not even a single day is shown in the current zoom level
		return;
	}

	painter->save();


#if QT_VERSION < 0x040000
	painter->setPen(colorGroup.text()); // ignore pen style
#else
	painter->setPen(pal.color(QPalette::Text)); // ignore pen style
#endif


	double day1 = majorTicks[0];
	double day2 = day1;
	if (majorTicks.count() > 1)
		day2 = majorTicks[majorTicks.count()-1];
	unsigned int mon1 = month(day1);
	unsigned int mon2 = month(day2);

	// draw month name directly between labels
	QFontMetrics fm(painter->font());

	// if we have month scale
	if (mon2 - mon1 > 3) {

		// determine width of labels
		QPointF labPos = labelPosition(dayAtMonthStart[mon1]);
		QPointF newLabPos = labelPosition(dayAtMonthStart[mon1+1]);
		int width = newLabPos.x() - labPos.x();
		// check if we can draw all full month names
		bool useLongNames = true;
		for (unsigned int i=0; i<12; ++i) {
			if (fm.boundingRect(longMonthNames[i]).width() > width) {
				useLongNames = false;
				break;
			}
		}

		labPos = labelPosition(day1);
		for (unsigned int i=mon1; i<=mon2; ++i) {
			QPointF newLabPos = labelPosition(dayAtMonthStart[i]);
			QString name;
			if (useLongNames)
				name = longMonthNames[i];
			else
				name = shortMonthNames[i];
			painter->drawText(labPos.x(), labPos.y() - 5, newLabPos.x() - labPos.x(), 20,
				Qt::AlignHCenter | Qt::AlignTop, name);
			labPos = newLabPos;
		}

		const QList<double> &minorTicks =
			scaleDiv().ticks(QwtScaleDiv::MinorTick);

		// check if distance between ticks is big enough for week numbers
		double d1 = minorTicks[0];
		double d2 = d1;
		if (minorTicks.count() > 1)
			d2 = minorTicks[1];

		unsigned int w1 = labelPosition(d2).x() - labelPosition(d1).x();
		unsigned int w2 = fm.boundingRect("5555").width();
		if (w1 >= w2) {
			for (int i = 0; i < (int)minorTicks.count(); i++) {
				const double v = minorTicks[i];
				labPos = labelPosition(v);
				if ( scaleDiv().contains(v) ) {
				painter->drawText(labPos.x() + 3, labPos.y() - fm.lineSpacing() - 1,
					50, 20, Qt::AlignLeft | Qt::AlignTop,
					QString("%1").arg((unsigned int)v/7.0 + 1) );
				}
			}
		}
	}
	else if (day2 - day1 > 14) {
		QPointF labPos = labelPosition(day1);
		// draw week number between week ticks
		for (int i = 0; i < (int)majorTicks.count(); i++) {
			const double v = majorTicks[i];
			QPointF newLabPos = labelPosition(v);
			painter->drawText(labPos.x(), labPos.y() - 5, newLabPos.x() - labPos.x(), 20,
				Qt::AlignHCenter | Qt::AlignTop,
				QString(tr("Week %1")).arg((unsigned int)v/7.0 + 1) );
			labPos = newLabPos;
		}
	}
	else {
		// draw date
		for (int i = 0; i < (int)majorTicks.count(); i++) {
			const double v = majorTicks[i];
			QPointF labPos = labelPosition(v);
			QDate d(2001,1,1);
			d = d.addDays(v);
			QString name = d.toString(tr("ddd MMM/dd"));
			painter->drawText(labPos.x(), labPos.y() - 5, 50, 20,
				Qt::AlignLeft | Qt::AlignTop,
				name );
		}
	}


	painter->restore();
}

