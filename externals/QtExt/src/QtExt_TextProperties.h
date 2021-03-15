/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_TextPropertiesH
#define QtExt_TextPropertiesH

#include <QFont>

class QTextDocument;

namespace QtExt {

/*! \brief Helper class for gathering properties of HTML formated text.
	It contains text height, ascend and descend of the most used HTML format text in the currently used renderer.
	These properties are depending on the used paint device.
*/
class TextProperties {
public:
	/*! Collection of text size information.*/
	struct TextSizes {
		int height;		///< Text height
		int ascent;		///< Upward vertical stem on some lowercase letters (e.g. h).
		int descent;	///< Portion of some lowercase letters (like g or p) that extends baseline.
	};

	/*! Standard constructor.
		\param fontFamily Font familiy name used for setting normal font.
		\param fontPointSize Font size used for normal text.
		\param textDocument Text document object used for render text (HTML).
		\param paintDevice If paintDevice is not 0 the text document will be set to this paintDevice.
		The paint device for the text document must be set before call of this function.
	*/
	TextProperties(const QString& fontFamily, int fontPointSize,
				   QTextDocument* textDocument, QPaintDevice* paintDevice = 0);

	/*! Updates all internal variables acording to current state of text document.*/
	void update(QTextDocument* textDocument, QPaintDevice* paintDevice = 0);

	/*! Returns the font used for HTML rendering.*/
	const QFont&		normalFont() const { return m_normalFont; }

	/*! Text properties for normal text.*/
	const TextSizes&	normal() const { return m_textNormal; }

	/*! Text properties for H1 text (caption first order).*/
	const TextSizes&	H1() const { return m_textH1; }

	/*! Text properties for H2 text (caption second order).*/
	const TextSizes&	H2() const { return m_textH2; }

	/*! Text properties for H3 text (caption 3rd order).*/
	const TextSizes&	H3() const { return m_textH3; }

	/*! Text properties for H4 text (caption 4th order).*/
	const TextSizes&	H4() const { return m_textH4; }

	/*! Text properties for small text.*/
	const TextSizes&	small() const { return m_textSmall; }

private:
	QFont			m_normalFont;		///< Font used for normal text.
//	QTextDocument*	m_textDocument;		///< Text document object used for render text (HTML).
	TextSizes		m_textNormal;		///< Sizes of normal text (without specifier).
	TextSizes		m_textH1;			///< Sizes of H1 tagged text.
	TextSizes		m_textH2;			///< Sizes of H2 tagged text.
	TextSizes		m_textH3;			///< Sizes of H3 tagged text.
	TextSizes		m_textH4;			///< Sizes of H4 tagged text.
	TextSizes		m_textSmall;		///< Sizes of Small tagged text.
};

} // namespace QtExt {

#endif // QtExt_TextPropertiesH
