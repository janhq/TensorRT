# For SDXL
- Build docker image to convert sdxl to tensorrt:
```
DOCKER_BUILDKIT=1 docker build -t janresearch.azurecr.io/sdxl_converter:tr23.04-trt8.6.1 -f sdxl.Dockerfile .
```
- Run the image up and convert sdxl to tensorrt:
```
docker run --gpus device=1 \
           --ipc=host \
           --ulimit memlock=-1 \
           --ulimit stack=67108864 \
           -v $(pwd)/output/sdxl/engine_xl_base:/workspace/TensorRT_SDXL/demo/Diffusion/engine_xl_base/ \
           -v $(pwd)/output/sdxl/engine_xl_refiner:/workspace/TensorRT_SDXL/demo/Diffusion/engine_xl_refiner/ \
           -v $(pwd)/output/sdxl/onnx_xl_base:/workspace/TensorRT_SDXL/demo/Diffusion/onnx_xl_base/ \
           -v $(pwd)/output/sdxl/onnx_xl_refiner:/workspace/TensorRT_SDXL/demo/Diffusion/onnx_xl_refiner/ \
           -v $(pwd)/output/sdxl/output:/workspace/TensorRT_SDXL/demo/Diffusion/output \
           -it --rm janresearch.azurecr.io/sdxl_converter:tr23.04-trt8.6.1
python3 demo_txt2img_xl.py --hf-token=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 -v "a beautiful photograph of Mt. Fuji during cherry blossom" --repeat-prompt 4 --build-dynamic-shape
```
- Copy the .plan file at `./output/sdxl` to serve on triton
```
cp /home/ubuntu/TensorRT_9/output/sdxl/engine_xl_base/clip.plan /home/ubuntu/triton_server_sdxl/models/text_encoder_1_base/1/clip.plan
cp /home/ubuntu/TensorRT_9/output/sdxl/engine_xl_base/clip2.plan /home/ubuntu/triton_server_sdxl/models/text_encoder_2_base/1/clip2.plan
cp /home/ubuntu/TensorRT_9/output/sdxl/engine_xl_base/unetxl.plan /home/ubuntu/triton_server_sdxl/models/unet_xl_base/1/unetxl.plan
cp /home/ubuntu/TensorRT_9/output/sdxl/engine_xl_refiner/clip2.plan /home/ubuntu/triton_server_sdxl/models/text_encoder_2_refiner/1/clip2.plan
cp /home/ubuntu/TensorRT_9/output/sdxl/engine_xl_refiner/unetxl.plan /home/ubuntu/triton_server_sdxl/models/unet_xl_refiner/1/unetxl.plan
```

