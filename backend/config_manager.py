class ConfigManager(object):
    def __init__(self, config_file_path: str):
        self.config_file_path = config_file_path
    
    def get_config(self) -> dict[str, str]:
        config = {}
        with open(self.config_file_path, "r") as file:
            line = file.readline()
            key, val = line.split(",")
            config[key] = val
        return config
    
    def set_config(self, keys: list[str], values: list[str]):
        config = self.get_config()
        for i, key in enumerate(keys):
            config[key] = values[i]
        
        with open(self.config_file_path, "w") as file:
            formatted_config = [f"{key},{value}" for key, value in config.items()]
            file.write("\n".join(formatted_config))

    
    