/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_ValueInputComboBoxH
#define QtExt_ValueInputComboBoxH

#include <QComboBox>

#include <memory>

#include "QtExt_ValidatingInputBase.h"

#include <IBK_Parameter.h>

namespace QtExt {

/*! A combo box for entering values and also selecting values from a choice of options.
	This combo box essentially works like the validating line edit (at least when it's fully implemented)
	and also offers the feature that selecting a string from the combo box choices will
	place the double value stored as item-data into the combo box line edit instead of the string.

	Currently set QLocale() will be used for converting numbers to/from the line edit.

	\code
	// ** Setup **
	// 1. First set limits of the acceptable value range, the last two arguments are optional and
	//	  can be used to specify whether the min and max values themselves are part of the acceptable range.
	combo->setup(minVal, maxVal, toolTip, false, true);
	//    then populate the combo box with items, mind to add the actual double value as item data
	combo->addItem(tr("0.2 - My first choice", 0.2);
	combo->addItem(tr("0.4 - My second choice", 0.4);
	combo->addItem(tr("default value", 1.0);

	// 2. Connect to the editingFinishedSuccessfully() signal, which is only emitted when
	// the user has changed the line edit and/or the combo box and the result is a valid entry
	// with respect to the min/max values.
	connect(combo, SIGNAL(editingFinishedSuccessfully()), this SLOT(onEditingFinishedSuccessfully()));
	\endcode

	The following code illustrates the usual work flow with the value input combo box
	\code
	// Set any value either by using
	combo->setValue(15);

	// or by setting the value directly via setText()
	combo->setCurrentText(QString("%L1").arg(15, 0, 'f', 2));

	// or if you have a parameter, use setFromParameter()
	combo->setFromParameter(p, unitToConvertTo, 15);

	// when user has finished, the value can be retrieved
	if (combo->isValid()) {
		double value = combo->value();
		// ...
	}
	\endcode
*/
class ValueInputComboBox : public QComboBox, public ValidatingInputBase {
	Q_OBJECT
public:
	/*! Default c'tor. */
	explicit ValueInputComboBox(QWidget *parent = 0);

	/*! Returns true if the input is a valid number and the number matches the validation rule.*/
	bool isValid() const;

	/*! Checks if the line edit contains a valid double number (regardless of range test).
		\param val The value is stored herein if the line edit's text can be parsed into a number.
		\return Returns true, if the line edit's text can be parsed into a number.
	*/
	bool isValidNumber(double & val) const;

	/*! Set the enabled state (overloaded to change background color).
		\param enabled Enabled state of widget.
	*/
	void setEnabled(bool enabled);

	/*! If parameter is not empty (name not empty), the value of the parameter is
		converted into the requested IO unit and written in the combo box, otherwise
		the default value is written into the combo box.
		\warning Function throws an exception if IO_unit of parameter and unit are not convertible.
	*/
	void setFromParameter(const IBK::Parameter & p, const std::string & unit, double defaultValue);

	/*! Sets a double value as text for the edit field using the current format or formatter.
		\param value Value to be set.
		\sa setFormat(), setFormatter(), FormatterBase
	*/
	void setValue(double value);

	/*! This function is for interactive checking - it tests the entered value for correct number
		format and whether it is in the allowed value range. In case of any error
		a critical message box pops up and the function returns false.
		If all checks out ok, the function silently returns true.
		Use this in the input check function of modal dialogs.
	*/
	bool check(double minVal, double maxVal, bool minValAllowed, bool maxValAllowed,
			   const QString & outOfRangeMessage);

	/*! Returns the current number entered in the combo box.
		\warning Call this function only if the check() function has evaluated with true beforehand,
		otherwise the outcome is undefined (0 is returned for invalid input).
	*/
	double value() const;

signals:

	/*! Emits the result of the editing, but only if a result was entered correctly. */
	void editingFinishedSuccessfully();

private slots:
	/*! Overloaded to ensure validation is applied when setting text. */
	void setCurrentText(const QString& text);

	/*! Evaluates input and colors line edit accordingly. */
	void onEditingFinished();

	/*! Evaluates input and colors line edit accordingly. */
	void onCurrentTextChanged(const QString & arg);

	/*! Triggered when user selects an option from the combo box and writes
		the double value stored as item data into the combo box instead.
	*/
	void onActivated(int index);
};

} // namespace QtExt

#endif // QtExt_ValueInputComboBoxH
