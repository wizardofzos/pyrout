# pyrout

## Usage

    from pyrout import verify
    if verify("IBMUSER","SYS1"):
        print("Authenticated")
    else:
        print("Bad user/pass")

## Installation

Installation on z/OS is not 100% straightforward... (is it ever?)

Issues:

1. Python and the 'intermediate' modules, as well as the pyrout.so need to be "program controlled". Can't embed those 'extattr +p' steps in the setup.py?
3. python setup.py install does not work as expected
4. There's no lxc_echocmd on z/OS


Steps to install:

Assuming you already have python (3.x) running on your z/OS

    git clone git@github.com:wizardofzos/pyrout.git
    cd pyrout
    CC=c99 pythyon setup.py install
    
So far, so good. However, the installer will barf with a "[Errno 129] EDC5129I No such file or directory. (errno2=0x053B006C)", because that xlc_echocmd is not available. Softlinking it to the /bin/xlc does nto seem to do the trick either because the "-o /path/to/resulting/module.so" needs to be at the front of the command?

In our example we get the following output:

    (base) IBMUSER:/prj/repos/pyrout: >CC=c99 python setup.py build
    running build
    running build_ext
    build_ext run self.libraries=[]
    build_ext run self.link_objects=['/zdo/conda/lib/libpython3.7m.x']
    building 'pyrout' extension
    creating build
    creating build/temp.zos-04.00-1090-3.7
    c99 -DNDEBUG -O -qdll -qexportall -qascii -q64 -qnocse -qgonum -qasm -qbitfield=signed -qtarget=zosv2r2 -qarch=10 -qtune=12 -O3 -qstrict -qfloat=ieee:nomaf -D__MV17195__ -D_XOPEN_SOURCE=600 -qlanglvl=extc1x -L/zdo/conda/lib -qdll -qexportall -qascii -q64 -qnocse -qgonum -qasm -qbitfield=signed -qtarget=zosv2r2 -qarch=10 -qtune=12 -O3 -qstrict -qfloat=ieee:nomaf -D__MV17195__ -D_XOPEN_SOURCE=600 -qlanglvl=extc1x -L/zdo/conda/lib -qdll -I/zdo/conda/include/python3.7m -c pyrout.c -o build/temp.zos-04.00-1090-3.7/pyrout.o
    WARNING CCN3236 /usr/include/unistd.h:1169  Macro name _POSIX_THREADS has been redefined.
    before link_shared_object: libraries=[]
    distutils link libraries=[] library_dirs=[] runtime_library_dirs=[]
    distutils link libraries=[] library_dirs=[] runtime_library_dirs=[]
    distutils link lib_opts=[]
    creating build/lib.zos-04.00-1090-3.7
    xlc_echocmd -qdll -qdll -qexportall -qascii -q64 -qnocse -qgonum -qasm -qbitfield=signed -qtarget=zosv2r2 -qarch=10 -qtune=12 -O3 -qstrict -qfloat=ieee:nomaf -L/zdo/conda/lib -qdll -qexportall -qascii -q64 -qnocse -qgonum -qasm -qbitfield=signed -qtarget=zosv2r2 -qarch=10 -qtune=12 -O3 -qstrict -qfloat=ieee:nomaf -L/zdo/conda/lib build/temp.zos-04.00-1090-3.7/pyrout.o /zdo/conda/lib/libpython3.7m.x -o build/lib.zos-04.00-1090-3.7/pyrout.so
    error: [Errno 129] EDC5129I No such file or directory. (errno2=0x053B006C)

From this take that last line: (split across lines for readability)

    xlc_echocmd -qdll -qdll -qexportall -qascii -q64 -qnocse -qgonum -qasm -qbitfield=signed -qtarget=zosv2r2 -qarch=10 -qtune=12 -O3 
                -qstrict -qfloat=ieee:nomaf -L/zdo/conda/lib -qdll -qexportall -qascii -q64 -qnocse -qgonum -qasm -qbitfield=signed 
                -qtarget=zosv2r2 -qarch=10 -qtune=12 -O3 -qstrict -qfloat=ieee:nomaf 
                -L/zdo/conda/lib build/temp.zos-04.00-1090-3.7/pyrout.o /zdo/conda/lib/libpython3.7m.x 
                -o build/lib.zos-04.00-1090-3.7/pyrout.so


Notice the "-o ..."  on the last line above...

Shift that to the front and change xlc_echocmd into xlc... The reulsting command then should be like the following:

    xlc -o build/lib.zos-04.00-1090-3.7/pyrout.so -qdll -qdll -qexportall -qascii -q64 -qnocse -qgonum -qasm -qbitfield=signed -qtarget=zosv2r2 -qarch=10 -qtune=12 -O3 -qstrict -qfloat=ieee:nomaf -L/zdo/conda/lib -qdll -qexportall -qascii -q64 -qnocse -qgonum -qasm -qbitfield=signed -qtarget=zosv2r2 -qarch=10 -qtune=12 -O3 -qstrict -qfloat=ieee:nomaf -L/zdo/conda/lib build/temp.zos-04.00-1090-3.7/pyrout.o /zdo/conda/lib/libpython3.7m.x
    
 If all goes well, it will result in a "FSUM3065 The LINKEDIT step ended with return code 4." And that's ok :)
 
 Next run the setup.py install again (again? yes again!)
 
    CC=c99 python setup.py install
    running install
    running build
    running build_ext
    build_ext run self.libraries=[]
    build_ext run self.link_objects=['/zdo/conda/lib/libpython3.7m.x']
    running install_lib
    copying build/lib.zos-04.00-1090-3.7/pyrout.so -> /zdo/conda/lib/python3.7/site-packages
    running install_egg_info
    Removing /zdo/conda/lib/python3.7/site-packages/pyrout-1.0.0-py3.7.egg-info
    Writing /zdo/conda/lib/python3.7/site-packages/pyrout-1.0.0-py3.7.egg-info
    
    
Then make sure that the python-execuatable, the pyrout.so and all 'intermediate' modules are extattr +p
In my case, when running the module from the interactive python these were:

- /zdo/conda/bin/python 
- /zdo/conda/lib/libpython3.7m.so 
- /zdo/conda/lib/python3.7/lib-dynload/_heapq.so
- /zdo/conda/lib/python3.7/lib-dynload/readline.so 
- /zdo/conda/lib/libreadline.so 
- /zdo/conda/lib/libncurses.so 
- /zdo/conda/lib/python3.7/site-packages/pyrout.so


## Other things

If you've any improvements, feel free to fork and fire a pull request. It would be amazing if that pesky xlc_echocmd issue would be fixed, I think it has to do with the 'setuptools' but then again: not sure and "It works for me like this" :)
The extattrs are very needed yet I'm curious to the implications on running the full python exec as PC?

Anyways. All stuff to build this on YOUR z/OS is in this repo.... enjoy and let me know if it runs/acts any different on your machine?



The actual C-code for the RACROUTE VERIFY password check via https://www.mail-archive.com/search?l=ibm-main@listserv.ua.edu&q=subject:%22Re%5C%3A+Code+to+verify+LOGON+password%22&o=newest&f=1
