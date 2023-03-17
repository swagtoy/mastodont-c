workspace("Mastodont");
configurations { "Debug", "Release" };

-- BEGIN Mastodont project
project("Mastodont");
kind("StaticLib");
language("C");
files { "include/*.h", "src/*.c" };
includedirs { "include/" };

filter { "system:linux or bsd or macosx", "action:gmake" }
	linkoptions { "`curl-config --libs`" }
	linkoptions { "`pkg-config --libs cjson`" }

filter { "configurations:Debug" };
	defines { "DEBUG" };
	symbols("On");

filter { "configurations:Release" };
	defines { "NDEBUG" };
	optimize("On");
-- END Mastodont-c
