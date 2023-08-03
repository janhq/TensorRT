- Build docker image to convert controlenet to tensorrt:
```
DOCKER_BUILDKIT=1 docker build -t janresearch.azurecr.io/controlnet_converter:tr23.04-trt8.6.1-nvidiaa6000 -f controlnet.Dockerfile .
```
- Run the image up and convert controlnet canny to tensorrt:
```
docker run --gpus device=1 \
           --ipc=host \
           --ulimit memlock=-1 \
           --ulimit stack=67108864 \
           -v $(pwd)/weights:/workspace/TensorRT_StableDiffusion_ControlNet/weights \
           -v $(pwd)/controlnet/canny/engine:/workspace/TensorRT_StableDiffusion_ControlNet/demo/Diffusion/engine \
           -v $(pwd)/controlnet/canny/onnx:/workspace/TensorRT_StableDiffusion_ControlNet/demo/Diffusion/onnx \
           -v $(pwd)/controlnet/canny/output:/workspace/TensorRT_StableDiffusion_ControlNet/demo/Diffusion/output \
           -it --rm janresearch.azurecr.io/controlnet_converter:tr23.04-trt8.6.1-nvidiaa6000
cd demo/Diffusion/
LD_PRELOAD=${PLUGIN_LIBS} CUDA_MODULE_LOADING=LAZY python3 demo_txt2img_controlnet.py --version 1.5 --hf-token=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 -v --controlnet-type canny --controlnet-scale 1.0 --repeat-prompt 4 "a beautiful photograph of Mt. Fuji during cherry blossom"
```
- Copy the canny file to serve on triton
```
mkdir -p /home/ubuntu/triton_server/models/text_encoder/1/
cp controlnet/canny/engine/clip.plan /home/ubuntu/triton_server/models/text_encoder/1/

mkdir -p /home/ubuntu/triton_server/models/unet_controlnet_canny/1/
cp controlnet/canny/engine/unet.plan /home/ubuntu/triton_server/models/unet_controlnet_canny/1/unet_controlnet_canny.plan

mkdir -p /home/ubuntu/triton_server/models/vae_decoder/1/
cp controlnet/canny/engine/vae.plan /home/ubuntu/triton_server/models/vae_decoder/1/
```
- Run the image up and convert controlnet depth to tensorrt:
```
docker run --gpus device=1 \
           --ipc=host \
           --ulimit memlock=-1 \
           --ulimit stack=67108864 \
           -v $(pwd)/weights:/workspace/TensorRT_StableDiffusion_ControlNet/weights \
           -v $(pwd)/controlnet/depth/engine:/workspace/TensorRT_StableDiffusion_ControlNet/demo/Diffusion/engine \
           -v $(pwd)/controlnet/depth/onnx:/workspace/TensorRT_StableDiffusion_ControlNet/demo/Diffusion/onnx \
           -v $(pwd)/controlnet/depth/output:/workspace/TensorRT_StableDiffusion_ControlNet/demo/Diffusion/output \
           -it --rm janresearch.azurecr.io/controlnet_converter:tr23.04-trt8.6.1-nvidiaa6000
cd demo/Diffusion/
LD_PRELOAD=${PLUGIN_LIBS} CUDA_MODULE_LOADING=LAZY python3 demo_txt2img_controlnet.py --version 1.5 --hf-token=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 -v --controlnet-type depth --controlnet-scale 1.0 --repeat-prompt 4 "a beautiful photograph of Mt. Fuji during cherry blossom"
```
- Copy the depth file to serve on triton
```
mkdir -p /home/ubuntu/triton_server/models/unet_controlnet_depth/1/
cp controlnet/depth/engine/unet.plan /home/ubuntu/triton_server/models/unet_controlnet_depth/1/unet_controlnet_depth.plan
```
- Run the image up and convert dreamshaper_7 to tensorrt:
```
docker run --gpus device=1 \
           --ipc=host \
           --ulimit memlock=-1 \
           --ulimit stack=67108864 \
           -v $(pwd)/weights:/workspace/TensorRT_StableDiffusion_ControlNet/weights \
           -v $(pwd)/controlnet/dreamshaper_7/engine:/workspace/TensorRT_StableDiffusion_ControlNet/demo/Diffusion/engine \
           -v $(pwd)/controlnet/dreamshaper_7/onnx:/workspace/TensorRT_StableDiffusion_ControlNet/demo/Diffusion/onnx \
           -v $(pwd)/controlnet/dreamshaper_7/output:/workspace/TensorRT_StableDiffusion_ControlNet/demo/Diffusion/output \
           -it --rm janresearch.azurecr.io/controlnet_converter:tr23.04-trt8.6.1-nvidiaa6000
cd demo/Diffusion/
LD_PRELOAD=${PLUGIN_LIBS} CUDA_MODULE_LOADING=LAZY python3 demo_txt2img_controlnet.py --version 1.5 --hf-token=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 -v --repeat-prompt 4 "a beautiful photograph of Mt. Fuji during cherry blossom"
```
- Copy the dreamshaper_7 file to serve on triton
```
mkdir -p /home/ubuntu/triton_server/models/unet_dreamshaper_7/1/
cp controlnet/dreamshaper_7/engine/unet.plan /home/ubuntu/triton_server/models/unet_dreamshaper_7/1/
```
Now back to the other repo for serving