# For SD15
Download and convert CivitAI weights:
```
pip install -r conversion_req.txt
./download_civit_ai_weights.sh
```
## Dreamshaper_7
- Run the image up and convert sd15 dreamshaper_7 to tensorrt:
```
docker run --gpus device=1 \
           --ipc=host \
           --ulimit memlock=-1 \
           --ulimit stack=67108864 \
           -v $(pwd)/weights:/workspace/TensorRT_SDXL/weights \
           -v $(pwd)/output/sb15_dreamshaper_7/engine:/workspace/TensorRT_SDXL/demo/Diffusion/engine \
           -v $(pwd)/output/sb15_dreamshaper_7/onnx:/workspace/TensorRT_SDXL/demo/Diffusion/onnx \
           -v $(pwd)/output/sb15_dreamshaper_7/output:/workspace/TensorRT_SDXL/demo/Diffusion/output \
           -it --rm janresearch.azurecr.io/sdxl_converter:tr23.04-trt8.6.1
python3 demo_txt2img.py --version 1.5-dreamshaper_7 --hf-token=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 -v --repeat-prompt 4 "a beautiful photograph of Mt. Fuji during cherry blossom" --build-dynamic-shape
```
- Copy the .plan file at `./output/sb15_dreamshaper_7` to serve on triton:
```
cp /home/ubuntu/TensorRT_9/output/sb15_dreamshaper_7/engine/clip.plan /home/ubuntu/triton_server_sd15/models/text_encoder/1/clip.plan
cp /home/ubuntu/TensorRT_9/output/sb15_dreamshaper_7/engine/unet.plan /home/ubuntu/triton_server_sd15/models/unet_dreamshaper_7/1/unet.plan
cp /home/ubuntu/TensorRT_9/output/sb15_dreamshaper_7/engine/vae.plan /home/ubuntu/triton_server_sd15/models/vae_decoder/1/vae.plan
```
## Openjourney_V4
- Run the image up and convert sd15 openjourney_V4 to tensorrt:
```
docker run --gpus device=1 \
           --ipc=host \
           --ulimit memlock=-1 \
           --ulimit stack=67108864 \
           -v $(pwd)/weights:/workspace/TensorRT_SDXL/weights \
           -v $(pwd)/output/sb15_openjourney_V4/engine:/workspace/TensorRT_SDXL/demo/Diffusion/engine \
           -v $(pwd)/output/sb15_openjourney_V4/onnx:/workspace/TensorRT_SDXL/demo/Diffusion/onnx \
           -v $(pwd)/output/sb15_openjourney_V4/output:/workspace/TensorRT_SDXL/demo/Diffusion/output \
           -it --rm janresearch.azurecr.io/sdxl_converter:tr23.04-trt8.6.1
python3 demo_txt2img.py --version 1.5-openjourney_V4 --hf-token=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 -v --repeat-prompt 4 "a beautiful photograph of Mt. Fuji during cherry blossom" --build-dynamic-shape
```
- Copy the .plan file at `./output/sb15_openjourney_V4` to serve on triton:
```
cp /home/ubuntu/TensorRT_9/output/sb15_openjourney_V4/engine/unet.plan /home/ubuntu/triton_server_sd15/models/unet_openjourney_V4/1/unet.plan
```

## Meinamix_meinaV11
- Run the image up and convert sd15 meinamix_meinaV11 to tensorrt:
```
docker run --gpus device=1 \
           --ipc=host \
           --ulimit memlock=-1 \
           --ulimit stack=67108864 \
           -v $(pwd)/weights:/workspace/TensorRT_SDXL/weights \
           -v $(pwd)/output/sb15_meinamix_meinaV11/engine:/workspace/TensorRT_SDXL/demo/Diffusion/engine \
           -v $(pwd)/output/sb15_meinamix_meinaV11/onnx:/workspace/TensorRT_SDXL/demo/Diffusion/onnx \
           -v $(pwd)/output/sb15_meinamix_meinaV11/output:/workspace/TensorRT_SDXL/demo/Diffusion/output \
           -it --rm janresearch.azurecr.io/sdxl_converter:tr23.04-trt8.6.1
python3 demo_txt2img.py --version 1.5-meinamix_meinaV11 --hf-token=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 -v --repeat-prompt 4 "a beautiful photograph of Mt. Fuji during cherry blossom" --build-dynamic-shape
```
- Copy the .plan file at `./output/sb15_meinamix_meinaV11` to serve on triton:
```
cp /home/ubuntu/TensorRT_9/output/sb15_meinamix_meinaV11/engine/unet.plan /home/ubuntu/triton_server_sd15/models/unet_meinamix_meinaV11/1/unet.plan
```

