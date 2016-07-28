:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Build messages for inclusion in the MlbDev/ProtoBuf library.
:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: 
:: Revision History  :  2016-06-26 --- Creation.
::                         Michael L. Brock
:: 
:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

set SRC_DIR=./

set DST_DIR=./CppOut

\Vendor\GoogleProtoBuf\protobuf-2.6.1\protoc.exe -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/ArrayTypes.proto
\Vendor\GoogleProtoBuf\protobuf-2.6.1\protoc.exe -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/FieldTypeTest.proto

:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

