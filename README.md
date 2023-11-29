# idleterm

I made idleterm to have something similar to [unimatrix](https://github.com/will8211/unimatrix) but made in C++ and a lot simpler, because I wanted an idle terminal screen running on very low-end devices. It currently is only tested on Linux, so feel free to make a windows version and talk to me if you want it to be on the main repo.
## Dependencies

The libx11-dev headers and libxrandr-dev packages, as well as g++ and libg++ to compile the code.

Ubuntu:
```bash
sudo apt install libx11-dev libxrandr-dev g++ libg++
```
Arch: if you use X11 and have Xrandr installed, the libs should be there.
```
sudo pacman -Sy gcc xorg-server xorg-xrandr --needed
```
## Installation

On any linux distro, just git clone the repo and run the install script (uses sudo to install it on the right folder)

```bash
  git clone https://github.com/augustopereiratx/idleterm
  cd idleterm
  ./install.sh
```

To run it, run `idleterm`. I recommend reading the help prompt by using the `-h` flag.
## Screenshots

![Program running in Kitty terminal](assets/1.png)
![Program running in Kitty terminal again](assets/2.png)
