#ifndef DWD_CHECKBOX_H
#define DWD_CHECKBOX_H

#include <QCheckBox>

class CheckBoxHelper : public QCheckBox
{
Q_OBJECT

public:
	CheckBoxHelper( const int &row, const int &col) :
		m_row(row),
		m_col(col)
	{
		connect(this, SIGNAL(stateChanged(int)), this, SLOT(doEmitStateChanged(int)));
	}

	virtual ~CheckBoxHelper() {}

	int m_row;
	int m_col;

private slots:
	// Emits the new signal
	void doEmitStateChanged(int i) {
		emit checkBoxChanged(i, m_row, m_col);
	}

signals:
	void checkBoxChanged(int, int row, int col);


};

#endif // DWD_CHECKBOX_H
