#include "abstractview.h"

using namespace troen;

AbstractView::AbstractView()
{
	m_node = new osg::Group();
}

osg::ref_ptr<osg::Group> AbstractView::getNode()
{
	return m_node;
}

void AbstractView::update()
{}