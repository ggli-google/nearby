// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.210505.3

#ifndef WINRT_Windows_Management_Deployment_Preview_1_H
#define WINRT_Windows_Management_Deployment_Preview_1_H
#include "winrt/impl/Windows.Management.Deployment.Preview.0.h"
WINRT_EXPORT namespace winrt::Windows::Management::Deployment::Preview
{
    struct __declspec(empty_bases) IClassicAppManagerStatics :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IClassicAppManagerStatics>
    {
        IClassicAppManagerStatics(std::nullptr_t = nullptr) noexcept {}
        IClassicAppManagerStatics(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
        IClassicAppManagerStatics(IClassicAppManagerStatics const&) noexcept = default;
        IClassicAppManagerStatics(IClassicAppManagerStatics&&) noexcept = default;
        IClassicAppManagerStatics& operator=(IClassicAppManagerStatics const&) & noexcept = default;
        IClassicAppManagerStatics& operator=(IClassicAppManagerStatics&&) & noexcept = default;
    };
    struct __declspec(empty_bases) IInstalledClassicAppInfo :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IInstalledClassicAppInfo>
    {
        IInstalledClassicAppInfo(std::nullptr_t = nullptr) noexcept {}
        IInstalledClassicAppInfo(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
        IInstalledClassicAppInfo(IInstalledClassicAppInfo const&) noexcept = default;
        IInstalledClassicAppInfo(IInstalledClassicAppInfo&&) noexcept = default;
        IInstalledClassicAppInfo& operator=(IInstalledClassicAppInfo const&) & noexcept = default;
        IInstalledClassicAppInfo& operator=(IInstalledClassicAppInfo&&) & noexcept = default;
    };
}
#endif