/*
 * SPDX-FileCopyrightText: Copyright (c) 1993-2023 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * SPDX-License-Identifier: LicenseRef-NvidiaProprietary
 *
 * NVIDIA CORPORATION, its affiliates and licensors retain all intellectual
 * property and proprietary rights in and to this material, related
 * documentation and any modifications thereto. Any use, reproduction,
 * disclosure or distribution of this material and related documentation
 * without an express license agreement from NVIDIA CORPORATION or
 * its affiliates is strictly prohibited.
 */

#ifndef NV_INFER_SAFE_RUNTIME_H
#define NV_INFER_SAFE_RUNTIME_H

#include "NvInferRuntimeBase.h"
#include "NvInferRuntimePlugin.h"
#include <cstddef>
#include <cstdint>

//!
//! \file NvInferSafeRuntime.h
//!
//! The functionality in this file is only supported in NVIDIA Drive(R) products.

//!
//! \namespace nvinfer1
//!
//! \brief The TensorRT API version 1 namespace.
//!
namespace nvinfer1
{
//!
//! \namespace nvinfer1::safe
//!
//! \brief The safety subset of TensorRT's API version 1 namespace.
//!
namespace safe
{
//! Forward declare safe::ICudaEngine for use in other interfaces.
class ICudaEngine;
//! Forward declare safe::IExecutionContext for use in other interfaces.
class IExecutionContext;

//!
//! \class IRuntime
//!
//! \brief Allows a serialized functionally safe engine to be deserialized.
//!
//! \warning In the safety runtime the application is required to set the error reporter for correct error handling.
//! \see setErrorRecorder()
//!
//! \warning Do not inherit from this class, as doing so will break forward-compatibility of the API and ABI.
//!
class IRuntime
{
public:
    //!
    //! \brief Deserialize an engine from a stream.
    //!
    //! If the serialized engine requires plugins the plugin creator must be registered by calling
    //! IPluginRegistry::registerCreator() before calling deserializeCudaEngine(). Every plugin creator
    //! registered must have a unique combination of namespace, plugin name, and version.
    //!
    //! \param blob The memory that holds the serialized engine.
    //! \param size The size of the memory in bytes.
    //!
    //! \return The engine, or nullptr if it could not be deserialized.
    //!
    //! \see IPluginRegistry::registerCreator()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes, if called from different instances of safe::IRuntime. Calling deserializeCudaEngine
    //!                  of the same safety runtime from multiple threads is not guaranteed to be thread safe.
    //!
    virtual ICudaEngine* deserializeCudaEngine(void const* const blob, std::size_t const size) noexcept = 0;

    //!
    //! \brief Set the GPU allocator.
    //! \param allocator Set the GPU allocator to be used by the runtime. All GPU memory acquired will use this
    //! allocator. If NULL is passed, the default allocator will be used.
    //!
    //! Default: uses cudaMalloc/cudaFree.
    //!
    //! If nullptr is passed, the default allocator will be used.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: No
    //!
    virtual void setGpuAllocator(IGpuAllocator* const allocator) noexcept = 0;

    //!
    //! \brief Set the ErrorRecorder for this interface.
    //!
    //! Assigns the ErrorRecorder to this interface. The ErrorRecorder will track all errors during execution.
    //! This function will call incRefCount of the registered ErrorRecorder at least once. Setting
    //! recorder to nullptr unregisters the recorder with the interface, resulting in a call to decRefCount if
    //! a recorder has been registered.
    //!
    //! \param recorder The error recorder to register with this interface.
    //
    //! \see getErrorRecorder()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: No
    //!
    virtual void setErrorRecorder(IErrorRecorder* const recorder) noexcept = 0;

    //!
    //! \brief Get the ErrorRecorder assigned to this interface.
    //!
    //! Retrieves the assigned error recorder object for the given class. A default error recorder does not exist,
    //! so a nullptr will be returned if setErrorRecorder has not been called.
    //!
    //! \return A pointer to the IErrorRecorder object that has been registered.
    //!
    //! \see setErrorRecorder()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual IErrorRecorder* getErrorRecorder() const noexcept = 0;

