
## Description

vinagre-applet is a replacement of the original Vinagre applet
designed for the Mate desktop 1.8 based on Gnome 2.


## Requirements

vinagre-applet required libxml2 to parse vinagre xml bookmarks.


## Building howto

To build vinagre-applet some development libraries are required :
* libmate-panel-applet
* libgtk2.0
* libglib2.0
* libxml2

Use autotools to generate configure and Makefile files :
```
aclocal
autoconf
autoheader
automake -a -c
```

Then you can build vinagre-applet executable and install it :
```
./configure --prefix=/usr
make
make install
```
or create a debian binary package :
```
dpkg-buildpackage -b -us -uc
```


## Licenses

vinagre-applet is a free software written by Jean Diraison in 2015.
It is covered by version 3 of the GNU General Public License.
vinagre-applet is designed to replace the default Vinagre applet
for the Mate desktop, based on Gnome2.

Vinagre is a remote desktop client for VNC, ssh and RDP protocols
created and developed by Jonh Wendell and David King.

