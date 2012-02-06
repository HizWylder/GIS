http://wiki.phoenixviewer.com/doku.php?id=fs_compiling_firestorm_32bit_ubuntu#fmod_using_autobuild

/** install autobuild  */
hg clone http://hg.secondlife.com/autobuild/


/** Fmod using autobuild */
hg clone https://bitbucket.org/lindenlab/3p-fmod/
cd 3p-fmod
autobuild build -c default
autobuild package

wrote  /home/{user}user/src/3p-fmod/fmod-3.75-linux-{yyyymmdd}.tar.bz2
md5    0396f1cc697d6a24e09e8e4ba6cffb90

cd ~/src/GIS 
cp autobuild.xml autobuild.xml.bk

autobuild installables edit fmod platform=linux hash=<md5 value> url=file:///<fmod path>

For example: 
autobuild installables edit fmod platform=linux hash=0396f1cc697d6a24e09e8e4ba6cffb90 url=file:///home/user/src/3p-fmod/fmod-3.75-linux-20110131.tar.bz2


/** DO THE SAME THING FOR llconvexdecompositionstub */
git clone git@github.com:HizWylder/llconvexdecomposition.git


autobuild.sh configure -c Release -- -DLL_TESTS:BOOL=FALSE -DFMOD:BOOL=TRUE -DUSE_KDU:BOOL=FALSE

CXX="ccache g++" autobuild.sh build -c Release --no-configure