    IRuntime() = default;
    virtual ~IRuntime() noexcept = default;
    IRuntime(IRuntime const&) = delete;
    IRuntime(IRuntime&&) = delete;
    IRuntime& operator=(IRuntime const&) & = delete;
    IRuntime& operator=(IRuntime&&) & = delete;
};

//!
//! \class ICudaEngine
//!
//! \brief A functionally safe engine for executing inference on a built network.
//!
//! \warning Do not inherit from this class, as doing so will break forward-compatibility of the API and ABI.
//!
class ICudaEngine
{
public:
    //!
    //! \brief Get the number of binding indices.
    //!
    //! \return The number of binding indices.
    //!
    //! \deprecated Deprecated in TensorRT 8.5. Superseded by getNbIOTensors.
    //!
    //! \see getBindingIndex()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    TRT_DEPRECATED virtual std::int32_t getNbBindings() const noexcept = 0;

    //!
    //! \brief Retrieve the binding index for a named tensor.
    //!
    //! safe::IExecutionContext::enqueueV2() requires an array of buffers.
    //! Engine bindings map from tensor names to indices in this array.
    //! Binding indices are assigned at engine build time, and take values in the range [0 ... n-1] where n is the total
    //! number of inputs and outputs.
    //!
    //! \warning Strings passed to the runtime must be 1024 characters or less including NULL terminator and must be
    //! NULL terminated.
    //!
    //! \param name The tensor name.
    //! \return The binding index for the named tensor, or -1 if the name is not found.
    //!
    //! \deprecated Deprecated in TensorRT 8.5. Superseded by name-based methods. Use them instead of binding-index
    //! based methods.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    TRT_DEPRECATED virtual std::int32_t getBindingIndex(AsciiChar const* const name) const noexcept = 0;

    //!
    //! \brief Retrieve the name corresponding to a binding index.
    //!
    //! This is the reverse mapping to that provided by getBindingIndex().
    //!
    //! \param bindingIndex The binding index.
    //! \return The name corresponding to the index, or nullptr if the index is out of range.
    //!
    //! \deprecated Deprecated in TensorRT 8.5. Superseded by name-based methods. Use them instead of binding-index
    //! based methods.
    //!
    //! \see getBindingIndex()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    TRT_DEPRECATED virtual AsciiChar const* getBindingName(std::int32_t const bindingIndex) const noexcept = 0;

    //!
    //! \brief Determine whether a binding is an input binding.
    //!
    //! \param bindingIndex The binding index.
    //! \return True if the index corresponds to an input binding and the index is in range.
    //!
    //! \deprecated Deprecated in TensorRT 8.5. Superseded by tensorIOMode().
    //!
    //! \see safe::ICudaEngine::tensorIOMode()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    TRT_DEPRECATED virtual bool bindingIsInput(std::int32_t const bindingIndex) const noexcept = 0;

    //!
    //! \brief Get the dimensions of a binding.
    //!
    //! \param bindingIndex The binding index.
    //! \return The dimensions of the binding if the index is in range, otherwise Dims()
    //!
    //! \deprecated Deprecated in TensorRT 8.5. Superseded by getTensorShape().
    //!
    //! \see safe::ICudaEngine::getTensorShape()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    TRT_DEPRECATED virtual Dims getBindingDimensions(std::int32_t const bindingIndex) const noexcept = 0;

    //!
    //! \brief Determine the required data type for a buffer from its binding index.
    //!
    //! \param bindingIndex The binding index.
    //! \return The type of the data in the buffer.
    //!
    //! \deprecated Deprecated in TensorRT 8.5. Superseded by getTensorDataType().
    //!
    //! \see safe::ICudaEngine::getTensorDataType()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    TRT_DEPRECATED virtual DataType getBindingDataType(std::int32_t const bindingIndex) const noexcept = 0;

    //!
    //! \brief Create an execution context.
    //!
    //! \see safe::IExecutionContext.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes; if createExecutionContext fails, users should treat this as a critical
    //!                  error and not perform any subsequent TensorRT operations apart from outputting
    //!                  the error logs.
    //!
    virtual IExecutionContext* createExecutionContext() noexcept = 0;

    //!
    //! \brief Create an execution context without any device memory allocated.
    //!
    //! The memory for execution of this device context must be supplied by the application.
    //!
    //! \see getDeviceMemorySize() safe::IExecutionContext::setDeviceMemory()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes; if createExecutionContext fails, users should treat this as a critical
    //!                  error and not perform any subsequent TensorRT operations apart from outputting
    //!                  the error logs.
    //!
    virtual IExecutionContext* createExecutionContextWithoutDeviceMemory() noexcept = 0;

