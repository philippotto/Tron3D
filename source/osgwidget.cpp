#include "osgwidget.h"

#include <QGridLayout>
#include <QCoreApplication>
#include <QEvent>

using namespace troen;

OSGWidget::OSGWidget(QWidget* parent) :
  QWidget(parent)
{
    m_layout = new QGridLayout(this);
    m_layout->setContentsMargins(1, 1, 1, 1);

    m_graphicsWindow = createGraphicsContext();
    if (m_graphicsWindow.valid()) {
        m_glWidget = m_graphicsWindow->getGLWidget();
        m_glWidget->setForwardKeyEvents(true);
        m_layout->addWidget(m_glWidget);
        setFocusProxy(m_glWidget);
    }

    setLayout(m_layout);
    setMouseTracking(true);
//    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
}

OSGWidget::~OSGWidget() {
}

osgQt::GraphicsWindowQt* OSGWidget::createGraphicsContext()
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->windowDecoration = true;
    traits->x = 0;
    traits->y = 0;
    traits->width = width();
    traits->height = height();
    traits->doubleBuffer = true;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();

    return new osgQt::GraphicsWindowQt(traits.get());
}
