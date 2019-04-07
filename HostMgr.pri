DESTDIR = ../../build/bin/
OBJECTS_DIR += ../../build/tmp
QMAKE_LFLAGS += -Wl,-rpath,\'\$\$ORIGING\':\'\$\$ORIGING/../lib\':../lib:./
DEFINES += BOOST_LOG_DYN_LINK
LIBS += -lboost_log_setup \
        -lboost_log \
        -ldl \
        -lboost_thread \
        -lpthread \
        -lboost_system \
        -lboost_filesystem \
        -lboost_regex \
        -lboost_date_time\
        -lboost_chrono\
        -lboost_atomic\
        -lsqlite3\
        -ljsoncpp \
        -lnl-3

INCLUDEPATH += /usr/include/libnl3/ \

#LIBS += -L$$DESTDIR/ -lBaseEnv -lPublicFun

