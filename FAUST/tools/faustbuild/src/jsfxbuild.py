from src.build import Build
import subprocess
from pathlib import Path

class JsfxBuild(Build):
    def __init__(self, output_folder_path):
        super().__init__(output_folder_path, post_build_operations=[self.create_reaper_session])
        self.output_format = "jsfx"

    def create_reaper_session(self, output_folder_path):
        # Check if the REAPER session file exists
        reaper_session_path = Path(output_folder_path).parent / "testing_session.rpp"

        if not reaper_session_path.exists():
            # Create a new REAPER session file using the `touch` command
            subprocess.run(["touch", str(reaper_session_path)])

            with open(reaper_session_path, "w") as f:
                f.write("<REAPER_PROJECT 0.1 \"7.26/macOS-arm64\" 1730469999\n>")