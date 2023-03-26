premake.gcc.cc = os.getenv("CC") or 'cc';

solution "mastodont-c";
configurations { "Debug", "Release" };

-- BEGIN Mastodont project
project("mastodont");
kind("StaticLib");
language("C");
files { "include/*.h", "src/*.c" };
includedirs { "include/" };

configuration { "linux", "bsd", "gmake" };
linkoptions { "`curl-config --libs`" };
linkoptions { "`pkg-config --libs cjson`" };
	

configuration { "Debug" };
defines { "DEBUG" };
flags("Symbols");

configuration { "Release" };
defines { "NDEBUG" };
flags("Optimize");
-- END Mastodont-c

local prefix = os.getenv("PREFIX") or "/usr/local";
local pkgconfig_path = os.getenv("PKG_CONFIG_PATH") or
                       (os.isdir(prefix .. "/pkgconfig") and prefix .. "/pkgconfig") or
                       prefix .. "/share/pkgconfig";

newaction {
	trigger = "install",
	description = "install the library",
	execute = function()
		os.copyfile("libmastodont.a", prefix .. "/lib");
		os.execute("install -d include/ " .. prefix .. "/include");
		os.mkdir(pkgconfig_path);
		os.copyfile("mastodont.pc", pkgconfig_path .. "/mastodont.pc");
	end
}