    //!
    //! \brief Return the amount of device memory required by an execution context.
    //!
    //! \see safe::IExecutionContext::setDeviceMemory()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual size_t getDeviceMemorySize() const noexcept = 0;

    //!
    //! \brief Return the number of bytes per component of an element.
    //!
    //! The vector component size is returned if getBindingVectorizedDim() != -1.
    //!
    //! \param bindingIndex The binding Index.
    //!
    //! \deprecated Deprecated in TensorRT 8.5. Superseded by getTensorBytesPerComponent().
    //!
    //! \see safe::ICudaEngine::getTensorBytesPerComponent()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    TRT_DEPRECATED virtual std::int32_t getBindingBytesPerComponent(std::int32_t const bindingIndex) const noexcept = 0;

    //!
    //! \brief Return the number of components included in one element.
    //!
    //! The number of elements in the vectors is returned if getBindingVectorizedDim() != -1.
    //!
    //! \param bindingIndex The binding Index.
    //!
    //! \deprecated Deprecated in TensorRT 8.5. Superseded by getTensorComponentsPerElement().
    //!
    //! \see safe::ICudaEngine::getTensorComponentsPerElement()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    TRT_DEPRECATED virtual std::int32_t getBindingComponentsPerElement(std::int32_t const bindingIndex) const noexcept = 0;

    //!
    //! \brief Return the binding format.
    //!
    //! \param bindingIndex The binding Index.
    //!
    //! \deprecated Deprecated in TensorRT 8.5. Superseded by getTensorFormat().
    //!
    //! \see safe::ICudaEngine::getTensorFormat()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    TRT_DEPRECATED virtual TensorFormat getBindingFormat(std::int32_t const bindingIndex) const noexcept = 0;

    //!
    //! \brief Return the dimension index that the buffer is vectorized.
    //!
    //! Specifically -1 is returned if scalars per vector is 1.
    //!
    //! \param bindingIndex The binding Index.
    //!
    //! \deprecated Deprecated in TensorRT 8.5. Superseded by getTensorVectorizedDim().
    //!
    //! \see safe::ICudaEngine::getTensorVectorizedDim()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    TRT_DEPRECATED virtual std::int32_t getBindingVectorizedDim(std::int32_t const bindingIndex) const noexcept = 0;

    //!
    //! \brief Returns the name of the network associated with the engine.
    //!
    //! The name is set during network creation and is retrieved after
    //! building or deserialization.
    //!
    //! \see INetworkDefinition::setName(), INetworkDefinition::getName()
    //!
    //! \return A null-terminated C-style string representing the name of the network.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual AsciiChar const* getName() const noexcept = 0;

    //!
    //! \brief Set the ErrorRecorder for this interface.
    //!
    //! Assigns the ErrorRecorder to this interface. The ErrorRecorder will track all errors during execution.
    //! This function will call incRefCount of the registered ErrorRecorder at least once. Setting
    //! recorder to nullptr unregisters the recorder with the interface, resulting in a call to decRefCount if
    //! a recorder has been registered.
    //!
    //! \param recorder The error recorder to register with this interface.
    //
    //! \see getErrorRecorder()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: No
    //!
    virtual void setErrorRecorder(IErrorRecorder* const recorder) noexcept = 0;

    //!
    //! \brief Get the ErrorRecorder assigned to this interface.
    //!
    //! Retrieves the assigned error recorder object for the given class. A
    //! nullptr will be returned if an error reporter has not been inherited
    //! from the IRuntime, and setErrorReporter() has not been called.
    //!
    //! \return A pointer to the IErrorRecorder object that has been registered.
    //!
    //! \see setErrorRecorder()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual IErrorRecorder* getErrorRecorder() const noexcept = 0;

    ICudaEngine() = default;
    virtual ~ICudaEngine() noexcept = default;
    ICudaEngine(ICudaEngine const&) = delete;
    ICudaEngine(ICudaEngine&&) = delete;
    ICudaEngine& operator=(ICudaEngine const&) & = delete;
    ICudaEngine& operator=(ICudaEngine&&) & = delete;

    //!
    //! \brief Get extent of an input or output tensor.
    //! \param tensorName The name of an input or output tensor.
    //!
    //! \warning The string tensorName must be 1024 characters or less including NULL terminator and must be
    //! NULL terminated.
    //!
    //! \return Extent of the tensor. Dims{-1, {}} will be returned if
    //! (1) name is not the name of an input or output tensor, or
    //! (2) name is nullptr, or
    //! (3) name exceeds the string length limit.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual Dims getTensorShape(AsciiChar const* const tensorName) const noexcept = 0;

