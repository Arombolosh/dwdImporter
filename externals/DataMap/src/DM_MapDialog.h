#ifndef DM_MapDialogH
#define DM_MapDialogH

#include <QDialog>

#include "DM_GraphicsView.h"

namespace DM {

namespace Ui {
class MapDialog;
}

class MapDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MapDialog(QWidget *parent = nullptr);
    ~MapDialog();

	/*! Pointer to Scene. */
	DM::Scene				*m_scene = nullptr;


private:
	Ui::MapDialog			*m_ui;

};

} // namespace DM
#endif // DM_MapDialogH
