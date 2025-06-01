FROM mcr.microsoft.com/dotnet/framework/runtime:4.8-windowsservercore-ltsc2019

RUN powershell -Command "if (-not (Test-Path 'C:\ProgramData')) { New-Item -ItemType Directory -Path 'C:\ProgramData' }"

RUN powershell -Command "Set-ExecutionPolicy Bypass -Scope Process -Force; \
    [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.SecurityProtocolType]::Tls12; \
    Invoke-WebRequest 'https://community.chocolatey.org/install.ps1' -OutFile 'install.ps1'; \
    .\install.ps1; Remove-Item -Force install.ps1"

RUN powershell -Command "$env:PATH = $env:PATH + ';C:\ProgramData\chocolatey\bin'; choco -v"

RUN powershell -Command "choco install -y visualstudio2022buildtools --package-parameters '--add Microsoft.VisualStudio.Workload.VCTools --add Microsoft.VisualStudio.Component.CMake.Tools --add Microsoft.VisualStudio.Component.Windows10SDK.19041 --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 --add Microsoft.VisualStudio.Component.ClangCL --add Microsoft.VisualStudio.Component.VC.Llvm.Clang'"

RUN powershell -Command "choco install -y ninja"


RUN powershell -Command "choco install -y cmake --installargs 'ADD_CMAKE_TO_PATH=System'; \
    choco install -y git;"

RUN powershell -Command "choco install -y llvm"
    
ENV CC="C:/Program Files/LLVM/bin/clang-cl.exe"
ENV CXX="C:/Program Files/LLVM/bin/clang-cl.exe"
ENV LLVM_COV="C:/Program Files/LLVM/bin/llvm-cov.exe"
ENV LLVM_PROFDATA="C:/Program Files/LLVM/bin/llvm-profdata.exe"


ENV VCPKG_PATH C:/vcpkg
ENV VCPKG_TARGET_TRIPLET x64-windows
ENV CMAKE_GENERATOR "Visual Studio 17 2022"

RUN git clone https://github.com/microsoft/vcpkg.git C:/vcpkg
WORKDIR C:/vcpkg
RUN powershell -Command ".\bootstrap-vcpkg.bat"

RUN vcpkg integrate install
RUN C:/vcpkg/vcpkg install glew[core]:x64-windows sdl2[core]:x64-windows sdl2-ttf[core]:x64-windows glm:x64-windows sdl2-image[core]:x64-windows gtest:x64-windows

ENV PATH="${PATH};C:\\Program Files\\LLVM\\bin;C:\\ProgramData\\chocolatey\\bin;C:\\Program Files\\Git\\cmd;C:\\vcpkg\\installed\\x64-windows\\debug\\bin;C:\\Program Files\\CMake\\bin;C:\\Windows\\System32\\WindowsPowerShell\\v1.0;C:\\Program Files\\LLVM\\bin"

WORKDIR C:/src
COPY . /src/

ENTRYPOINT ["C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe"]