    //!
    //! \brief Determine the required data type for a buffer from its tensor name.
    //!
    //! \param tensorName The name of an input or output tensor.
    //!
    //! \warning The string tensorName must be 1024 characters or less including NULL terminator and must be
    //! NULL terminated.
    //!
    //! \return The type of the data in the buffer. DataType::kFLOAT will be returned if
    //! (1) name is not the name of an input or output tensor, or
    //! (2) name is nullptr, or
    //! (3) name exceeds the string length limit.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual DataType getTensorDataType(AsciiChar const* const tensorName) const noexcept = 0;

    //!
    //! \brief Determine whether a tensor is an input or output tensor.
    //!
    //! \param tensorName The name of an input or output tensor.
    //!
    //! \warning The string tensorName must be 1024 characters or less including NULL terminator and must be
    //! NULL terminated.
    //!
    //! \return kINPUT if tensorName is an input, kOUTPUT if tensorName is an output, or kNONE if neither.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual TensorIOMode getTensorIOMode(AsciiChar const* const tensorName) const noexcept = 0;

    //!
    //! \brief Return the number of bytes per component of an element.
    //!
    //! \param tensorName The name of an input or output tensor.
    //!
    //! \warning The string tensorName must be 1024 characters or less including NULL terminator and must be
    //! NULL terminated.
    //!
    //! \return The vector component size. 0 will be returned if
    //! (1) name is not the name of an input or output tensor, or
    //! (2) name is nullptr, or
    //! (3) name exceeds the string length limit, or
    //! (4) the tensor of given name is not vectorized.
    //!
    //! \see safe::ICudaEngine::getTensorVectorizedDim()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual std::int32_t getTensorBytesPerComponent(AsciiChar const* const tensorName) const noexcept = 0;

    //!
    //! \brief Return the number of components included in one element.
    //!
    //! \param tensorName The name of an input or output tensor.
    //!
    //! \warning The string tensorName must be 1024 characters or less including NULL terminator and must be
    //! NULL terminated.
    //!
    //! \return The vector component size. -1 will be returned if
    //! (1) name is not the name of an input or output tensor, or
    //! (2) name is nullptr, or
    //! (3) name exceeds the string length limit, or
    //! (4) the tensor of given name is not vectorized.
    //!
    //! \see safe::ICudaEngine::getTensorVectorizedDim()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual std::int32_t getTensorComponentsPerElement(AsciiChar const* const tensorName) const noexcept = 0;

    //!
    //! \brief Return the tensor format.
    //!
    //! \param tensorName The name of an input or output tensor.
    //!
    //! \warning The string tensorName must be 1024 characters or less including NULL terminator and must be
    //! NULL terminated.
    //!
    //! \return The tensor format. TensorFormat::kLINEAR will be returned if
    //! (1) name is not the name of an input or output tensor, or
    //! (2) name is nullptr, or
    //! (3) name exceeds the string length limit.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual TensorFormat getTensorFormat(AsciiChar const* const tensorName) const noexcept = 0;

    //!
    //! \brief Return the dimension index along which buffer is vectorized.
    //!
    //! Specifically -1 is returned if scalars per vector is 1.
    //!
    //! \param tensorName The name of an input or output tensor.
    //!
    //! \warning The string tensorName must be 1024 characters or less including NULL terminator and must be
    //! NULL terminated.
    //!
    //! \return The dimension index along which the buffer is vectorized. -1 will be returned if
    //! (1) name is not the name of an input or output tensor, or
    //! (2) name is nullptr, or
    //! (3) name exceeds the string length limit, or
    //! (4) the tensor of given name is not vectorized.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual std::int32_t getTensorVectorizedDim(AsciiChar const* const tensorName) const noexcept = 0;

    //!
    //! \brief Return the number of input and output tensors for the network from which the engine was built.
    //!
    //! \return The number of IO tensors.
    //!
    //! \see getIOTensorName()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual std::int32_t getNbIOTensors() const noexcept = 0;

