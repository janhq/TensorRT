FROM nvcr.io/nvidia/tritonserver:23.06-py3

WORKDIR /workspace

RUN apt-get update && apt-get install cmake ffmpeg libsm6 libxext6 -y

COPY ./demo/Diffusion/requirements.txt /tmp/requirements.txt
RUN pip3 install -r /tmp/requirements.txt && \
    pip3 install --upgrade pip && \
    pip3 install --upgrade tensorrt

COPY . /workspace/TensorRT_SDXL/
ENV TRT_OSSPATH=/workspace/TensorRT_SDXL
WORKDIR ${TRT_OSSPATH}/demo/Diffusion/
