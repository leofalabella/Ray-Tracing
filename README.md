# Ray-Tracing-IN204
 IN204 final project


## Running the Code

### Prerequisites
The dependences to run this code are:
- Compiler `g++`
- `SFML` libraries

You can install the prerequisites with:

`make install`

### Compiling and Running

#### Using Make
To compile the code and generate the executable, simply run:

`make`

This will compile the source files (`main.cc`) using `g++` with optimization flags `-O3 -DNDEBUG`. The resulting executable will be named `out`.

To clean the compiled files and generated images, you can run:

`make clean`

#### Running the Executable
After compiling, you can run the executable using:

`./out > out.ppm`

This will generate two image files: `out.ppm` and `out.png` in the `images` directory.

#### Alternative: Running Everything
You can also compile, run, and clean in one go using:

`make all`

This command will clean any existing files, compile the code, run the executable, and move the generated image files to the `images` directory.

### Uninstallation
If you want to uninstall the SFML libraries, you can use:

`make uninstall`