    //!
    //! \brief Return the name of an IO tensor.
    //!
    //! If the index does not fall between 0 and getNbIOTensors()-1, the function will fail with an error code of ErrorCode::kINVALID_ARGUMENT(3) that is
    //! emitted to the registered IErrorRecorder.
    //!
    //! \param index The value that falls between 0 and getNbIOTensors()-1.
    //!
    //! \return The name of an IO tensor. nullptr will be returned if the index does not fall between 0 and
    //! getNbIOTensors()-1.
    //!
    //! \see getNbIOTensors()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual AsciiChar const* getIOTensorName(std::int32_t const index) const noexcept = 0;
};

//!
//! \brief Space to record information about runtime errors
//!
//! kNAN_CONSUMED errors occur when NAN values are stored in an INT8 quantized datatype.
//! kINF_CONSUMED errors occur when +-INF values are stored in an INT8 quantized datatype.
//! kGATHER_OOB errors occur when gather index tensor contains value is out side of data tensor
//! kSCATTER_OOB and kSCATTER_RACE are reserved for future use
//!
//! Mark RuntimeErrorType that occur during asynchronous kernel execution
struct RuntimeErrorInformation
{
    uint64_t bitMask; //!< Each bit represent a RuntimeErrorType has occured during kernel execution
};

enum class RuntimeErrorType : uint64_t
{
    kNAN_CONSUMED = 1ULL << 0, //!< NaN floating-point value was silently consumed
    kINF_CONSUMED = 1ULL << 1, //!< Inf floating-point value was silently consumed
    kGATHER_OOB = 1ULL << 2,   //!< Out-of-bounds access in gather operation
    kSCATTER_OOB = 1ULL << 3,  //!< Out-of-bounds access in scatter operation
    kSCATTER_RACE = 1ULL << 4, //!< Race condition in scatter operation
};

//!
//! \class IExecutionContext
//!
//! \brief Functionally safe context for executing inference using an engine.
//!
//! Multiple safe execution contexts may exist for one safe::ICudaEngine instance, allowing the same
//! engine to be used for the execution of multiple inputs simultaneously.
//!
//! \warning Do not call the APIs of the same IExecutionContext from multiple threads at any given time.
//! Each concurrent execution must have its own instance of an IExecutionContext.
//!
//! \warning Do not inherit from this class, as doing so will break forward-compatibility of the API and ABI.
//!
class IExecutionContext
{
public:
    //!
    //! \brief Get the associated engine.
    //!
    //! \see safe::ICudaEngine
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual ICudaEngine const& getEngine() const noexcept = 0;

    //!
    //! \brief Set the name of the execution context.
    //!
    //! This method copies the name string.
    //!
    //! \warning Strings passed to the runtime must be 1024 characters or less including NULL terminator and must be
    //! NULL terminated.
    //!
    //! \see getName()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: No
    //!
    virtual void setName(AsciiChar const* const name) noexcept = 0;

    //!
    //! \brief Return the name of the execution context.
    //!
    //! \see setName()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: No
    //!
    virtual AsciiChar const* getName() const noexcept = 0;

    //!
    //! \brief Set the device memory for use by this execution context.
    //!
    //! If using enqueueV2() to run the network, The memory is in use
    //! from the invocation of enqueueV2() until network execution is complete.
    //! Releasing or otherwise using the memory for other purposes during this time will result in undefined behavior.
    //!
    //! \warning Do not release or use for other purposes the memory set here during network execution.
    //!
    //! \see safe::ICudaEngine::getDeviceMemorySize() safe::ICudaEngine::createExecutionContextWithoutDeviceMemory()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: No
    //!
    virtual void setDeviceMemory(void* const memory) noexcept = 0;

    //!
    //! \brief Return the strides of the buffer for the given binding.
    //!
    //! \param bindingIndex The binding index.
    //!
    //! \deprecated Deprecated in TensorRT 8.5. Superseded by getTensorStrides().
    //!
    //! \see safe::IExecutionContext::getTensorStrides()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    TRT_DEPRECATED virtual Dims getStrides(std::int32_t const bindingIndex) const noexcept = 0;

    //!
    //! \brief Set the ErrorRecorder for this interface.
    //!
    //! Assigns the ErrorRecorder to this interface. The ErrorRecorder will track all errors during execution.
    //! This function will call incRefCount of the registered ErrorRecorder at least once. Setting
    //! recorder to nullptr unregisters the recorder with the interface, resulting in a call to decRefCount if
    //! a recorder has been registered.
    //!
    //! \param recorder The error recorder to register with this interface.
    //
    //! \see getErrorRecorder()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: No
    //!
    virtual void setErrorRecorder(IErrorRecorder* const recorder) noexcept = 0;

