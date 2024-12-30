# 一.使用vcpkg编译
```
- name: use-vcpkg
  run: |
      git clone https://github.com/Microsoft/vcpkg.git
      cd vcpkg
      ./bootstrap-vcpkg.sh
      #vcpkg install openssl:x64-windows-static
      vcpkg install curl[brotli,c-ares,http2,ldap,non-http,websockets,zstd]:x64-windows-static
      vcpkg integrate install
      #vcpkg install curl[brotli,c-ares,http2,ldap,non-http,openssl,tool,websockets,zstd]:x64-windows
      #vcpkg install curl[openssl]:x64-windows-static
```
可以只默认编译
生成于
```
.\vcpkg\packages
.\vcpkg\installed\x64-windows-static\lib\
```
## 注：
vcpkg integrate install 用于集成到msbuild
对应vcxproj:
```
<PropertyGroup Label="Globals">
    <VCProjectVersion>16.0</VCProjectVersion>
    <Keyword>Win32Proj</Keyword>
    <ProjectGuid>{7fa92def-b64e-46d6-b7a6-9fec17d09ff8}</ProjectGuid>
    <RootNamespace>ProjectProc</RootNamespace>
    <WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>
    <VcpkgTriplet Condition="'$(Platform)'=='x64'">x64-windows-static</VcpkgTriplet>
```
使用win32 x86应用程序:
```
<VcpkgTriplet Condition="'$(Platform)'=='Win32'">x86-windows-static</VcpkgTriplet>
```
使用win64 x64应用程序:
```
<VcpkgTriplet Condition="'$(Platform)'=='x64'">x64-windows-static</VcpkgTriplet>
```

x64-windows-static 生成x64的MT静态库
x64-windows-static-md 生成x64的MD静态库(还没试过)
x64-windows 生成x64的动态库

# 二.修改vs设置或vcxproj
## 1.基本问题
首先的问题是libcurl的配置，当然，这其实也不能算是个问题：工程配置中添加头文件和lib文件的引用

头文件：项目右键--》属性--》配置属性--》C/C++--》附件包含目录

lib文件路径：项目右键--》属性--》配置属性--》链接器--》附件库目录

