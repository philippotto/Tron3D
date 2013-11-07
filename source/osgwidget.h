#pragma once

#include <QWidget>

#include <osgQt/GraphicsWindowQt>

class QGridLayout;

namespace troen {

    /**
     * @brief Qt Widget used to display the OSG Viewer.
     */
    class OSGWidget : public QWidget
    {
        Q_OBJECT

    public:
        explicit OSGWidget(QWidget* parent = NULL);
        virtual ~OSGWidget();

        inline osg::GraphicsContext* graphicsContext()
			{ return m_graphicsWindow.get(); }

    private:
        osgQt::GraphicsWindowQt* createGraphicsContext();

        QGridLayout*                            m_layout;
        osgQt::GLWidget*                        m_glWidget;
        osg::ref_ptr<osgQt::GraphicsWindowQt>   m_graphicsWindow;
    };

}