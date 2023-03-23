solution "mastodont-c"
configurations { "Debug", "Release" };

-- BEGIN Mastodont project
project("mastodont");
kind("StaticLib");
language("C");
files { "include/*.h", "src/*.c" };
includedirs { "include/" };

configuration { "linux", "bsd", "gmake" }
	linkoptions { "`curl-config --libs`" }
	linkoptions { "`pkg-config --libs cjson`" }

configuration { "Debug" };
	defines { "DEBUG" };
	flags("Symbols");

configuration { "Release" };
	defines { "NDEBUG" };
	flags("Optimize");
-- END Mastodont-c
