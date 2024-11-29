import gdown
import zipfile
import os

url = "https://drive.google.com/uc?id=1xaJ0OVjoQzGfQ-jNC9aA3Je_U-rV1yAf"
downloaded_file = "riscv-tests-64.zip"
extract_to = "riscv-tests-64"

print("Downloading file...")
gdown.download(url, downloaded_file, quiet=False)

print(f"File downloaded successfully as {downloaded_file}")
if downloaded_file.endswith('.zip'):
    if not os.path.exists(extract_to):
        os.makedirs(extract_to)

    print(f"Unzipping {downloaded_file} to {extract_to}...")
    with zipfile.ZipFile(downloaded_file, 'r') as zip_ref:
        zip_ref.extractall(extract_to)

    print(f"Extraction completed. Files are saved to {extract_to}")
else:
    print("Downloaded file is not a ZIP file.")
