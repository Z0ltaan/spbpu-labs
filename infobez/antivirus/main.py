import os
import hashlib
import subprocess
import logging

logging.basicConfig(
  # level=logging.FATAL,
  datefmt= "%d-%m-%Y %H-%M-%S",
)

logger = logging.getLogger(__name__)


def get_txt_files_in_current_dir():
    excluded_files = {'HashList.txt', 'VirusHashList.txt', 'report.txt'}
    files = [f for f in os.listdir('.') if f.endswith('.txt') and os.path.isfile(f)]
    return [f for f in files if f not in excluded_files]

def compute_sha256(file_path):
    hash_sha256 = hashlib.sha256()
    try:
        with open(file_path, "rb") as f:
            for chunk in iter(lambda: f.read(4096), b""):
                hash_sha256.update(chunk)
        return hash_sha256.hexdigest()
    except FileNotFoundError:
        return None

def load_virus_hashes():
    virus_hashes = set()
    if os.path.exists("VirusHashList.txt"):
        with open("VirusHashList.txt", "r", encoding="utf-8") as f:
            for line in f:
                clean_line = line.strip().lower()
                if clean_line:
                    virus_hashes.add(clean_line)
    return virus_hashes

def main():
    target_files = get_txt_files_in_current_dir()
    
    origin_hashes = {}
    with open("HashList.txt", "w", encoding="utf-8") as f_hashlist:
        for file_name in target_files:
            file_hash = compute_sha256(file_name)
            if file_hash:
                origin_hashes[file_name] = file_hash
                f_hashlist.write(f"{file_name} - {file_hash}\n")

    logger.info("Saved initial sums to HashList.txt")

    logger.info("Launching FC...")
    try:
        filename = "./FC"
        if os.name == 'nt':
            filename = "FC.exe"

        subprocess.run([filename], shell=True)
    except Exception as e:
        logger.error(f"[Error] Could not launch FC: {e}")
        logger.info("Continuing working with current file state...")

    virus_hashes = load_virus_hashes()
    
    changed_files = {}
    infected_files = {}

    for file_name in target_files:
        if not os.path.exists(file_name):
            continue
            
        current_hash = compute_sha256(file_name)
        old_hash = origin_hashes.get(file_name)

        if current_hash in virus_hashes:
            infected_files[file_name] = current_hash
        elif current_hash != old_hash:
            changed_files[file_name] = current_hash

    with open("report.txt", "w", encoding="utf-8") as f_report:
        f_report.write("Origin hash:\n")
        for file_name, file_hash in origin_hashes.items():
            f_report.write(f"{file_name} - {file_hash}\n")
            
        f_report.write("Changed:\n")
        for file_name, file_hash in changed_files.items():
            f_report.write(f"{file_name} - {file_hash}\n")
            
        f_report.write("Infected:\n")
        for file_name, file_hash in infected_files.items():
            f_report.write(f"{file_name} - {file_hash}\n")

    print("Results are saved to report.txt")

    if infected_files:
        print("Found infected files. Deleting:")
        for file_name in infected_files.keys():
            try:
                os.remove(file_name)
                print(f"Purged {file_name}")
            except Exception as e:
                print(f"Could not delete {file_name}: {e}")
    else:
        print("No infected files found")

if __name__ == "__main__":
    main()

