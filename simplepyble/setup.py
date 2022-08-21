import cmake_build_extension
import pathlib
import setuptools


# Get the long description from the README file
here = pathlib.Path(__file__).parent.resolve()
# long_description = (here / "README.md").read_text(encoding="utf-8")
long_description = "hello"

# The information here can also be placed in setup.cfg - better separation of
# logic and declaration, and simpler if you include description/version in a file.
setuptools.setup(
    name="simplepyble",
    version="0.0.5",  # ! Ensure it matches the intended release version!
    author="Kevin Dewald",
    author_email="kevin@dewald.me",
    description="The ultimate fully-fledged cross-platform BLE library, designed for simplicity and ease of use.",
    long_description=long_description,
    long_description_content_type='text/markdown',
    ext_modules=[
        cmake_build_extension.CMakeExtension(
            name="simplepyble",
            cmake_depends_on=["pybind11"],
            disable_editable=True,
        )
    ],
    cmdclass={
        "build_ext": cmake_build_extension.BuildExtension
    },
    zip_safe=False,
    extras_require={},
    python_requires=">=3.7",
    classifiers=[
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3 :: Only",
    ],
)
