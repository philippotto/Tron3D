GP2013 - Awesome Game
======

Audiovisual sex as a source of inspiration can be found here: http://www.youtube.com/watch?v=3GBLtnxxtzQ


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


Set up correct Working Directory:
- in MSVC goto DEBUG->properties->ConfigurationProperties->Debugging and set WorkingDirectory to ".." (without the quotes)

___________________

STYLE GUIDE

- use forward declarations & (reference) pointers to reduce compile time

- member variables start with m_

- includes in specifies order:
	(classname.h), qt, osg, bullet, fmod, ownHeaders
- do not "inherit" includes, always include all necessary headers

- use reference counting whereever possible (osg::ref_pr<>)

- use "#pragma once" include guard

- use namespaces for modules



Naming
- file names are all lowercase
- classes start with capital letter