lib文件：项目右键--》属性--》配置属性--》链接器--》输入--》附件依赖项
## 2.链接问题
### 首先出现的连接问题是：
```
error LNK2001: 无法解析的外部符号 __imp_curl_easy_perform
error LNK2001: 无法解析的外部符号 __imp_curl_easy_init
error LNK2001: 无法解析的外部符号 __imp_curl_slist_append
error LNK2001: 无法解析的外部符号 __imp_curl_slist_free_all
error LNK2001: 无法解析的外部符号 __imp_curl_easy_cleanup
error LNK2001: 无法解析的外部符号 __imp_curl_easy_setopt
```
解决方案：加入预编译选项：项目右键--》属性--》配置属性--》C/C++--》预处理器--》预处理器，把 BUILDING_LIBCURL或CURL_STATICLIB
winsock.h和winsock2.h冲突问题：
WIN32_LEAN_AND_MEAN
#### 对应vcxproj:(在文件末尾)
```
<ClCompile>
      <WarningLevel>TurnOffAllWarnings</WarningLevel>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
      <SDLCheck>false</SDLCheck>
      <RuntimeLibrary>MultiThreaded</RuntimeLibrary>
      <PreprocessorDefinitions>CURL_STATICLIB;WIN32_LEAN_AND_MEAN;NDEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
      <Optimization>Disabled</Optimization>
      <BufferSecurityCheck>false</BufferSecurityCheck>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <EnableCOMDATFolding>true</EnableCOMDATFolding>
      <OptimizeReferences>true</OptimizeReferences>
      <GenerateDebugInformation>false</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>
  <ItemGroup>
    <ClCompile Include="main.cpp" />
  </ItemGroup>
  <Import Project=".\libcurl.props" />
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
  <ImportGroup Label="ExtensionTargets">
  </ImportGroup>
</Project>
```
### 接着出现的问题是：
```
1>libcurl.lib(ldap.obj) : error LNK2001: 无法解析的外部符号 __imp_ldap_init
1>libcurl.lib(ldap.obj) : error LNK2001: 无法解析的外部符号 __imp_ldap_unbind_s
1>libcurl.lib(ldap.obj) : error LNK2001: 无法解析的外部符号 __imp_ldap_set_option
1>libcurl.lib(ldap.obj) : error LNK2001: 无法解析的外部符号 __imp_ldap_simple_bind_s
1>libcurl.lib(ldap.obj) : error LNK2001: 无法解析的外部符号 __imp_ldap_search_s
1>libcurl.lib(ldap.obj) : error LNK2001: 无法解析的外部符号 __imp_ldap_msgfree
1>libcurl.lib(ldap.obj) : error LNK2001: 无法解析的外部符号 __imp_ldap_err2string
1>libcurl.lib(ldap.obj) : error LNK2001: 无法解析的外部符号 __imp_ldap_first_entry
1>libcurl.lib(ldap.obj) : error LNK2001: 无法解析的外部符号 __imp_ldap_next_entry
1>libcurl.lib(ldap.obj) : error LNK2001: 无法解析的外部符号 __imp_ldap_first_attribute
1>libcurl.lib(ldap.obj) : error LNK2001: 无法解析的外部符号 __imp_ldap_next_attribute
1>libcurl.lib(ldap.obj) : error LNK2001: 无法解析的外部符号 __imp_ldap_get_values_len
1>libcurl.lib(ldap.obj) : error LNK2001: 无法解析的外部符号 __imp_ldap_value_free_len
1>libcurl.lib(ldap.obj) : error LNK2001: 无法解析的外部符号 __imp_ldap_get_dn
1>libcurl.lib(ldap.obj) : error LNK2001: 无法解析的外部符号 __imp_ldap_memfree
1>libcurl.lib(ldap.obj) : error LNK2001: 无法解析的外部符号 __imp_ber_free
```
解决方案：给工程添加依赖的库：项目右键--》属性--》配置属性--》链接器--》输入--》附件依赖项，把 ws2_32.lib、 winmm.lib、 wldap32.lib添加进去
若使用props:
```
<?xml version="1.0" encoding="utf-8"?>
<Project xmlns="http://schemas.microsoft.com/developer/msbuild/2003"
    ToolsVersion="4.0">
    <ImportGroup Label="PropertySheets" />
    <PropertyGroup Label="UserMacros" />
    <PropertyGroup />
    
    <ItemDefinitionGroup>
        <ClCompile>
            <AdditionalIncludeDirectories>%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
            <TreatWChar_tAsBuiltInType>true</TreatWChar_tAsBuiltInType>
        </ClCompile>
        <Link>
            <AdditionalLibraryDirectories>%(AdditionalLibraryDirectories);</AdditionalLibraryDirectories>
            <AdditionalDependencies>%(AdditionalDependencies);libcurl.lib;zlib.lib;Ws2_32.lib;Wldap32.lib;winmm.lib;Crypt32.lib;Normaliz.lib</AdditionalDependencies>
        </Link>
    </ItemDefinitionGroup>

</Project>
```
说明：必须添加libcurl.lib;zlib.lib;Ws2_32.lib;Wldap32.lib;winmm.lib;Crypt32.lib;Normaliz.lib
否则会报错:
```
libcurl.lib(xxx.obj) : error LNK2001: 无法解析的外部符号 __imp_xxx
```
最后出现的问题：
```
1>libeay32MDd.lib(e_capi.obj) : error LNK2001: 无法解析的外部符号__imp__CertGetCertificateContextProperty@16
1>libeay32MDd.lib(e_capi.obj) : error LNK2001: 无法解析的外部符号 __imp__CertOpenStore@20
1>libeay32MDd.lib(e_capi.obj) : error LNK2001: 无法解析的外部符号 __imp__CertCloseStore@8
1>libeay32MDd.lib(e_capi.obj) : error LNK2001: 无法解析的外部符号 __imp__CertEnumCertificatesInStore@8
1>libeay32MDd.lib(e_capi.obj) : error LNK2001: 无法解析的外部符号 __imp__CertFreeCertificateContext@4
1>libeay32MDd.lib(e_capi.obj) : error LNK2001: 无法解析的外部符号 __imp__CertFindCertificateInStore@24
1>libeay32MDd.lib(e_capi.obj) : error LNK2001: 无法解析的外部符号 __imp__CertDuplicateCertificateContext@4
```
解决方案：给工程添加依赖的库：项目右键--》属性--》配置属性--》链接器--》输入--》附件依赖项，把 Crypt32.lib添加进去
## 3.MT，MD选择问题
MT为.lib
MD为.dll
```
<RuntimeLibrary>MultiThreaded</RuntimeLibrary>
```
对应vcxproj:
```
<ClCompile>
      <WarningLevel>TurnOffAllWarnings</WarningLevel>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
      <SDLCheck>false</SDLCheck>
      <RuntimeLibrary>MultiThreaded</RuntimeLibrary>
      <PreprocessorDefinitions>CURL_STATICLIB;WIN32_LEAN_AND_MEAN;NDEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
      <Optimization>Disabled</Optimization>
      <BufferSecurityCheck>false</BufferSecurityCheck>
```
# 附：

