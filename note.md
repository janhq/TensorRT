- Build docker image to convert sdxl to tensorrt:
```
DOCKER_BUILDKIT=1 docker build -t janresearch.azurecr.io/sdxl_converter:tr23.04-trt8.6.1-nvidiaa6000 -f sdxl.Dockerfile .
```
- Run the image up and convert sdxl to tensorrt:
```
docker run --gpus device=1 \
           --ipc=host \
           --ulimit memlock=-1 \
           --ulimit stack=67108864 \
           -v $(pwd)/weights:/workspace/TensorRT_SDXL/weights \
           -v $(pwd)/sdxl/engine_xl_base:/workspace/TensorRT_SDXL/demo/Diffusion/engine_xl_base/ \
           -v $(pwd)/sdxl/engine_xl_refiner:/workspace/TensorRT_SDXL/demo/Diffusion/engine_xl_refiner/ \
           -v $(pwd)/sdxl/onnx_xl_base:/workspace/TensorRT_SDXL/demo/Diffusion/onnx_xl_base/ \
           -v $(pwd)/sdxl/onnx_xl_refiner:/workspace/TensorRT_SDXL/demo/Diffusion/onnx_xl_refiner/ \
           -v $(pwd)/sdxl/output:/workspace/TensorRT_SDXL/demo/Diffusion/output \
           -it --rm janresearch.azurecr.io/sdxl_converter:tr23.04-trt8.6.1-nvidiaa6000
cd demo/Diffusion/
python3 demo_txt2img_xl.py --hf-token=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 -v "a beautiful photograph of Mt. Fuji during cherry blossom" --repeat-prompt 4 --build-dynamic-shape
```
- Copy the .plan file at `./sdxl` to serve on triton
```
cp /home/ubuntu/TensorRT_9/sdxl/engine_xl_base/clip.plan /home/ubuntu/triton_server_sdxl/models/text_encoder_1_base/1/clip.plan
cp /home/ubuntu/TensorRT_9/sdxl/engine_xl_base/clip2.plan /home/ubuntu/triton_server_sdxl/models/text_encoder_2_base/1/clip2.plan
cp /home/ubuntu/TensorRT_9/sdxl/engine_xl_base/unetxl.plan /home/ubuntu/triton_server_sdxl/models/unet_xl_base/1/unetxl.plan
cp /home/ubuntu/TensorRT_9/sdxl/engine_xl_refiner/clip2.plan /home/ubuntu/triton_server_sdxl/models/text_encoder_2_refiner/1/clip2.plan
cp /home/ubuntu/TensorRT_9/sdxl/engine_xl_refiner/unetxl.plan /home/ubuntu/triton_server_sdxl/models/unet_xl_refiner/1/unetxl.plan
```