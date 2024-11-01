from src.build import Build

class CppBuild(Build):
    def __init__(self, output_folder_path):
        super().__init__(output_folder_path)
        self.output_format = "cpp"