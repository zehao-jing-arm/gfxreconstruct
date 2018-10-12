/*
** Copyright (c) 2018 LunarG, Inc.
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef BRIMSTONE_FORMAT_PARAMETER_ENCODER_H
#define BRIMSTONE_FORMAT_PARAMETER_ENCODER_H

#include <cstring>
#include <cwchar>
#include <memory>
#include <type_traits>

#include "vulkan/vulkan.h"

#include "util/defines.h"
#include "util/output_stream.h"
#include "util/platform.h"
#include "format/format.h"

BRIMSTONE_BEGIN_NAMESPACE(brimstone)
BRIMSTONE_BEGIN_NAMESPACE(format)

class ParameterEncoder
{
public:
    ParameterEncoder(util::OutputStream* stream) : output_stream_(stream) { }

    ~ParameterEncoder() { }

    void Reset() { output_stream_->Reset(); }

    // clang-format off

    // Values
    void EncodeUInt8Value(uint8_t value)                                                                              { EncodeValue(value); }
    void EncodeUInt16Value(uint16_t value)                                                                            { EncodeValue(value); }
    void EncodeInt32Value(int32_t value)                                                                              { EncodeValue(value); }
    void EncodeUInt32Value(uint32_t value)                                                                            { EncodeValue(value); }
    void EncodeInt64Value(int64_t value)                                                                              { EncodeValue(value); }
    void EncodeUInt64Value(uint64_t value)                                                                            { EncodeValue(value); }
    void EncodeFloatValue(float value)                                                                                { EncodeValue(value); }
    void EncodeVkBool32Value(VkBool32 value)                                                                          { EncodeValue(value); }
    void EncodeVkSampleMaskValue(VkSampleMask value)                                                                  { EncodeValue(static_cast<SampleMaskEncodeType>(value)); }
    void EncodeVkDeviceSizeValue(VkDeviceSize value)                                                                  { EncodeValue(static_cast<DeviceSizeEncodeType>(value)); }
    void EncodeSizeTValue(size_t value)                                                                               { EncodeValue(static_cast<SizeTEncodeType>(value)); }

    // Encode the address values for pointers to non-Vulkan objects to be used as object IDs.
    void EncodeAddress(const void* value)                                                                             { EncodeValue(reinterpret_cast<AddressEncodeType>(value)); }
    void EncodeVoidPtr(const void* value)                                                                             { EncodeAddress(value); }
    template<typename T>
    void EncodeFunctionPtr(T value)                                                                                   { EncodeValue(reinterpret_cast<AddressEncodeType>(value)); }

    template<typename T>
    void EncodeHandleIdValue(T value)                                                                                 { EncodeValue(TypeCast<HandleEncodeType>(value)); }
    template<typename T>
    void EncodeEnumValue(T value)                                                                                     { EncodeValue(static_cast<EnumEncodeType>(value)); }
    template<typename T>
    void EncodeFlagsValue(T value)                                                                                    { EncodeValue(static_cast<FlagsEncodeType>(value)); }

    // Pointers
    void EncodeInt32Ptr(const int32_t* ptr, bool omit_addr = false, bool omit_data = false)                           { EncodePointer(ptr, omit_addr, omit_data); }
    void EncodeUInt32Ptr(const uint32_t* ptr, bool omit_addr = false, bool omit_data = false)                         { EncodePointer(ptr, omit_addr, omit_data); }
    void EncodeInt64Ptr(const int64_t* ptr, bool omit_addr = false, bool omit_data = false)                           { EncodePointer(ptr, omit_addr, omit_data); }
    void EncodeUInt64Ptr(const uint64_t* ptr, bool omit_addr = false, bool omit_data = false)                         { EncodePointer(ptr, omit_addr, omit_data); }
    void EncodeFloatPtr(const float* ptr, bool omit_addr = false, bool omit_data = false)                             { EncodePointer(ptr, omit_addr, omit_data); }
    void EncodeVkBool32Ptr(const VkBool32* ptr, bool omit_addr = false, bool omit_data = false)                       { EncodePointer(ptr, omit_addr, omit_data); }
    void EncodeVkSampleMaskPtr(const VkSampleMask* ptr, bool omit_addr = false, bool omit_data = false)               { EncodePointerConverted<SampleMaskEncodeType>(ptr, omit_addr, omit_data); }
    void EncodeVkDeviceSizePtr(const VkDeviceSize* ptr, bool omit_addr = false, bool omit_data = false)               { EncodePointerConverted<DeviceSizeEncodeType>(ptr, omit_addr, omit_data); }
    void EncodeSizeTPtr(const size_t* ptr, bool omit_addr = false, bool omit_data = false)                            { EncodePointerConverted<SizeTEncodeType>(ptr, omit_addr, omit_data); }

    // Treat pointers to non-Vulkan objects as 64-bit object IDs.
    void EncodeVoidPtrPtr(const void* const* ptr, bool omit_addr = false, bool omit_data = false)                     { EncodePointerConverted<AddressEncodeType>(ptr, omit_addr, omit_data); }

    template<typename T>
    void EncodeHandleIdPtr(const T* ptr, bool omit_addr = false, bool omit_data = false)                              { EncodePointerConverted<HandleEncodeType>(ptr, omit_addr, omit_data); }
    template<typename T>
    void EncodeEnumPtr(const T* ptr, bool omit_addr = false, bool omit_data = false)                                  { EncodePointerConverted<EnumEncodeType>(ptr, omit_addr, omit_data); }
    template<typename T>
    void EncodeFlagsPtr(const T* ptr, bool omit_addr = false, bool omit_data = false)                                 { EncodePointerConverted<FlagsEncodeType>(ptr, omit_addr, omit_data); }

    // Arrays
    void EncodeInt32Array(const int32_t* arr, size_t len, bool omit_addr = false, bool omit_data = false)             { EncodeArray(arr, len, omit_addr, omit_data); }
    void EncodeUInt32Array(const uint32_t* arr, size_t len, bool omit_addr = false, bool omit_data = false)           { EncodeArray(arr, len, omit_addr, omit_data); }
    void EncodeInt64Array(const int64_t* arr, size_t len, bool omit_addr = false, bool omit_data = false)             { EncodeArray(arr, len, omit_addr, omit_data); }
    void EncodeUInt64Array(const uint64_t* arr, size_t len, bool omit_addr = false, bool omit_data = false)           { EncodeArray(arr, len, omit_addr, omit_data); }
    void EncodeFloatArray(const float* arr, size_t len, bool omit_addr = false, bool omit_data = false)               { EncodeArray(arr, len, omit_addr, omit_data); }
    void EncodeVkBool32Array(const VkBool32* arr, size_t len, bool omit_addr = false, bool omit_data = false)         { EncodeArray(arr, len, omit_addr, omit_data); }
    void EncodeVkSampleMaskArray(const VkSampleMask* arr, size_t len, bool omit_addr = false, bool omit_data = false) { EncodeArrayConverted<SampleMaskEncodeType>(arr, len, omit_addr, omit_data); }
    void EncodeVkDeviceSizeArray(const VkDeviceSize* arr, size_t len, bool omit_addr = false, bool omit_data = false) { EncodeArrayConverted<DeviceSizeEncodeType>(arr, len, omit_addr, omit_data); }
    void EncodeSizeTArray(const size_t* arr, size_t len, bool omit_addr = false, bool omit_data = false)              { EncodeArrayConverted<SizeTEncodeType>(arr, len, omit_addr, omit_data); }

    // Array of bytes.
    void EncodeUInt8Array(const void* arr, size_t len, bool omit_addr = false, bool omit_data = false)                { EncodeArray(reinterpret_cast<const uint8_t*>(arr), len, omit_addr, omit_data); }
    void EncodeVoidArray(const void* arr, size_t len, bool omit_addr = false, bool omit_data = false)                 { EncodeArray(reinterpret_cast<const uint8_t*>(arr), len, omit_addr, omit_data); }

    template<typename T>
    void EncodeHandleIdArray(const T* arr, size_t len, bool omit_addr = false, bool omit_data = false)                { EncodeArrayConverted<HandleEncodeType>(arr, len, omit_addr, omit_data); }
    template<typename T>
    void EncodeEnumArray(const T* arr, size_t len, bool omit_addr = false, bool omit_data = false)                    { EncodeArrayConverted<EnumEncodeType>(arr, len, omit_addr, omit_data); }
    template<typename T>
    void EncodeFlagsArray(const T* arr, size_t len, bool omit_addr = false, bool omit_data = false)                   { EncodeArrayConverted<FlagsEncodeType>(arr, len, omit_addr, omit_data); }

    void EncodeString(const char* str, bool omit_addr = false, bool omit_data = false)                                { EncodeBasicString<char, PointerAttributes::kIsString>(str, omit_addr, omit_data); }
    void EncodeWString(const wchar_t* str, bool omit_addr = false, bool omit_data = false)                            { EncodeBasicString<wchar_t, PointerAttributes::kIsWString>(str, omit_addr, omit_data); }
    void EncodeStringArray(const char* const* str, size_t len, bool omit_addr = false, bool omit_data = false)        { EncodeBasicStringArray<char, PointerAttributes::kIsString>(str, len, omit_addr, omit_data); }
    void EncodeWStringArray(const wchar_t* const* str, size_t len, bool omit_addr = false, bool omit_data = false)    { EncodeBasicStringArray<wchar_t, PointerAttributes::kIsWString>(str, len, omit_addr, omit_data); }

    // clang-format on

    void EncodeStructPtrPreamble(const void* ptr, bool omit_addr = false, bool omit_data = false)
    {
        uint32_t pointer_attrib = kIsStruct | kIsSingle | GetPointerAttributeMask(ptr, omit_addr, omit_data);

        output_stream_->Write(&pointer_attrib, sizeof(pointer_attrib));

        if ((pointer_attrib & kHasAddress) == kHasAddress)
        {
            EncodeAddress(ptr);
        }
    }

    void EncodeStructArrayPreamble(const void* arr, size_t len, bool omit_addr = false, bool omit_data = false)
    {
        uint32_t pointer_attrib = kIsStruct | kIsArray | GetPointerAttributeMask(arr, omit_addr, omit_data);

        output_stream_->Write(&pointer_attrib, sizeof(pointer_attrib));

        if (arr != nullptr)
        {
            if ((pointer_attrib & kHasAddress) == kHasAddress)
            {
                EncodeAddress(arr);
            }

            // Always write the array size when the pointer is not null.
            EncodeSizeTValue(len);
        }
    }

private:
    uint32_t GetPointerAttributeMask(const void* ptr, bool omit_addr, bool omit_data)
    {
        uint32_t pointer_attrib = 0;

        if (ptr == nullptr)
        {
            pointer_attrib |= kIsNull;
        }
        else
        {
            if (!omit_addr)
            {
                pointer_attrib |= kHasAddress;
            }

            if (!omit_data)
            {
                pointer_attrib |= kHasData;
            }
        }

        return pointer_attrib;
    }

    template<typename DstT, typename SrcT>
    typename std::enable_if<!std::is_pointer<SrcT>::value && !std::is_pointer<DstT>::value, DstT>::type TypeCast(SrcT value)
    {
        return static_cast<DstT>(value);
    }

    template<typename DstT, typename SrcT>
    typename std::enable_if<std::is_pointer<SrcT>::value || std::is_pointer<DstT>::value, DstT>::type TypeCast(SrcT value)
    {
        return reinterpret_cast<DstT>(value);
    }

    template<typename T>
    void EncodeValue(T value)
    {
        output_stream_->Write(&value, sizeof(T));
    }

    template<typename T>
    void EncodePointer(const T* ptr, bool omit_addr = false, bool omit_data = false)
    {
        uint32_t pointer_attrib = kIsSingle | GetPointerAttributeMask(ptr, omit_addr, omit_data);

        output_stream_->Write(&pointer_attrib, sizeof(pointer_attrib));

        if (ptr != nullptr)
        {
            if ((pointer_attrib & kHasAddress) == kHasAddress)
            {
                EncodeAddress(ptr);
            }

            if ((pointer_attrib & kHasData) == kHasData)
            {
                output_stream_->Write(ptr, sizeof(T));
            }
        }
    }

    template<typename DstT, typename SrcT>
    void EncodePointerConverted(const SrcT* ptr, bool omit_addr = false, bool omit_data = false)
    {
        uint32_t pointer_attrib = kIsSingle | GetPointerAttributeMask(ptr, omit_addr, omit_data);

        output_stream_->Write(&pointer_attrib, sizeof(pointer_attrib));

        if (ptr != nullptr)
        {
            if ((pointer_attrib & kHasAddress) == kHasAddress)
            {
                EncodeAddress(ptr);
            }

            if ((pointer_attrib & kHasData) == kHasData)
            {
                DstT converted = TypeCast<DstT>(*ptr);
                output_stream_->Write(&converted, sizeof(DstT));
            }
        }
    }

    template<typename T>
    void EncodeArray(const T* arr, size_t len, bool omit_addr = false, bool omit_data = false)
    {
        uint32_t pointer_attrib = kIsArray | GetPointerAttributeMask(arr, omit_addr, omit_data);

        output_stream_->Write(&pointer_attrib, sizeof(pointer_attrib));

        if (arr != nullptr)
        {
            if ((pointer_attrib & kHasAddress) == kHasAddress)
            {
                EncodeAddress(arr);
            }

            // Always write the array size when the pointer is not null.
            EncodeSizeTValue(len);

            if ((pointer_attrib & kHasData) == kHasData)
            {
                output_stream_->Write(arr, len * sizeof(T));
            }
        }
    }

    // Perform a type conversion for array elements when the original type has a size that is not equal to the target type for conversion.
    template<typename DstT, typename SrcT>
    typename std::enable_if<sizeof(SrcT) != sizeof(DstT), void>::type EncodeArrayConverted(const SrcT* arr, size_t len, bool omit_addr = false, bool omit_data = false)
    {
        uint32_t pointer_attrib = kIsArray | GetPointerAttributeMask(arr, omit_addr, omit_data);

        output_stream_->Write(&pointer_attrib, sizeof(pointer_attrib));

        if (arr != nullptr)
        {
            if ((pointer_attrib & kHasAddress) == kHasAddress)
            {
                EncodeAddress(arr);
            }

            // Always write the array size when the pointer is not null.
            EncodeSizeTValue(len);

            if ((pointer_attrib & kHasData) == kHasData)
            {
                for (size_t i = 0; i < len; ++i)
                {
                    DstT converted = TypeCast<DstT>(arr[i]);
                    fwrite(&converted, sizeof(DstT), 1, stdout);
                }
            }
        }
    }

    // Overload for the case where the original type and the conversion type have matching sizes, where we can skip the type conversion.
    template<typename DstT, typename SrcT>
    typename std::enable_if<sizeof(SrcT) == sizeof(DstT), void>::type EncodeArrayConverted(const SrcT* arr, size_t len, bool omit_addr = false, bool omit_data = false)
    {
        EncodeArray(arr, len, omit_addr, omit_data);
    }

    template <typename CharT, PointerAttributes EncodeAttrib>
    void EncodeBasicString(const CharT* str, bool omit_addr, bool omit_data)
    {
        uint32_t pointer_attrib = EncodeAttrib | kIsSingle | GetPointerAttributeMask(str, omit_addr, omit_data);

        output_stream_->Write(&pointer_attrib, sizeof(pointer_attrib));

        if (str != nullptr)
        {
            if ((pointer_attrib & kHasAddress) == kHasAddress)
            {
                EncodeAddress(str);
            }

            // Always write the string length.
            size_t len = util::platform::StringLength(str);

            EncodeSizeTValue(len);

            if ((pointer_attrib & kHasData) == kHasData)
            {
                output_stream_->Write(str, len);
            }
        }
    }

    template <typename CharT, PointerAttributes EncodeAttrib>
    void EncodeBasicStringArray(const CharT* const* str, size_t len, bool omit_addr, bool omit_data)
    {
        uint32_t pointer_attrib = EncodeAttrib | kIsArray | GetPointerAttributeMask(str, omit_addr, omit_data);

        output_stream_->Write(&pointer_attrib, sizeof(pointer_attrib));

        if (str != nullptr)
        {
            if ((pointer_attrib & kHasAddress) == kHasAddress)
            {
                EncodeAddress(str);
            }

            // Always write the array size.
            EncodeSizeTValue(len);

            if ((pointer_attrib & kHasData) == kHasData)
            {
                for (size_t i = 0; i < len; ++i)
                {
                    EncodeBasicString<CharT, EncodeAttrib>(str[i], omit_addr, omit_data);
                }
            }
        }
    }

private:
    util::OutputStream*                 output_stream_;
};

BRIMSTONE_END_NAMESPACE(format)
BRIMSTONE_END_NAMESPACE(brimstone)

#endif // BRIMSTONE_FORMAT_PARAMETER_ENCODER_H