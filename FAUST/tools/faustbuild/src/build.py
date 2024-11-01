import subprocess
from pathlib import Path

class Build:
    def __init__(self, output_folder_path, post_build_operations=None):
        self.output_folder_path = Path(output_folder_path)
        self.post_build_operations = post_build_operations or []

    def build(self, faust_file):
        # Create the output folder if it doesn't exist
        self.output_folder_path.mkdir(parents=True, exist_ok=True)

        # Read the FAUST code from the file
        with faust_file.open("r") as f:
            faust_code = f.read()

        # Generate the output file path
        output_file_path = self.output_folder_path / f"{faust_file.stem}.{self.output_format}"

        # Run the faust command to generate the output file
        subprocess.run(["faust", "-lang", self.output_format, faust_file, "-o", str(output_file_path), "-cn", faust_file.stem])

        # Perform post-build operations if necessary
        for operation in self.post_build_operations:
            operation(self.output_folder_path)