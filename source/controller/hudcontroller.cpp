#include "hudcontroller.h"
// troen
#include "../view/hudview.h"
//#include "../model/hudmodel.h"

using namespace troen;

HUDController::HUDController()
{
	m_view = std::static_pointer_cast<HUDView>(std::make_shared<HUDView>());

	// TODO: is a model really necessary?
	// m_model = std::static_pointer_cast<HUDModel>(std::make_shared<HUDModel>());


}

void HUDController::resize(int width, int height)
{
	std::static_pointer_cast<HUDView>(m_view)->resize(width, height);
}

void HUDController::attachSceneToRadarCamera(osg::Group* scene)
{
	std::static_pointer_cast<HUDView>(m_view)->attachSceneToRadarCamera(scene);

}