    //!
    //! \brief get the ErrorRecorder assigned to this interface.
    //!
    //! Retrieves the assigned error recorder object for the given class. A default error recorder does not exist,
    //! so a nullptr will be returned if setErrorRecorder has not been called.
    //!
    //! \return A pointer to the IErrorRecorder object that has been registered.
    //!
    //! \see setErrorRecorder()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: No
    //!
    virtual IErrorRecorder* getErrorRecorder() const noexcept = 0;

    //!
    //! \brief Enqueue inference of a batch on a stream.
    //!
    //! This method requires an array of input and output buffers. The mapping from tensor names to indices can be
    //! queried using safe::ICudaEngine::getBindingIndex().
    //! This method only works for an execution context built from a network without an implicit batch dimension.
    //! \param bindings An array of pointers to input and output buffers for the network.
    //! \param stream A cuda stream on which the inference kernels will be enqueued.
    //! \param inputConsumed An optional event which will be signaled when the input buffers can be refilled with new
    //! data.
    //!
    //! \return True if the kernels were enqueued successfully.
    //!
    //! \deprecated Deprecated in TensorRT 8.5. Superseded by enqueueV3().
    //!
    //! \see safe::IExecutionContext::enqueueV3()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: No
    //!
    TRT_DEPRECATED virtual bool enqueueV2(
        void* const* const bindings, cudaStream_t const stream, cudaEvent_t const* const inputConsumed) noexcept = 0;

    IExecutionContext() = default;
    virtual ~IExecutionContext() noexcept = default;
    IExecutionContext(IExecutionContext const&) = delete;
    IExecutionContext(IExecutionContext&&) = delete;
    IExecutionContext& operator=(IExecutionContext const&) & = delete;
    IExecutionContext& operator=(IExecutionContext&&) & = delete;

    //!
    //! \brief Set error buffer output for floating point errors.
    //!
    //! The error buffer output must be allocated in device memory and will be used for subsequent
    //! calls to enqueueV2. Checking the contents of the error buffer after inference is the responsibility
    //! of the application. The pointer passed here must have alignment adequate for the RuntimeErrorInformation
    //! struct.
    //!
    //! \warning Do not release or use the contents of the error buffer for any other purpose before synchronizing
    //! on the CUDA stream passed to enqueueV2.
    //!
    //! \param buffer The device memory to use as floating point error buffer
    //!
    //! \see getErrorBuffer()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: No
    //!
    virtual void setErrorBuffer(RuntimeErrorInformation* const buffer) noexcept = 0;

    //!
    //! \brief Get error buffer output for floating point errors.
    //!
    //! \return Pointer to device memory to use as floating point error buffer or nullptr if not set.
    //!
    //! \see setErrorBuffer()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual RuntimeErrorInformation* getErrorBuffer() const noexcept = 0;

    //!
    //! \brief Return the strides of the buffer for the given tensor name.
    //!
    //! The strides are in units of elements, not components or bytes.
    //! For example, for TensorFormat::kHWC8, a stride of one spans 8 scalars.
    //!
    //! \param tensorName The name of an input or output tensor.
    //!
    //! \warning The string tensorName must be 1024 characters or less including NULL terminator and must be
    //! NULL terminated.
    //!
    //! \return The strides of the buffer for the given tensor name. Dims{-1, {}} will be returned if
    //! (1) name is not the name of an input or output tensor, or
    //! (2) name is nullptr, or
    //! (3) name exceeds the string length limit.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual Dims getTensorStrides(AsciiChar const* const tensorName) const noexcept = 0;

    //!
    //! \brief Set memory address for given input tensor.
    //!
    //! An address defaults to nullptr.
    //!
    //! Before calling enqueueV3(), each input must have a non-null address.
    //!
    //! \param tensorName The name of an input tensor.
    //! \param data The pointer (void const*) to the const data owned by the user.
    //!
    //! \warning The string tensorName must be 1024 characters or less including NULL terminator and must be
    //! NULL terminated.
    //! \warning The pointer must have at least 256-byte alignment.
    //!
    //! \return True on success, false if
    //! (1) name is not the name of an input tensor, or
    //! (2) name is nullptr, or
    //! (3) name exceeds the string length limit, or
    //! (4) pointer to the const data is nullptr or not aligned.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: No
    //!
    virtual bool setInputTensorAddress(AsciiChar const* const tensorName, void const* const data) noexcept = 0;

