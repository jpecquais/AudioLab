import sys
from pathlib import Path
from src.jsfxbuild import JsfxBuild
from src.cppbuild import CppBuild

if __name__ == "__main__":
    # Get the FAUST code file from the command-line argument
    faust_file = Path(sys.argv[1])
    output_format = "jsfx"

    common_output_path = "build/"

    if "-to" in sys.argv:
        output_format = sys.argv[sys.argv.index("-to") + 1]
    if "-o" in sys.argv:
        common_output_path = sys.argv[sys.argv.index("-o") + 1]

    # Create the appropriate build object based on the output format
    if output_format == "cpp":
        build_obj = CppBuild(common_output_path+"cpp")
    elif output_format == "jsfx":
        build_obj = JsfxBuild(common_output_path+"jsfx/Effects")
    else:
        raise ValueError(f"Unsupported output format: {output_format}")

    # Build the FAUST code
    build_obj.build(faust_file)
