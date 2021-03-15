#ifndef QtExt_CoordinateIndexEditH
#define QtExt_CoordinateIndexEditH

#include <QWidget>
#include <QVector>

#include "QtExt_ValidatingInputBase.h"

namespace QtExt {

namespace Ui {
class CoordinateIndexEdit;
}

/*! \brief CoordinateIndexEdit allows to edit a coordinate vector by value and/or index.
	The ccordinate vector must be set before use. The vector must be sorted.
	The edit widget consists of two combined edit fields.
	One spin edit for the indices and one ValidatingLineEdit for the coordinate values. For properties of this edit field \sa QtExt::ValidatingLineEdit .
*/
class CoordinateIndexEdit : public QWidget
{
	Q_OBJECT

public:
	/*! Default c'tor. */
	explicit CoordinateIndexEdit(QWidget *parent = nullptr);

	/*! Standard destructor. Delete the ui.*/
	~CoordinateIndexEdit();

	/*! Initialises the internal coordinate vector.*/
	void set(const QVector<double>& coordinates);

	/*! Returns true if the input is a valid number and the number matches the validation rule.*/
	bool isValid() const;

	/*! Sets a new validator. Class will take ownership of this.
		Any existing validator will be deleted.
		Calls same function from base class ValidatingInputBase. Necessary in order to avoid abiguity with QLineEdit.
		\param validator New validator as derivation of ValidatorBase.
	*/
	void setValidator(ValidatorBase* validator);

	/*! Checks if the line edit contains a valid double number (regardless of range test).
		\param val The value is stored herein if the line edit's text can be parsed into a number.
		\return Returns true, if the line edit's text can be parsed into a number.
	*/
	bool isValidNumber(double & val) const;

	/*! Set the enabled state (overloaded to change background color).
		\param enabled Enabled state of widget.
	*/
	void setEnabled(bool enabled);

	/*! Set whether line edit is read only (overloaded to change background color).
		\param readOnly Read-only state of widget.
	*/
	void setReadOnly(bool readOnly);

	/*! Sets a double value as text for the edit field using the current format or formatter.
		\param value Value to be set.
		\sa setFormat(), setFormatter(), FormatterBase
	*/
	void setValue(double value);

	/*! Returns the current value of the line edit.
		Returns the last valid number that was entered in the line edit. If the line edit currently contains
		an invalid number, the last number that was accepted is returned.
		\note You should only call this function after isValid() returned true.
	*/
	double value() const;

	/*! Return the current index of the coordinate vector.*/
	int index() const;

	/*! Set the index to the given one and updates coordinate with value from internal vector at given position.
	   \param index Index in coordinate vector.
	*/
	void setIndex(unsigned int index);

protected:
	void changeEvent(QEvent *e);

signals:
	/*! Emits the result of the editing, but only if a result was entered correctly. */
	void editingFinishedSuccessfully();

private slots:
	/*! Is called then a change in the edit field is finished in order to check the values. */
	void onEditingFinished();

	/*! Is called then index in spin box is changed.*/
	void onIndexChanged(int index);

private:
	Ui::CoordinateIndexEdit *ui;
	QVector<double>	m_coordinates;
	bool			m_ascending;
	int				m_maxCoordinateIndex;
	int				m_minCoordinateIndex;

	/*! Return if the internal coordinate vector is valid.*/
	bool			validCoordinates() const;
};


} // namespace QtExt
#endif // QtExt_CoordinateIndexEditH
