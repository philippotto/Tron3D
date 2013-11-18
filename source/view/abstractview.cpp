#include "abstractview.h"

using namespace troen;

AbstractView::AbstractView()
{
	
}

osg::ref_ptr<osg::Group> AbstractView::getNode()
{
	return m_node;
}