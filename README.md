#    Mastodont-c

 - *NOTE:* API is not stable yet. Please be patient.

Mastodont-c is a C library to access the Mastodon API and it's variants
(Pleroma, Akkoma, Soapbox).

It was written in C due to it's portability. It's designed to be used in
Treebird, but it can be used in other programs as well. In fact, there are
tests specifically designed to showcase it's power

#    Build system

Mastodont-c uses a pretty quirky build system called Premake5 mainly just
 to make me happy and for portability. I plan on writing some neat little 
 Java bindings for this library someday to see if I can expand it onto other
 platforms :-)
 
#### On UNIX-like systems...

    $ premake5 gmake

(Silly systems can use `--os bsd` to force it)

#    Documentation

Documentation is essentially non-existant, but this will improve over time.
