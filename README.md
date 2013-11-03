GP2013 - Awesome Game by Team Awesome
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

Set up correct Working Directory:
- in MSVC goto DEBUG->properties->ConfigurationProperties->Debugging and set WorkingDirectory to ".." (withough the quotes)