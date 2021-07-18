from distutils.core import setup, Extension

def main():
    setup(name="pyrout",
          version="1.0.0",
          description="Python interface for some RACROUT extensions.",
          author="The Wizard of z/OS",
          author_email="wizard@zdevops.com",
          ext_modules=[Extension("pyrout", ["pyrout.c"])])

if __name__ == "__main__":
    main()
