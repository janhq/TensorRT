#!/bin/bash

# Define the directory where the files will be saved

# # =============== First for safe tensor models =================
# # Define the list of URLs
# url_list=(
#   "https://civitai.com/api/download/models/119057" # meinamix
# #   "https://civitai.com/api/download/models/109123" # dreamshaper
# )
# download_dir="./models_store_safetensor"
# mkdir -p "$download_dir"
# # Loop through each URL and download the files
# for url in "${url_list[@]}"; do
#   wget -nc --content-disposition -P "$download_dir" "$url"
# done
# # Loop through the downloaded files and echo their names
# for file in "$download_dir"/*; do
#   echo "Start converting for File: $(basename "$file")"
#   python3 convert_model_to_hf.py --checkpoint_path $file --original_config_file ./v1-inference.yaml --from_safetensors --dump_path ./weights/$(basename "$file" | cut -d. -f1)/ --half
# done

# =============== Then for checkpoint models ===============
url_list=(
  "https://civitai.com/api/download/models/27392" # openjourney
)
download_dir="./models_store_checkpoints"
mkdir -p "$download_dir"
# Loop through each URL and download the files
for url in "${url_list[@]}"; do
  wget -nc --content-disposition -P "$download_dir" "$url"
done
for file in "$download_dir"/*; do
  echo "Start converting for File: $(basename "$file")"
  python3 convert_model_to_hf.py --checkpoint_path $file --original_config_file ./v1-inference.yaml --dump_path ./weights/$(basename "$file" | cut -d. -f1)/ --half
done