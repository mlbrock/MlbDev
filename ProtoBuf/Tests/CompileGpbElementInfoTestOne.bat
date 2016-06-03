set SRC_DIR=\DevEnv\GitRepos\MiscProjects\GbpUtils\Tests

set DST_DIR=\DevEnv\GitRepos\MiscProjects\GbpUtils\Tests\CppOut

\Vendor\GoogleProtoBuf\protobuf-2.6.1\protoc.exe -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%\GpbElementInfoTestOne.proto


