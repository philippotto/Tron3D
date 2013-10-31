#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <osgQt/GraphicsWindowQt>
#include <QVBoxLayout>

class ViewerWidget : public QWidget
{
	public:
		ViewerWidget(osg::Camera* camera, osg::Node* scene);

	protected:
		virtual void paintEvent(QPaintEvent* event)
		{
			_viewer.frame();
		}
		osgViewer::Viewer _viewer;
		QTimer _timer;
};