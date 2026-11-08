:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Build messages for inclusion in the MlbDev/ProtoBuf library.
:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: 
:: File Name       :  CompileMessageTypes.bat
::
:: File Description:  Build messages for the MlbDev/ProtoBuf library.
::
:: Revision History: 2016-06-26 --- Creation.
::                      Michael L. Brock
:: 
::      Copyright Michael L. Brock 2016 - 2026.
::
::      Distributed under the Boost Software License, Version 1.0.
::      (See accompanying file LICENSE_1_0.txt or copy at
::      http://www.boost.org/LICENSE_1_0.txt)
::
:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

set SRC_DIR=./

set DST_DIR=./CppOut

\Vendor\GoogleProtoBuf\protobuf-2.6.1\protoc.exe -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/ArrayTypes.proto
\Vendor\GoogleProtoBuf\protobuf-2.6.1\protoc.exe -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/FieldTypeTest.proto

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

