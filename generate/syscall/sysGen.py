import yaml

def parse_yaml(file_path):
    with open(file_path, 'r') as file:
        try:
            data = yaml.safe_load(file)
            return data
        except yaml.YAMLError as e:
            print(f"Error parsing YAML file: {e}")
            return None

if __name__ == "__main__":
    yaml_file = 'syscalls.yaml'
    parsed_data = parse_yaml(yaml_file)

    if parsed_data:
        print("Parsed YAML Data:")
        print(parsed_data)