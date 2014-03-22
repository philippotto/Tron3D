#include "shaders.h"
// OSG
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

using namespace troen;
using namespace shaders;


std::vector<osg::ref_ptr<osg::Program> > shaders::m_allShaderPrograms;

void shaders::reloadShaders()
{
	shaders::m_allShaderPrograms.resize(SHADER_NAME_COUNT);

	std::string deformShader = "BendedViews/shaders/deform_ws_bowl.vert";

	reloadShader(shaders::m_allShaderPrograms[DEFAULT], "source/shaders/default.frag", deformShader, "source/shaders/default.vert");
	reloadShader(shaders::m_allShaderPrograms[BIKE], "source/shaders/bike.frag", deformShader, "source/shaders/bike.vert");
	reloadShader(shaders::m_allShaderPrograms[GRID], "source/shaders/grid.frag", deformShader, "source/shaders/grid.vert");
	reloadShader(shaders::m_allShaderPrograms[GRID_NOREFLECTION], "source/shaders/grid_noreflection.frag", deformShader, "source/shaders/grid.vert");
	reloadShader(shaders::m_allShaderPrograms[FENCE], "source/shaders/fence.frag", deformShader, "source/shaders/fence.vert");
	reloadShader(shaders::m_allShaderPrograms[GBUFFER], "source/shaders/gbuffer.frag", "source/shaders/gbuffer.vert", "");
	reloadShader(shaders::m_allShaderPrograms[HBLUR], "source/shaders/Blur.frag", "source/shaders/HBlur.vert", "");
	reloadShader(shaders::m_allShaderPrograms[VBLUR], "source/shaders/Blur.frag", "source/shaders/VBlur.vert", "");
	reloadShader(shaders::m_allShaderPrograms[POST_PROCESSING], "source/shaders/postprocessing.frag", "", "");
	reloadShader(shaders::m_allShaderPrograms[OUTER_WALL], "source/shaders/outer_wall.frag", deformShader, "source/shaders/outer_wall.vert");
	reloadShader(shaders::m_allShaderPrograms[SELECT_GLOW_OBJECTS], "source/shaders/selectglowobjects.frag", "", "");
	reloadShader(shaders::m_allShaderPrograms[SKYDOME], "source/shaders/skydome.frag", "", "");
}

void shaders::reloadShader(
	osg::ref_ptr<osg::Program> & program,
	std::string fragmentFileName,
	std::string vertexFileName,
	std::string secondVertexFileName)
{
	// reload shader files, if necessary
	if (program)
	{
		loadShaderSource(program->getShader(0), fragmentFileName);

		if (vertexFileName != "")
		{
			loadShaderSource(program->getShader(1), vertexFileName);
		}

		if (secondVertexFileName != "")
			loadShaderSource(program->getShader(2), secondVertexFileName);

	}
	else
	{
		program = new osg::Program();

		osg::ref_ptr<osg::Shader> fragShader = new osg::Shader(osg::Shader::FRAGMENT);
		loadShaderSource(fragShader, fragmentFileName);
		program->addShader(fragShader);

		if (vertexFileName != "")
		{
			osg::ref_ptr<osg::Shader> vertShader = new osg::Shader(osg::Shader::VERTEX);
			loadShaderSource(vertShader, vertexFileName);
			program->addShader(vertShader);
		}

		if (secondVertexFileName != "")
		{
			osg::ref_ptr<osg::Shader> vertShader = new osg::Shader(osg::Shader::VERTEX);
			loadShaderSource(vertShader, secondVertexFileName);
			program->addShader(vertShader);

		}

		std::string mystr;
		mystr = std::string(osgDB::getStrippedName(fragmentFileName));

		program->setName(mystr);
	}
}

bool shaders::loadShaderSource(osg::Shader* obj, const std::string& fileName)
{
	std::string fqFileName = osgDB::findDataFile(fileName);
	if (fqFileName.length() == 0)
	{
		std::cout << "[TroenGame::abstractView]  File \"" << fileName << "\" not found." << std::endl;
		return false;
	}
	bool success = obj->loadShaderSourceFromFile(fqFileName.c_str());
	if (!success)
	{
		std::cout << "Couldn't load file: " << fileName << std::endl;
		return false;
	}
	else
	{
		return true;
	}
}

