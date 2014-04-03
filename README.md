Trön
======

Trön is an action packed 3D-remake of the classic Armagetron
featuring polished graphics, customizable maps, powerups, scriptable AI, multiplayer and many more!


![Trön](data/screenshots/ramp.png)
![Bended Views](data/screenshots/bended.png)


##Project Setup

Set up OSG:
- Copy the compiled OpenSceneGraph folder to C:\Program Files\ and create a System Variable "OSG_DIR" with the value: C:\Program Files\OpenSceneGraph
- add %OSG_DIR%\lib; %OSG_DIR%\bin to your PATH variable

Set up FMOD:
- Copy the fmod folder (in our Dropbox) to %ProjectDir%\3rdParty\fmod
- add the environment variable FMOD_DIR to this directory
- add %FMOD_DIR%\fmoddesignerapi\api; %FMOD_DIR%\api; to your PATH

Set up BULLET_PHYSICS:
- Just copy the BULLET_PHYSICS folder (in our Dropbox) to %ProjectDir%\3rdParty\BULLET_PHYSICS
- no setting of any evironment variable or PATH variable necessary

Set up HIDAPI
- Copy the hidapi folder (in our Dropbox) to
%ProjectDir%\3rdParty\
- add %ProjectDir%\3rdParty\hidapi to your PATH

Set up V8 & Scriptzeug
- Copy the scriptzeug and v8 folder (in our Dropbox) to 
%ProjectDir%\3rdParty\
- create the environment variable %V8_ROOT% and set it to %ProjectDir%\3rdParty\v8
- add the following to your path:
- %V8_ROOT%\bin
- %ProjectDir%\3rdParty\scriptzeug\lib

Set up Oculus Rift
- Copy the LibOVR (in our Dropbox) to
%ProjectDir%\3rdParty\LibOVR
- create the environment variable %OCULUS_ROOT% and set it to %ProjectDir%\3rdParty\LibOVR

Set up correct Working Directory:
- in MSVC goto DEBUG->properties->ConfigurationProperties->Debugging and set WorkingDirectory to ".." (without the quotes)



##Styleguide

- use forward declarations & (reference) pointers to reduce compile time

- member variables start with m_

- includes in specified order:
	(classname.h), qt, osg, bullet, fmod, ownHeaders
- do not "inherit" includes, always include all necessary headers

- use reference counting whereever possible (osg::ref_pr<>, std::shared_ptr<>)

- use "#pragma once" include guard

- use namespaces for modules

- file names are all lowercase

- classes start with capital letter
