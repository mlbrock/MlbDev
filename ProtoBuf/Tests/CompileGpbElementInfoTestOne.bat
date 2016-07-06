:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Test message for the MlbDev/ProtoBuf library.
:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: 
:: Revision History  :  2015-06-20 --- Created in GpbUtils / MiscProjects repo
::                         Michael L. Brock
::                      2016-06-03 --- Copied over to ProtoBuf / MlbDev repo.
::                         Michael L. Brock
:: 
:: ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

set SRC_DIR=\DevEnv\GitRepos\MiscProjects\GbpUtils\Tests

set DST_DIR=\DevEnv\GitRepos\MiscProjects\GbpUtils\Tests\CppOut

\Vendor\GoogleProtoBuf\protobuf-2.6.1\protoc.exe -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%\GpbElementInfoTestOne.proto