# Controlnet SD1.5
## Canny
- Run the image up and convert controlnet canny to tensorrt:
```
docker run --gpus device=1 \
           --ipc=host \
           --ulimit memlock=-1 \
           --ulimit stack=67108864 \
           -v $(pwd)/weights:/workspace/TensorRT_SDXL/weights \
           -v $(pwd)/output/sb15_dreamshaper_7_controlnet_canny/engine:/workspace/TensorRT_SDXL/demo/Diffusion/engine \
           -v $(pwd)/output/sb15_dreamshaper_7_controlnet_canny/onnx:/workspace/TensorRT_SDXL/demo/Diffusion/onnx \
           -v $(pwd)/output/sb15_dreamshaper_7_controlnet_canny/output:/workspace/TensorRT_SDXL/demo/Diffusion/output \
           -it --rm janresearch.azurecr.io/sdxl_converter:tr23.04-trt8.6.1
python3 demo_controlnet.py --version 1.5-dreamshaper_7 --hf-token=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 -v --controlnet-type canny --controlnet-scale 1.0 --repeat-prompt 4 "a beautiful photograph of Mt. Fuji during cherry blossom" --build-dynamic-shape
```
- Copy the .plan file at `./output/sb15_dreamshaper_7_controlnet_canny` to serve on triton:
```
cp /home/ubuntu/TensorRT_9/output/sb15_dreamshaper_7_controlnet_canny/engine/unet.plan /home/ubuntu/triton_server_sd15/models/unet_controlnet_canny/1/unet.plan
```
## Depth
- Run the image up and convert controlnet depth to tensorrt:
```
docker run --gpus device=1 \
           --ipc=host \
           --ulimit memlock=-1 \
           --ulimit stack=67108864 \
           -v $(pwd)/weights:/workspace/TensorRT_SDXL/weights \
           -v $(pwd)/output/sb15_dreamshaper_7_controlnet_depth/engine:/workspace/TensorRT_SDXL/demo/Diffusion/engine \
           -v $(pwd)/output/sb15_dreamshaper_7_controlnet_depth/onnx:/workspace/TensorRT_SDXL/demo/Diffusion/onnx \
           -v $(pwd)/output/sb15_dreamshaper_7_controlnet_depth/output:/workspace/TensorRT_SDXL/demo/Diffusion/output \
           -it --rm janresearch.azurecr.io/sdxl_converter:tr23.04-trt8.6.1
python3 demo_controlnet.py --version 1.5-dreamshaper_7 --hf-token=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 -v --controlnet-type depth --controlnet-scale 1.0 --repeat-prompt 4 "a beautiful photograph of Mt. Fuji during cherry blossom" --build-dynamic-shape
```
- Copy the .plan file at `./output/sb15_dreamshaper_7_controlnet_depth` to serve on triton:
```
cp /home/ubuntu/TensorRT_9/output/sb15_dreamshaper_7_controlnet_depth/engine/unet.plan /home/ubuntu/triton_server_sd15/models/unet_controlnet_depth/1/unet.plan
```

# For SD15 Inpainting
- Run the image up and convert sd15 inpaint runwayml to tensorrt:
```
docker run --gpus device=0 \
           --ipc=host \
           --ulimit memlock=-1 \
           --ulimit stack=67108864 \
           -v $(pwd)/weights:/workspace/TensorRT_SDXL/weights \
           -v $(pwd)/output/sd15_inpaint_runwayml/engine:/workspace/TensorRT_SDXL/demo/Diffusion/engine \
           -v $(pwd)/output/sd15_inpaint_runwayml/onnx:/workspace/TensorRT_SDXL/demo/Diffusion/onnx \
           -v $(pwd)/output/sd15_inpaint_runwayml/output:/workspace/TensorRT_SDXL/demo/Diffusion/output \
           -it --rm janresearch.azurecr.io/sdxl_converter:tr23.04-trt8.6.1-nvidiaa6000
cd demo/Diffusion/
python3 demo_inpaint.py --version 1.5 --hf-token=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 -v --repeat-prompt 4 "Face of a yellow cat, high resolution, sitting on a park bench" --build-dynamic-shape
```
- Copy the .plan file at `./output/sd15_inpaint_runwayml` to serve on triton:
```
cp /home/ubuntu/TensorRT/output/sd15_inpaint_runwayml/engine/unet_inpaint.plan /home/ubuntu/sd_1.5/models/unet_inpaint/1/unet_inpaint.plan
cp /home/ubuntu/TensorRT/output/sd15_inpaint_runwayml/engine/vae_encoder_inpaint.plan /home/ubuntu/sd_1.5/models/vae_encoder_inpaint/1/vae_encoder_inpaint.plan
```