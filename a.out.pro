QT += widgets
QMAKE_CXXFLAGS += -mfpmath=sse -fstack-protector-all -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format
TARGET        = a.out
HEADERS       = window.h \
                methods.h
SOURCES       = main.cpp \
                window.cpp \
                methods.cpp \
		paint.cpp   \
