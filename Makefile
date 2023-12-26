TARGET   := rt_iow_gpu
SRCS     := src/main.cpp
INCLUDES := $(wildcard include/*.hpp)

CXX      := icpx
SYCLFLAGS := -fsycl
CXXFLAGS := -Iinclude -std=c++20 -Wall -Wextra
LDFLAGS  :=

ifeq ($(BUILD_MODE), release)
    CXXFLAGS += -O3
else
    CXXFLAGS += -Og
endif

.PHONY: clean

$(TARGET): main.o
	$(CXX) -o $@ $^ $(SYCLFLAGS) $(LDFLAGS)

%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(SYCLFLAGS) $(CXXFLAGS)

main.o: $(INCLUDES)

clean:
	-$(RM) $(TARGET)

