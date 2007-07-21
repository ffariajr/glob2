def establish_options(env):
    opts = Options('options_cache.py')
    opts.Add("CXXFLAGS", "Manually add to the CXXFLAGS", "-g -pg")
    opts.Add(BoolOption("release", "Build for release", 0))
    opts.Add(BoolOption("mingw", "Build with mingw enabled", 0))
    Help(opts.GenerateHelpText(env))
    opts.Update(env)
    opts.Save('options_cache.py', env)


class Configuration:
    """Handles the config.h file"""
    def __init__(self):
        self.f = open("config.h", 'w')
        self.f.write("// config.h. Generated by scons\n")
        self.f.write("\n")
    def add(self, variable, doc, value=""):
        self.f.write("// %s\n" % doc)
        self.f.write("#define %s %s\n" % (variable, value))
        self.f.write("\n")

def configure(env):
    """Configures glob2"""
    conf = Configure(env)
    configfile = Configuration()
    configfile.add("PACKAGE", "Name of package", "glob2")
    configfile.add("PACKAGE_BUGREPORT", "Define to the address where bug reports for this package should be sent.", "glob2-devel@nongnu.org")
    configfile.add("PACKAGE_DATA_DIR", "data directory", "/usr/local/share/glob2")
    configfile.add("PACKAGE_NAME", "Define to the full name of this package.", "Globulation 2")
    configfile.add("PACKAGE_TARNAME", "Define to the one symbol short name of this package.", "glob2")
    configfile.add("PACKAGE_VERSION", "Define to the version of this package.", env["VERSION"])
    configfile.add("AUDIO_RECORDER_OSS", "Set the audio input type to OSS; the UNIX Open Sound System")
    #Simple checks for required libraries
    if not conf.CheckLib('SDL'):
        print "Could not find libSDL"
        Exit(1)
    if not conf.CheckLib('SDL_ttf'):
        print "Could not find libSDL_ttf"
        Exit(1)
    if not conf.CheckLib('SDL_image'):
        print "Could not find libSDL_image"
        Exit(1)
    if not conf.CheckLib('SDL_net'):
        print "Could not find libSDL_net"
        Exit(1)
    if not conf.CheckLib('speex') or not conf.CheckCHeader('speex/speex.h'):
        print "Could not find libspeex or could not find 'speex/speex.h'"
        Exit(1)
    if not conf.CheckLib('vorbisfile'):
        print "Could not find libvorbisfile to link against"
        Exit(1)
    if not conf.CheckLib('z') or not conf.CheckCHeader('zlib.h'):
        print "Could not find zlib.h"
        Exit(1)
    if not conf.CheckLib('boost_thread') or not conf.CheckCXXHeader('boost/thread/thread.hpp'):
        print "Could not find libboost_thread or boost/thread/thread.hpp"
        Exit(1)
    if not conf.CheckCXXHeader('boost/shared_ptr.hpp'):
        print "Could not find boost/shared_ptr.hpp"
        Exit(1)
    if not conf.CheckCXXHeader('boost/tuple/tuple.hpp'):
        print "Could not find boost/tuple/tuple.hpp"
        Exit(1)
    if not conf.CheckCXXHeader('boost/tuple/tuple_comparison.hpp'):
        print "Could not find boost/tuple/tuple_comparison.hpp"
        Exit(1)
    if not conf.CheckCXXHeader('boost/logic/tribool.hpp'):
        print "Could not find boost/logic/tribool.hpp"
        Exit(1)
    if not conf.CheckCXXHeader('boost/lexical_cast.hpp'):
        print "Could not find boost/lexical_cast.hpp"
        Exit(1)
     
    #Do checks for OpenGL, which is different on every system
    gl_libraries = []
    if conf.CheckLib('GL') and conf.CheckCHeader('GL/gl.h'):
        gl_libraries.append("GL")
    elif conf.CheckLib('GL') and conf.CheckCHeader('OpenGL/gl.h'):
        gl_libraries.append("GL")
    elif conf.CheckLib('opengl32') and conf.CheckCHeader('GL/gl.h'):
        gl_libraries.append("opengl32")
    else:
        print "Could not find libGL or opengl32, or could not find GL/gl.h or OpenGL/gl.h"
        Exit(1)
    
    #Do checks for GLU, which is different on every system
    if conf.CheckLib('GLU') and conf.CheckCHeader("GL/glu.h"):
        gl_libraries.append("GLU")
    elif conf.CheckLib('GLU') and conf.CheckCHeader("OpenGL/glu.h"):
        gl_libraries.append("GLU")
    elif conf.CheckLib('glu32') and conf.CheckCHeader('GL/glu.h'):
        gl_libraries.append("glu32")
    else:
        print "Could not find libGLU or glu32, or could not find GL/glu.h or OpenGL/glu.h"
        Exit(1)
    
    if gl_libraries:
        configfile.add("HAVE_OPENGL ", "Defined when OpenGL support is present and compiled")
        env.Append(LIBS=gl_libraries)
    
    #Do checks for fribidi
    if conf.CheckLib('fribidi') and conf.CheckCHeader('fribidi/fribidi.h'):
        configfile.add("HAVE_FRIBIDI ", "Defined when FRIBIDI support is present and compiled")
        env.Append(LIBS=['fribidi'])



def main():
    env = Environment()
    env["VERSION"] = "0.8.24"
    establish_options(env)
    configure(env)
    env.Append(CPPPATH='#libgag/include')
    env.Append(LIBPATH='#libgag/src')
    if env['release']:
        env.Append(CXXFLAGS='-O3')
        env.Append(LINKFLAGS='')
    if env['mingw']:
        env.Append(CXXFLAGS="-ISDL")
    env.Append(LIBS=['SDL_ttf', 'SDL_image', 'SDL_net', 'speex', 'vorbisfile', 'boost_thread'])
    env.ParseConfig("sdl-config --cflags")
    env.ParseConfig("sdl-config --libs")
    Export('env')
    env["TARFILE"] = env.Dir("#").abspath + "/glob2-" + env["VERSION"] + ".tar"
    env.Tar(env["TARFILE"], Split("AUTHORS COPYING Doxyfile INSTALL mkdata mkdist mkmap README README.hg SConstruct syncdata syncmaps TODO"))
    env.Alias("dist", env["TARFILE"])
    SConscript("campaigns/SConscript")
    SConscript("data/SConscript")
    SConscript("gnupg/SConscript")
    SConscript("libgag/SConscript")
    SConscript("libusl/SConscript")
    SConscript("maps/SConscript")
    SConscript("scripts/SConscript")
    SConscript("src/SConscript")
    SConscript("tools/SConscript")
    
main()