yml:
```
name: rqtest
on:
  push:
    branches: [ "main" ]

env:
  # Path to the solution file relative to the root of the project.
  SOLUTION_FILE_PATH: .

  # Configuration type to build.
  # You can convert this to a build matrix if you need coverage of multiple configuration types.
  # https://docs.github.com/actions/learn-github-actions/managing-complex-workflows#using-a-build-matrix
  #BUILD_CONFIGURATION: Release

permissions:
  contents: write

jobs:
  build:
    runs-on: windows-latest

    steps:
    - uses: actions/checkout@v4

    - name: Add MSBuild to PATH
      uses: microsoft/setup-msbuild@v2

    - name: Restore NuGet packages
      working-directory: ${{env.GITHUB_WORKSPACE}}
      run: nuget restore ${{env.SOLUTION_FILE_PATH}}
      

    
    # - name: Deploy
      # uses: peaceiris/actions-gh-pages@v4
      # with:
        # github_token: ${{ secrets.GITHUB_TOKEN }}
        # publish_dir: ./vcpkg/packages
        # keep_files: true
              
    - name: build
      run: |
           #New-Item -Path ".\" -Name "libcurl" -ItemType "directory"
           Invoke-WebRequest -Uri "https://raw.github.com/x500x/building/refs/heads/x64-windows-static%408.11.0/zlib_x64-windows-static/lib/zlib.lib" -OutFile ".\rqtest\zlib.lib"
           Invoke-WebRequest -Uri "https://raw.github.com/x500x/building/refs/heads/x64-windows-static%408.11.0/curl_x64-windows-static/lib/libcurl.lib" -OutFile ".\rqtest\libcurl.lib"
           msbuild /m /p:Configuration=Release ${{env.SOLUTION_FILE_PATH}}  #x64
           #msbuild /m /p:Configuration=Release;Platform="x86" ${{env.SOLUTION_FILE_PATH}}  #x86
           
    - name: upload__artifact2
      uses: actions/upload-artifact@5d5d22a31266ced268874388b861e4b58bb5c2f3 # v4.3.1
      with:
       path: .\x64
       
    - name: test
      run: .\x64\Release\ProjectProc.exe
       
    # - name: Deploy
      # uses: peaceiris/actions-gh-pages@v4
      # with:
        # github_token: ${{ secrets.GITHUB_TOKEN }}
        # publish_dir: .\vcpkg\installed\x64-windows-static\lib\
        # keep_files: true
```
vcpkg_building:
```
name: vcpkg_building

on:
  push:
    branches: [ "main" ]

env:
  # Path to the solution file relative to the root of the project.
  SOLUTION_FILE_PATH: .

  # Configuration type to build.
  # You can convert this to a build matrix if you need coverage of multiple configuration types.
  # https://docs.github.com/actions/learn-github-actions/managing-complex-workflows#using-a-build-matrix
  #BUILD_CONFIGURATION: Release

permissions:
  contents: write

jobs:
  build:
    runs-on: windows-latest

    steps:
    - uses: actions/checkout@v4

    - name: Add MSBuild to PATH
      uses: microsoft/setup-msbuild@v2

    # - name: Restore NuGet packages
      # working-directory: ${{env.GITHUB_WORKSPACE}}
      # run: nuget restore ${{env.SOLUTION_FILE_PATH}}
      
    - name: use-vcpkg
      run: |
          git clone https://github.com/Microsoft/vcpkg.git
          cd vcpkg
          ./bootstrap-vcpkg.sh
          vcpkg integrate install
          vcpkg install curl:x64-windows-static
          Move-Item -Path .\ -Destination C:\Logs
          #vcpkg install curl[brotli,c-ares,http2,ldap,non-http,openssl,tool,websockets,zstd]:x64-windows
          #vcpkg install curl[openssl]:x64-windows-static
    
    - name: Deploy
      uses: peaceiris/actions-gh-pages@v4
      with:
        github_token: ${{ secrets.GITHUB_TOKEN }}
        publish_dir: ./vcpkg/packages
        keep_files: true
```

