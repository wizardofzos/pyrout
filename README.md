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

1. Python and the 'intermediate' modules, as well as the pyrout.so need to be "program controlled" 
2. python setup.py install does not work as expected
3. There's no lxc_echocmd on z/OS


Steps to install:
