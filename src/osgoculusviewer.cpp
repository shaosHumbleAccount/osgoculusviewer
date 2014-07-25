/*
 * main.cpp
 *
 *  Created on: Jul 03, 2013
 *      Author: Bjorn Blissing
 */

#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include "oculuseventhandler.h"
#include "oculusviewconfig.h"


int main( int argc, char** argv )
{
	// use an ArgumentParser object to manage the program arguments.
	osg::ArgumentParser arguments(&argc,argv);
	// read the scene from the list of file specified command line arguments.
	osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFiles(arguments);

	// if not loaded assume no arguments passed in, try use default cow model instead.
	if (!loadedModel) loadedModel = osgDB::readNodeFile("C:\\kb\\projects\\point_gui\\trunk\\res\\robot_model\\LBR4-DLR\\dlr-lwr-3.iv");

	// Still no loaded model, then exit
	if (!loadedModel) return 0;

	// Subtract at least one bit of the node mask to disable rendering for main camera
	osg::Node::NodeMask sceneNodeMask = loadedModel->getNodeMask() & ~0x1;
	loadedModel->setNodeMask(sceneNodeMask);
		 
	// Create Oculus View Config
	osg::ref_ptr<OculusViewConfig> oculusViewConfig = new OculusViewConfig;
	oculusViewConfig->setScreenNum(1);
	// Set the node mask used for scene
	oculusViewConfig->setSceneNodeMask(sceneNodeMask);
	// Create viewer
	osgViewer::Viewer viewer(arguments);
	// Add statistics handler
	viewer.addEventHandler(new osgViewer::StatsHandler);
	// Add Oculus keyboard handler
	viewer.addEventHandler(new OculusEventHandler(oculusViewConfig));
	// Apply view config
	viewer.apply(oculusViewConfig);
	// Add loaded model to viewer
	viewer.setSceneData(loadedModel);

	viewer.setUpViewOnSingleScreen(1u);

	osgGA::TrackballManipulator* trackballCamMan = new osgGA::TrackballManipulator;
	trackballCamMan->setTransformation(osg::Vec3d(2,2,2),
                                       osg::Vec3d(0,0,0),
                                       osg::Vec3d(0,0,1));
    viewer.setCameraManipulator( trackballCamMan, false );

	// Start Viewer
	return viewer.run();
}