vcxproj:
```
<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">
    <ProjectConfiguration Include="Debug|Win32">
      <Configuration>Debug</Configuration>
      <Platform>Win32</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|Win32">
      <Configuration>Release</Configuration>
      <Platform>Win32</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Debug|x64">
      <Configuration>Debug</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|x64">
      <Configuration>Release</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
  </ItemGroup>
  <PropertyGroup Label="Globals">
    <VCProjectVersion>16.0</VCProjectVersion>
    <Keyword>Win32Proj</Keyword>
    <ProjectGuid>{7fa92def-b64e-46d6-b7a6-9fec17d09ff8}</ProjectGuid>
    <RootNamespace>ProjectProc</RootNamespace>
    <WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>
    <VcpkgTriplet Condition="'$(Platform)'=='x64'">x64-windows-static</VcpkgTriplet>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>true</UseDebugLibraries>
    <PlatformToolset>v143</PlatformToolset>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <PlatformToolset>v143</PlatformToolset>
    <WholeProgramOptimization>true</WholeProgramOptimization>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>true</UseDebugLibraries>
    <PlatformToolset>v143</PlatformToolset>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <PlatformToolset>v143</PlatformToolset>
    <WholeProgramOptimization>true</WholeProgramOptimization>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
  <ImportGroup Label="ExtensionSettings">
  </ImportGroup>
  <ImportGroup Label="Shared">
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <EmbedManifest>false</EmbedManifest>
  </PropertyGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <SDLCheck>true</SDLCheck>
      <PreprocessorDefinitions>WIN32;_DEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <GenerateDebugInformation>true</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
      <SDLCheck>true</SDLCheck>
      <PreprocessorDefinitions>WIN32;NDEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <EnableCOMDATFolding>true</EnableCOMDATFolding>
      <OptimizeReferences>true</OptimizeReferences>
      <GenerateDebugInformation>true</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <SDLCheck>true</SDLCheck>
      <PreprocessorDefinitions>_DEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <GenerateDebugInformation>true</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <ClCompile>
      <WarningLevel>TurnOffAllWarnings</WarningLevel>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
      <SDLCheck>false</SDLCheck>
      <RuntimeLibrary>MultiThreaded</RuntimeLibrary>
      <PreprocessorDefinitions>CURL_STATICLIB;WIN32_LEAN_AND_MEAN;NDEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
      <Optimization>Disabled</Optimization>
      <BufferSecurityCheck>false</BufferSecurityCheck>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <EnableCOMDATFolding>true</EnableCOMDATFolding>
      <OptimizeReferences>true</OptimizeReferences>
      <GenerateDebugInformation>false</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>
  <ItemGroup>
    <ClCompile Include="main.cpp" />
  </ItemGroup>
  <Import Project=".\libcurl.props" />
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
  <ImportGroup Label="ExtensionTargets">
  </ImportGroup>
</Project>

```

libcurl.props:
```
<?xml version="1.0" encoding="utf-8"?>
<Project xmlns="http://schemas.microsoft.com/developer/msbuild/2003"
    ToolsVersion="4.0">
    <ImportGroup Label="PropertySheets" />
    <PropertyGroup Label="UserMacros" />
    <PropertyGroup />
    
    <ItemDefinitionGroup>
        <ClCompile>
            <AdditionalIncludeDirectories>%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
            <TreatWChar_tAsBuiltInType>true</TreatWChar_tAsBuiltInType>
        </ClCompile>
        <Link>
            <AdditionalLibraryDirectories>%(AdditionalLibraryDirectories);</AdditionalLibraryDirectories>
            <AdditionalDependencies>%(AdditionalDependencies);libcurl.lib;zlib.lib;Ws2_32.lib;Wldap32.lib;winmm.lib;Crypt32.lib;Normaliz.lib</AdditionalDependencies>
        </Link>
    </ItemDefinitionGroup>

</Project>
```