    //!
    //! \brief Set memory address for given output tensor.
    //!
    //! An address defaults to nullptr.
    //!
    //! Before calling enqueueV3(), each output must have a non-null address.
    //!
    //! \param tensorName The name of an output tensor.
    //! \param data The pointer (void*) to the data owned by the user.
    //!
    //! \warning The string tensorName must be 1024 characters or less including NULL terminator and must be
    //! NULL terminated.
    //! \warning The pointer must have at least 256-byte alignment.
    //!
    //! \return True on success. Return false if
    //! (1) name is not the name of an output tensor, or
    //! (2) name is nullptr, or
    //! (3) name exceeds the string length limit, or
    //! (4) pointer to data is nullptr or not aligned.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: No
    //!
    virtual bool setOutputTensorAddress(AsciiChar const* const tensorName, void* const data) noexcept = 0;

    //!
    //! \brief Mark input as consumed.
    //!
    //! Passing event==nullptr removes whatever event was set, if any.
    //!
    //! \param event The cuda event that is triggered after all input tensors have been consumed.
    //!
    //! \return True on success, false if error occurred.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: No
    //!
    virtual bool setInputConsumedEvent(cudaEvent_t const event) noexcept = 0;

    //!
    //! \brief Return the event associated with consuming the input.
    //!
    //! \return The cuda event, nullptr will be returned if the event is not set yet.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual cudaEvent_t getInputConsumedEvent() const noexcept = 0;

    //!
    //! \brief Get memory address for given input tensor.
    //!
    //! \param tensorName The name of an input tensor.
    //!
    //! \warning The string tensorName must be 1024 characters or less including NULL terminator and must be
    //! NULL terminated.
    //!
    //! \return The memory address for the given input tensor. nullptr will be returned if
    //! (1) name is not the name of an input tensor, or
    //! (2) name is nullptr, or
    //! (3) name exceeds the string length limit, or
    //! (4) the memory address for the given input tensor is not set yet.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual void const* getInputTensorAddress(AsciiChar const* const tensorName) const noexcept = 0;

    //!
    //! \brief Get memory address for given output tensor.
    //!
    //! \param tensorName The name of an output tensor.
    //!
    //! \warning The string tensorName must be 1024 characters or less including NULL terminator and must be
    //! NULL terminated.
    //!
    //! \return Raw output data pointer (void*) for given output tensor, return nullptr if
    //! (1) name is not the name of an output tensor, or
    //! (2) name is nullptr, or
    //! (3) name exceeds the string length limit, or
    //! (4) the memory address for the given output tensor is not set yet.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual void* getOutputTensorAddress(AsciiChar const* const tensorName) const noexcept = 0;

    //!
    //! \brief Enqueue inference on a stream.
    //!
    //! Modifying or releasing memory that has been registered for the tensors before stream
    //! synchronization or the event passed to setInputConsumedEvent has been being triggered results in undefined
    //! behavior.
    //!
    //! \param stream A cuda stream on which the inference kernels will be enqueued.
    //!
    //! \return True on success, false if any execution error occurred.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual bool enqueueV3(cudaStream_t const stream) noexcept = 0;
};

//!
//! \class IPluginRegistry
//!
//! \brief Single registration point for all plugins in an application. It is
//! used to find plugin implementations during engine deserialization.
//! Internally, the plugin registry is considered to be a singleton so all
//! plugins in an application are part of the same global registry.
//! Note that the plugin registry is only supported for plugins of type
//! IPluginV2 and should also have a corresponding IPluginCreator implementation.
//!
//! \see IPluginV2 and IPluginCreator
//!
//! \warning Do not inherit from this class, as doing so will break forward-compatibility of the API and ABI.
//!
//! \warning IPluginRegistry::setErrorRecorder() must be called to register
//! an error recorder with the registry before using other methods in the registry.
//!

class IPluginRegistry
{
public:
    //!
    //! \brief Register a plugin creator. Returns false if one with same type
    //! is already registered.
    //!
    //! \warning The string pluginNamespace must be 1024 bytes or less including the NULL terminator and must be NULL
    //! terminated.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes; calls to this method will be synchronized by a mutex.
    //!
    virtual bool registerCreator(IPluginCreator& creator, AsciiChar const* const pluginNamespace) noexcept = 0;

