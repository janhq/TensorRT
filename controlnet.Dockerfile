FROM nvcr.io/nvidia/tritonserver:23.06-py3

WORKDIR /workspace

RUN apt-get update && apt-get install cmake ffmpeg libsm6 libxext6 -y
RUN pip install --upgrade pip && pip install --upgrade tensorrt
COPY ./demo/Diffusion/requirements.txt /tmp/requirements.txt
RUN pip3 install -r /tmp/requirements.txt && \
    pip3 install opencv-python "torch<2" diffusers==0.18.2 transformers==4.31.0 \
                 onnxruntime==1.15.1 controlnet_aux==0.0.6 mediapipe

COPY . /workspace/TensorRT_StableDiffusion_ControlNet/
ENV TRT_OSSPATH=/workspace/TensorRT_StableDiffusion_ControlNet
WORKDIR ${TRT_OSSPATH}

RUN mkdir -p build \
    && cd build \
    && cmake .. -DTRT_OUT_DIR=$PWD/out \
    && cd plugin \
    && make -j$(nproc)

ENV PLUGIN_LIBS="${TRT_OSSPATH}/build/out/libnvinfer_plugin.so"

RUN cd demo/Diffusion/ \
    && mkdir -p onnx engine output
