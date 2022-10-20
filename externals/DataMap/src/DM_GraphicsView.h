#ifndef DM_GraphicsViewH
#define DM_GraphicsViewH

#include <QGraphicsView>

#include "DM_Scene.h"

namespace DM {

class GraphicsView : public QGraphicsView
{
	Q_OBJECT
public:

	explicit GraphicsView(QWidget *parent = nullptr);

	~GraphicsView();

protected:
	void wheelEvent(QWheelEvent * event) override;
	void resizeEvent(QResizeEvent * event) override;



};

}
#endif // DWDGRAPHICSVIEW_H
