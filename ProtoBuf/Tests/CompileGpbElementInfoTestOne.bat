:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Build test messages for the MlbDev/ProtoBuf library
:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: 
:: File Name       : CompileGpbElementInfoTestOne.bat
::
:: File Description: Build test messages for the MlbDev/ProtoBuf library.
::
:: Revision History: 2015-06-20 --- Created in GpbUtils / MiscProjects repo
::                      Michael L. Brock
::                   2016-06-03 --- Copied over to ProtoBuf / MlbDev repo.
::                      Michael L. Brock
:: 
::      Copyright Michael L. Brock 2015 - 2018.
::
::      Distributed under the Boost Software License, Version 1.0.
::      (See accompanying file LICENSE_1_0.txt or copy at
::      http://www.boost.org/LICENSE_1_0.txt)
::
:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

set SRC_DIR=\DevEnv\GitRepos\MiscProjects\GbpUtils\Tests

set DST_DIR=\DevEnv\GitRepos\MiscProjects\GbpUtils\Tests\CppOut

\Vendor\GoogleProtoBuf\protobuf-2.6.1\protoc.exe -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%\GpbElementInfoTestOne.proto

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

