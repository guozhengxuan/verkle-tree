hunter_config(
        Boost VERSION "1.79.0"
        URL
        "https://osp-1257653870.cos.ap-guangzhou.myqcloud.com/FISCO-BCOS/FISCO-BCOS/deps/boost_1_79_0.tar.bz2
        https://downloads.sourceforge.net/project/boost/boost/1.79.0/source/boost_1_79_0.tar.bz2
        https://nchc.dl.sourceforge.net/project/boost/boost/1.79.0/boost_1_79_0.tar.bz2"
        SHA1
        31209dcff292bd6a64e5e08ceb3ce44a33615dc0
        CMAKE_ARGS
        CONFIG_MACRO=BOOST_UUID_RANDOM_PROVIDER_FORCE_POSIX
)