    //!
    //! \brief Return all the registered plugin creators and the number of
    //! registered plugin creators. Returns nullptr if none found.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: No
    //!
    virtual IPluginCreator* const* getPluginCreatorList(int32_t* const numCreators) const noexcept = 0;

    //!
    //! \brief Return plugin creator based on plugin name, version, and
    //! namespace associated with plugin during network creation.
    //!
    //! \warning The strings pluginName, pluginVersion, and pluginNamespace must be 1024 bytes or less including the
    //! NULL terminator and must be NULL terminated.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual IPluginCreator* getPluginCreator(AsciiChar const* const pluginName, AsciiChar const* const pluginVersion,
        AsciiChar const* const pluginNamespace = "") noexcept
        = 0;

    //!
    //! \brief Set the ErrorRecorder for this interface
    //!
    //! Assigns the ErrorRecorder to this interface. The ErrorRecorder will track all errors during execution.
    //! This function will call incRefCount of the registered ErrorRecorder at least once. Setting
    //! recorder to nullptr unregisters the recorder with the interface, resulting in a call to decRefCount if
    //! a recorder has been registered.
    //!
    //! \param recorder The error recorder to register with this interface.
    //
    //! \see getErrorRecorder()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: No
    //!
    virtual void setErrorRecorder(IErrorRecorder* const recorder) noexcept = 0;

    //!
    //! \brief Get the ErrorRecorder assigned to this interface.
    //!
    //! Retrieves the assigned error recorder object for the given class. A default error recorder does not exist,
    //! so a nullptr will be returned if setErrorRecorder has not been called, or an ErrorRecorder has not been
    //! inherited.
    //!
    //! \return A pointer to the IErrorRecorder object that has been registered.
    //!
    //! \see setErrorRecorder()
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual IErrorRecorder* getErrorRecorder() const noexcept = 0;

    //!
    //! \brief Deregister a previously registered plugin creator.
    //!
    //! Since there may be a desire to limit the number of plugins,
    //! this function provides a mechanism for removing plugin creators registered in TensorRT.
    //! The plugin creator that is specified by \p creator is removed from TensorRT and no longer tracked.
    //!
    //! \return True if the plugin creator was deregistered, false if it was not found in the registry or otherwise
    //! could
    //!     not be deregistered.
    //!
    //! \usage
    //! - Allowed context for the API call
    //!   - Thread-safe: Yes
    //!
    virtual bool deregisterCreator(IPluginCreator const& creator) noexcept = 0;

    // @cond SuppressDoxyWarnings
    IPluginRegistry() = default;
    IPluginRegistry(IPluginRegistry const&) = delete;
    IPluginRegistry(IPluginRegistry&&) = delete;
    IPluginRegistry& operator=(IPluginRegistry const&) & = delete;
    IPluginRegistry& operator=(IPluginRegistry&&) & = delete;
    // @endcond

protected:
    virtual ~IPluginRegistry() noexcept = default;
};

//!
//! \brief Create an instance of an safe::IRuntime class.
//!
//! This class is the logging class for the runtime.
//!
//! \usage
//! - Allowed context for the API call
//!   - Thread-safe: Yes
//!
IRuntime* createInferRuntime(ILogger& logger) noexcept;

//!
//! \brief Return the safe plugin registry
//!
//! \usage
//! - Allowed context for the API call
//!   - Thread-safe: Yes
//!
extern "C" TENSORRTAPI IPluginRegistry* getSafePluginRegistry() noexcept;

//!
//! \brief Register the plugin creator to the registry
//! The static registry object will be instantiated when the plugin library is
//! loaded. This static object will register all creators available in the
//! library to the registry.
//!
//! \warning Statically registering plugins should be avoided in the automotive
//!  safety context as the application developer should first register an error recorder
//!  with the plugin registry via IPluginRegistry::setErrorRecorder() before using
//!  IPluginRegistry::registerCreator() or other methods.
//!
template <typename T>
class PluginRegistrar
{
public:
    PluginRegistrar()
    {
        getSafePluginRegistry()->registerCreator(instance, "");
    }

private:
    //! Plugin instance.
    T instance{};
};

} // namespace safe

} // namespace nvinfer1

#define REGISTER_SAFE_TENSORRT_PLUGIN(name)                                                                            \
    static nvinfer1::safe::PluginRegistrar<name> pluginRegistrar##name {}
#endif // NV_INFER_SAFE_RUNTIME_H
